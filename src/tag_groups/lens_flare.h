#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"

enum {
    LENS_FLARE_REFLECTION_FLAGS_ROTATE_FROM_CENTER_OF_SCREEN_BIT,
    LENS_FLARE_REFLECTION_FLAGS_RADIUS_NOT_SCALED_BY_DISTANCE_BIT,
    LENS_FLARE_REFLECTION_FLAGS_RADIUS_SCALED_BY_OCCLUSION_BIT,
    LENS_FLARE_REFLECTION_FLAGS_ZBUFFER_BIT,
    NUMBER_OF_LENS_FLARE_REFLECTION_FLAGS
};

enum {
    LENS_FLARE_REFLECTION_ANIMATION_FLAGS_COLOR_INTERPOLATE_IN_HSV_BIT,
    LENS_FLARE_REFLECTION_ANIMATION_FLAGS_COLOR_INTERPOLATE_ALONG_FARTHEST_HUE_PATH_BIT,
    NUMBER_OF_LENS_FLARE_REFLECTION_ANIMATION_FLAGS
};

enum {
    LENS_FLARE_FLAGS_SUN_BIT,
    NUMBER_OF_LENS_FLARE_FLAGS
};

enum {
    LENS_FLARE_SCALE_FUNCTION_NONE,
    LENS_FLARE_SCALE_FUNCTION_NON_LOCAL_INCIDENT_ANGLE,
    LENS_FLARE_SCALE_FUNCTION_LOCAL_INCIDENT_ANGLE,
    LENS_FLARE_SCALE_FUNCTION_VIEWER_ANGLE,
    NUMBER_OF_LENS_FLARE_SCALE_FUNCTIONS
};

enum {
    LENS_FLARE_OCCLUSION_OFFSET_DIRECTION_TOWARD_VIEWER,
    LENS_FLARE_OCCLUSION_OFFSET_DIRECTION_MARKER_FORWARD,
    LENS_FLARE_OCCLUSION_OFFSET_DIRECTION_NONE,
    NUMBER_OF_LENS_FLARE_OCCLUSION_OFFSET_DIRECTIONS
};

enum {
    LENS_FLARE_CORONA_ROTATION_FUNCTION_NONE,
    LENS_FLARE_CORONA_ROTATION_FUNCTION_EYE_IN_LIGHT_SPACE,
    LENS_FLARE_CORONA_ROTATION_FUNCTION_LIGHT_IN_EYE_SPACE,
    LENS_FLARE_CORONA_ROTATION_FUNCTION_EYE_TO_LIGHT_IN_LIGHT_SPACE,
    LENS_FLARE_CORONA_ROTATION_FUNCTION_EYE_TO_LIGHT_IN_EYE_SPACE,
    NUMBER_OF_LENS_FLARE_CORONA_ROTATION_FUNCTIONS
};

#pragma pack(push, 1)

struct lens_flare_reflection {
    uint16_t flags;
    uint16_t type;
    int16_t bitmap_index;
    uint16_t pad;
    uint32_t unused1[5];
    float offset;
    float rotation_offset;
    uint32_t unused2[1];
    float_bounds radius;
    uint16_t radius_scale_function;
    uint16_t radius_pad;
    float_bounds brightness;
    uint16_t brightness_scale_function;
    uint16_t brightness_pad;
    float_argb_color tint_color;
    float_argb_color_bounds animation_color;
    uint16_t animation_flags;
    uint16_t animation_function;
    float animation_period;
    float animation_phase;
    uint32_t unused3[1];
};
static_assert(sizeof(struct lens_flare_reflection) == 128);

struct lens_flare {
    float falloff_angle;
    float cutoff_angle;
    float runtime_cosine_falloff_angle;
    float runtime_cosine_cutoff_angle;
    float occlusion_radius;
    uint16_t occlusion_offset_direction;
    uint16_t occlusion_pad;
    float near_fade_distance;
    float far_fade_distance;
    struct tag_reference primary_map;
    uint16_t flags;
    uint16_t pad;
    uint32_t unused1[19];
    uint16_t corona_rotation_function;
    uint16_t corona_rotation_pad;
    float corona_rotation_function_scale;
    uint32_t unused2[6];
    float_vector2d corona_radius_scale;
    uint32_t unused3[7];
    struct tag_reflexive reflections;
    uint32_t unused4[8];
};
static_assert(sizeof(struct lens_flare) == 240);

#pragma pack(pop)

bool lens_flare_final_postprocess(TagID tag, struct tag_data_instance *tag_data);
