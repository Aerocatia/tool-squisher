#include <stdint.h>
#include <stdio.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"
#include "../resources/resources.h"

#include "bitmap.h"

bool bitmap_final_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct bitmap *bitmap_group = tag_get(tag, TAG_FOURCC_BITMAP, tag_data);
    if(!bitmap_group) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n", tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_BITMAP));
        return false;
    }

    // Nothing we can do
    if(!tag_data->indexed_external_tags) {
        return true;
    }

    // Fix maps using direct bitmaps.map data offsets if they should be using external tags
    // This happens due to an oversight in tool.exe when checking if tags are the same as the ones in the resource maps or not
    const char *tag_path = tag_path_get(tag, tag_data);
    for(size_t i = 0; i < bitmap_group->bitmaps.count; i++) {
        struct bitmap_data *bitmap = bitmap_get_data(bitmap_group, i, tag_data);
        if(!bitmap) {
            fprintf(stderr,"bitmap data %zu in \"%s.%s\" is out of bounds\n", i, tag_path, tag_fourcc_to_extension(TAG_FOURCC_BITMAP));
            return false;
        }

        // Set an index based on tag path
        if(TEST_FLAG(bitmap->flags, BITMAP_DATA_FLAGS_EXTERNAL_BIT)) {
            auto resource_index = resources_get_bitmap_index(tag_path);
            if(resource_index == RESOURCE_MAP_NO_MATCH) {
                // can't add this.
                fprintf(stderr,"bitmap \"%s\" has external pixels but does not map to the stock bitmaps.map by path index\nThe map should be rebuilt\n", tag_path);
                return false;
            }

            tag_data->tags[tag.index].external = 1;
            tag_data->tags[tag.index].base_address = resource_index;
            fprintf(stderr,"bitmap \"%s\" had external pixels and was remapped to use bitmaps.map resource index %u\n", tag_path, resource_index);
            break;
        }
    }

    return true;
}
