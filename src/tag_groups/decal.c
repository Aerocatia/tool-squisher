#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"

#include "decal.h"
#include "bitmap.h"

#define NUMBER_OF_RESOURCE_MAP_DECAL_BITMAPS 38

static const char *decal_external_bitmaps[] = {
    "effects\\decals\\bullet holes\\bitmaps\\dirt pistol",
    "effects\\decals\\bullet holes\\bitmaps\\stone",
    "effects\\decals\\bullet holes\\bitmaps\\snow",
    "effects\\decals\\bullet holes\\bitmaps\\metal",
    "effects\\decals\\blood splats\\bitmaps\\blood splat",
    "effects\\decals\\bullet holes\\bitmaps\\unbreakable glass",
    "effects\\decals\\blood splats\\bitmaps\\blood splat grunt",
    "effects\\decals\\blood splats\\bitmaps\\blood splat hunter glow",
    "effects\\decals\\blood splats\\bitmaps\\blood splat hunter",
    "effects\\decals\\blood splats\\bitmaps\\blood splat elite",
    "effects\\decals\\bullet holes\\bitmaps\\ice",
    "effects\\decals\\bullet holes\\bitmaps\\plasma burn",
    "effects\\decals\\bullet holes\\bitmaps\\plasma burn inner glow",
    "effects\\decals\\bullet holes\\bitmaps\\plasma burn outer glow",
    "effects\\decals\\blood splats\\bitmaps\\blood splat engineer",
    "effects\\decals\\bullet holes\\bitmaps\\plasma burn large",
    "effects\\decals\\bullet holes\\bitmaps\\plasma green inner glow",
    "levels\\b30\\decals-environment\\bitmaps\\decal fore symb red",
    "levels\\b30\\decals-environment\\bitmaps\\decal fore symb blue",
    "levels\\test\\sidewinder\\decals-environment\\bitmaps\\fore_stripe_plate_red",
    "levels\\test\\sidewinder\\decals-environment\\bitmaps\\fore_stripe_plate_blue",
    "effects\\decals\\bullet holes\\bitmaps\\impact flame thrower char",
    "effects\\decals\\vehicle marks\\bitmaps\\snow treadmarks",
    "levels\\b30\\decals-environment\\bitmaps\\decal fore symb blue b",
    "levels\\b30\\decals-environment\\bitmaps\\decal fore symb red b",
    "levels\\test\\boardingaction\\decals-environment\\bitmaps\\1_blue",
    "levels\\test\\boardingaction\\decals-environment\\bitmaps\\2_red",
    "levels\\test\\boardingaction\\decals-environment\\bitmaps\\5_red",
    "levels\\test\\boardingaction\\decals-environment\\bitmaps\\1_red",
    "levels\\test\\boardingaction\\decals-environment\\bitmaps\\2_blue",
    "levels\\test\\boardingaction\\decals-environment\\bitmaps\\3_blue",
    "levels\\test\\boardingaction\\decals-environment\\bitmaps\\3_red",
    "levels\\test\\boardingaction\\decals-environment\\bitmaps\\4_blue",
    "levels\\test\\boardingaction\\decals-environment\\bitmaps\\4_red",
    "levels\\test\\boardingaction\\decals-environment\\bitmaps\\5_blue",
    "levels\\a10\\decals-environment\\bitmaps\\decal danger biohazard",
    "levels\\a10\\decals-environment\\bitmaps\\decal danger electric",
    "effects\\decals\\blood splats\\bitmaps\\glowingdrip splat"
};
static_assert(sizeof(decal_external_bitmaps) / sizeof(char *) == NUMBER_OF_RESOURCE_MAP_DECAL_BITMAPS);

static const float decal_external_bitmap_extents[] = {
    0.0f,  33.0f, 0.0f,  23.0f, 40.0f, 34.0f, 38.0f, 34.0f,
    34.0f, 36.0f, 64.0f, 9.0f,  13.0f, 34.0f, 38.0f, 10.0f,
    13.0f, 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  34.0f
};
static_assert(sizeof(decal_external_bitmap_extents) / sizeof(float) == NUMBER_OF_RESOURCE_MAP_DECAL_BITMAPS);

bool decal_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct decal *decal = tag_get(tag, TAG_FOURCC_DECAL, tag_data);
    if(!decal) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n",
        tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_DECAL));
        return false;
    }

    // tool.exe has code to calculate the maximum sprite extent, but for some reason it's
    // considered to have no bitmap when the code is run so it always defaults to 16.0f
    auto map = decal->shader.decal.map.index;
    if(!TEST_FLAG(decal->flags, DECAL_FLAGS_SPRITE_SCALE_BUG_FIX_BIT) || map.whole_id == NULL_ID) {
        decal->runtime_maximum_sprite_extent = 16.0f;
        return true;
    }

    // Calculate runtime_maximum_sprite_extent based on the bitmap
    if(!tag_id_is_valid_tag(map, tag_data)) {
        fprintf(stderr,"decal \"%s.%s\" references an invalid bitmap\n",
        tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_DECAL));
        return false;
    }

    // Cheat here and use a pre-calculated value for bitmaps stored in Custom Edition bitmaps.map since we know what they are
    // Ideally the game would re-calculate this on map load when loading external bitmap tags
    if(tag_is_external(map, tag_data)) {
        for(size_t i = 0; i < NUMBER_OF_RESOURCE_MAP_DECAL_BITMAPS; i++) {
            if(strcmp(tag_path_get(map, tag_data), decal_external_bitmaps[i]) == 0) {
                decal->runtime_maximum_sprite_extent = decal_external_bitmap_extents[i];
                return true;
            }
        }

        fprintf(stderr,"decal \"%s.%s\" references external bitmap \"%s.%s\" that does not match a known decal bitmap and cannot be processed by this tool\n",
        tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_DECAL), tag_path_get(map, tag_data), tag_fourcc_to_extension(TAG_FOURCC_BITMAP));
        return false;
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
