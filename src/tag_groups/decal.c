#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"

#include "decal.h"
#include "bitmap.h"

#define NUMBER_OF_RESOURCE_MAP_DECAL_SPRITES 16

static const char *decal_external_sprite_bitmaps[] = {
    "effects\\decals\\bullet holes\\bitmaps\\stone",
    "effects\\decals\\bullet holes\\bitmaps\\metal",
    "effects\\decals\\blood splats\\bitmaps\\blood splat",
    "effects\\decals\\bullet holes\\bitmaps\\unbreakable glass",
    "effects\\decals\\blood splats\\bitmaps\\blood splat grunt",
    "effects\\decals\\blood splats\\bitmaps\\blood splat hunter glow",
    "effects\\decals\\blood splats\\bitmaps\\blood splat hunter",
    "effects\\decals\\blood splats\\bitmaps\\blood splat elite",
    "effects\\decals\\bullet holes\\bitmaps\\ice",
    "effects\\decals\\bullet holes\\bitmaps\\plasma burn" ,
    "effects\\decals\\bullet holes\\bitmaps\\plasma burn inner glow",
    "effects\\decals\\bullet holes\\bitmaps\\plasma burn outer glow" ,
    "effects\\decals\\blood splats\\bitmaps\\blood splat engineer" ,
    "effects\\decals\\bullet holes\\bitmaps\\plasma burn large",
    "effects\\decals\\bullet holes\\bitmaps\\plasma green inner glow",
    "effects\\decals\\blood splats\\bitmaps\\glowingdrip splat"
};
static_assert(sizeof(decal_external_sprite_bitmaps) / sizeof(char *) == NUMBER_OF_RESOURCE_MAP_DECAL_SPRITES);

static const float decal_external_sprite_bitmap_extents[] = {
    33.0f, 23.0f, 40.0f, 34.0f, 38.0f, 34.0f, 34.0f, 36.0f, 64.0f, 9.0f, 13.0f, 34.0f, 38.0f, 10.0f, 13.0f, 34.0f
};
static_assert(sizeof(decal_external_sprite_bitmap_extents) / sizeof(float) == NUMBER_OF_RESOURCE_MAP_DECAL_SPRITES);

bool decal_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct decal *decal = tag_get(tag, TAG_FOURCC_DECAL, tag_data);
    if(!decal) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n",
        tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_DECAL));
        return false;
    }

    // tool.exe has code to calculate the maximum sprite extent, but for some reason the bitmap
    // tag id is always considered to be NULL_ID when the code is run, so it always defaults to 16.0f
    auto map = decal->shader.decal.map.index;
    if(map.whole_id == NULL_ID) {
        decal->runtime_maximum_sprite_extent = 16.0f;
        return true;
    }

    // Do what tool.exe was intending to do
    if(!tag_id_is_valid_tag(map, tag_data)) {
        fprintf(stderr,"decal \"%s.%s\" references an invalid bitmap\n",
        tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_DECAL));
        return false;
    }

    // We are going to cheat here and use a pre-calculated value for bitmaps stored in bitmaps.map since we know that they are.
    // Yes this does mean you go to the shadow realm if the bitmaps.map contains a completely different custom sprite, but this
    // was always going to be a problem. Ideally the game would re-calculate this on map load when loading external bitmap tags.
    if(tag_is_external(map, tag_data)) {
        for(size_t i = 0; i < NUMBER_OF_RESOURCE_MAP_DECAL_SPRITES; i++) {
            if(strcmp(tag_path_get(map, tag_data), decal_external_sprite_bitmaps[i]) == 0) {
                decal->runtime_maximum_sprite_extent = decal_external_sprite_bitmap_extents[i];
                return true;
            }
        }

        // non-sprites are zero
        decal->runtime_maximum_sprite_extent = 0.0f;
        return true;
    }

    struct bitmap *bitmap_group = tag_get(map, TAG_FOURCC_BITMAP, tag_data);
    if(!bitmap_group) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n",
        tag_path_get(map, tag_data), tag_fourcc_to_extension(TAG_FOURCC_BITMAP));
    }

    float max_sprite_extent = 0.0f;
    if(bitmap_group->type == BITMAP_TYPE_SPRITES) {
        for(size_t sequence_index = 0; sequence_index < bitmap_group->sequences.count; sequence_index++) {
            struct bitmap_sequence *sequence = bitmap_get_sequence(bitmap_group, sequence_index, tag_data);
            if(!sequence) {
                fprintf(stderr,"bitmap sequence %zu in \"%s.%s\" is out of bounds\n",
                sequence_index, tag_path_get(map, tag_data), tag_fourcc_to_extension(TAG_FOURCC_BITMAP));
                return false;
            }

            for(size_t sprite_index = 0; sprite_index < sequence->sprites.count; sprite_index++) {
                struct bitmap_sprite *sprite = bitmap_get_sprite(sequence, sprite_index, tag_data);
                if(!sprite) {
                    fprintf(stderr,"bitmap sprite %zu of sequence %zu in \"%s.%s\" is out of bounds\n",
                    sprite_index, sequence_index, tag_path_get(map, tag_data), tag_fourcc_to_extension(TAG_FOURCC_BITMAP));
                    return false;
                }

                struct bitmap_data *bitmap = bitmap_get_data(bitmap_group, sprite->bitmap_index, tag_data);
                if(!bitmap) {
                    fprintf(stderr,"bitmap data %u in \"%s.%s\" is out of bounds\n",
                    sprite->bitmap_index, tag_path_get(map, tag_data), tag_fourcc_to_extension(TAG_FOURCC_BITMAP));
                    return false;
                }

                max_sprite_extent = MAX(max_sprite_extent, (sprite->registration_point.x) * (float)bitmap->width);
                max_sprite_extent = MAX(max_sprite_extent, (sprite->registration_point.y) * (float)bitmap->height);
                max_sprite_extent = MAX(max_sprite_extent, (sprite->bounds.x1 - sprite->bounds.x0 - sprite->registration_point.x) * (float)bitmap->width);
                max_sprite_extent = MAX(max_sprite_extent, (sprite->bounds.y1 - sprite->bounds.y0 - sprite->registration_point.y) * (float)bitmap->height);
            }
        }
    }

    decal->runtime_maximum_sprite_extent = max_sprite_extent;
    return true;
}
