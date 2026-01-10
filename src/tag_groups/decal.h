#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "shader.h"

enum {
    DECAL_FLAGS_GEOMETRY_INHERITED_BY_NEXT_DECAL_IN_CHAIN_BIT,
    DECAL_FLAGS_COLOR_INTERPOLATE_IN_HSV_BIT,
    DECAL_FLAGS_COLOR_INTERPOLATE_ALONG_FARTHEST_HUE_PATH_BIT,
    DECAL_FLAGS_NO_RANDOM_ROTATION_BIT,
    DECAL_FLAGS_WATER_EFFECT_BIT,
    DECAL_FLAGS_SAPIEN_ONLY_SNAP_TO_AXIS_BIT,
    DECAL_FLAGS_SAPIEN_ONLY_INCREMENTAL_COUNTER_BIT,
    DECAL_FLAGS_ANIMATION_LOOP_BIT,
    DECAL_FLAGS_PRESERVE_ASPECT_BIT,
    DECAL_FLAGS_DISABLED_BY_BLOOD_SETTING_BIT,
    DECAL_FLAGS_SPRITE_SCALE_BUG_FIX_BIT,
    NUMBER_OF_DECAL_FLAGS
};

enum {
    DECAL_TYPE_SCRATCH,
    DECAL_TYPE_SPLATTER,
    DECAL_TYPE_BURN,
    DECAL_TYPE_SIGN,
    NUMBER_OF_DECAL_TYPES
};

enum {
    DECAL_LAYER_PRIMARY,
    DECAL_LAYER_SECONDARY,
    DECAL_LAYER_LIGHT,
    DECAL_LAYER_ALPHA_TESTED,
    DECAL_LAYER_WATER,
    NUMBER_OF_DECAL_LAYERS
};

#pragma pack(push, 1)

struct decal {
    uint16_t flags;
    uint16_t type;
    uint16_t layer;
    uint16_t pad1;
    struct tag_reference next_decal_in_chain;
    float radius_lower_bounds;
    float radius_upper_bounds;
    uint32_t unused1[3];
    float_bounds intensity_bounds;
    float_rgb_color_bounds color_bounds;
    uint32_t unused2[3];
    int16_t animation_loop_frame_index;
    int16_t animation_speed;
    uint32_t unused3[7];
    float_bounds lifetime_bounds;
    float_bounds decay_time_bounds;
    uint32_t unused5[3];
    struct shader_decal shader;
    float runtime_maximum_sprite_extent;
    uint16_t runtime_incremental_counter;
    uint16_t pad2;
    uint32_t unused6[2];
};
static_assert(sizeof(struct decal) == 268);

#pragma pack(pop)

bool decal_postprocess(TagID tag, struct tag_data_instance *tag_data);
