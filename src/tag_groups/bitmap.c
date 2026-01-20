#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"
#include "../resources/resources.h"

#include "bitmap.h"

bool bitmap_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    // Nothing to process?
    if(!tag_data->indexed_external_tags || tag_is_external(tag, tag_data)) {
        return true;
    }

    // Fix maps using direct bitmaps.map data offsets if they should be using external tags
    // This happens due to an oversight in tool.exe when checking if tags are the same as the ones in the resource maps or not
    struct bitmap *bitmap_group = tag_get(tag, TAG_FOURCC_BITMAP, tag_data);
    const char *tag_path = tag_path_get(tag, tag_data);
    if(!bitmap_group) {
        fprintf(stderr, "tag data for \"%s.%s\" is invalid\n", tag_path, tag_fourcc_to_extension(TAG_FOURCC_BITMAP));
        return false;
    }

    bool make_external = false;
    for(size_t i = 0; i < bitmap_group->bitmaps.count; i++) {
        struct bitmap_data *bitmap = bitmap_get_data(bitmap_group, i, tag_data);
        if(!bitmap) {
            fprintf(stderr, "bitmap data %zu in \"%s.%s\" is out of bounds\n", i, tag_path, tag_fourcc_to_extension(TAG_FOURCC_BITMAP));
            return false;
        }

        if(TEST_FLAG(bitmap->flags, BITMAP_DATA_FLAGS_EXTERNAL_BIT)) {
            make_external = true;
            break;
        }
    }

    // Set an index based on tag path
    if(make_external) {
        auto resource_index = resources_get_bitmap_index(tag_path);
        if(resource_index == RESOURCE_MAP_NO_MATCH) {
            fprintf(stderr, "bitmap \"%s\" has external pixels but does not map to the stock bitmaps.map by path index\nThe map should be rebuilt\n", tag_path);
            return false;
        }

        // Zero stale reflexive data
        for(size_t i = 0; i < bitmap_group->sequences.count; i++) {
            struct bitmap_sequence *sequence = bitmap_get_sequence(bitmap_group, i, tag_data);
            if(!sequence) {
                fprintf(stderr, "bitmap sequence %zu in \"%s.%s\" is out of bounds\n",
                    i, tag_path, tag_fourcc_to_extension(TAG_FOURCC_BITMAP));
                return false;
            }

            if(!tag_reflexive_erase_element_data(&sequence->sprites, sizeof(struct bitmap_sprite), tag_data)) {
                fprintf(stderr, "sprite data for bitmap sequence %zu in \"%s.%s\" is out of bounds\n",
                    i, tag_path, tag_fourcc_to_extension(TAG_FOURCC_BITMAP));
                return false;
            }
        }

        if(!tag_reflexive_erase_element_data(&bitmap_group->sequences, sizeof(struct bitmap_sequence), tag_data)) {
            fprintf(stderr, "bitmap sequence data for \"%s.%s\" is out of bounds\n",
                tag_path, tag_fourcc_to_extension(TAG_FOURCC_BITMAP));
            return false;
        }

        if(!tag_reflexive_erase_element_data(&bitmap_group->bitmaps, sizeof(struct bitmap_data), tag_data)) {
            fprintf(stderr, "bitmap data for \"%s.%s\" is out of bounds\n",
                tag_path, tag_fourcc_to_extension(TAG_FOURCC_BITMAP));
            return false;
        }

        memset(bitmap_group, 0, sizeof(struct bitmap));
        bitmap_group = nullptr;
        tag_data->tags[tag.index].external = 1;
        tag_data->tags[tag.index].base_address = resource_index;
        fprintf(stderr, "bitmap \"%s\" had external pixels and was remapped to use bitmaps.map resource index %u\n", tag_path, resource_index);
    }

    return true;
}
