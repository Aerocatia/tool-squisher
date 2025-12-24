#pragma once

#include <stdint.h>
#include <stdio.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "shader.h"

enum  {
    SHADER_MODEL_FLAGS_DETAIL_AFTER_REFLECTION_BIT,
    SHADER_MODEL_FLAGS_TWO_SIDED_BIT,
    SHADER_MODEL_FLAGS_NOT_ALPHA_TESTED_BIT,
    SHADER_MODEL_FLAGS_ALPHA_BLENDED_DECAL_BIT,
    SHADER_MODEL_FLAGS_TRUE_ATMOSPHERIC_FOG_BIT,
    SHADER_MODEL_FLAGS_DISABLE_TWO_SIDED_CULLING_BIT,
    SHADER_MODEL_FLAGS_USE_XBOX_MULTIPURPOSE_CHANNEL_ORDER_BIT,
    NUMBER_OF_SHADER_MODEL_FLAGS
};

enum {
    SHADER_MODEL_SELF_ILLUMINATION_NO_RANDOM_PHASE_BIT,
    NUMBER_OF_SHADER_MODEL_SELF_ILLUMINATION_FLAGS
};

enum {
    SHADER_MODEL_DETAIL_FUNCTION_BIASED_MULTIPLY,
    SHADER_MODEL_DETAIL_FUNCTION_MULTIPLY,
    SHADER_MODEL_DETAIL_FUNCTION_BIASED_ADD,
    NUMBER_OF_SHADER_MODEL_DETAIL_FUNCTIONS
};

enum {
    SHADER_MODEL_DETAIL_MASK_NONE,
    SHADER_MODEL_DETAIL_MASK_REFLECTION_MASK_INVERSE,
    SHADER_MODEL_DETAIL_MASK_REFLECTION_MASK,
    SHADER_MODEL_DETAIL_MASK_SELF_ILLUMINATION_MASK_INVERSE,
    SHADER_MODEL_DETAIL_MASK_SELF_ILLUMINATION_MASK,
    SHADER_MODEL_DETAIL_MASK_CHANGE_COLOR_MASK_INVERSE,
    SHADER_MODEL_DETAIL_MASK_CHANGE_COLOR_MASK,
    SHADER_MODEL_DETAIL_MASK_MULTIPURPOSE_AUXILIARY_MASK_INVERSE,
    SHADER_MODEL_DETAIL_MASK_MULTIPURPOSE_AUXILIARY_MASK,
    NUMBER_OF_SHADER_MODEL_DETAIL_MASKS
};

#pragma pack(push, 1)

struct _shader_model {
    uint16_t flags;
    uint16_t type;
    uint32_t unused1[3];
    float translucency;
    uint32_t unused2[4];
    uint16_t diffuse_change_color_source;
    uint16_t pad;
    uint32_t unused3[7];
    uint16_t self_illumination_flags;
    uint16_t self_illumination_pad;
    uint16_t self_illumination_color_source;
    uint16_t self_illumination_animation_function;
    float self_illumination_animation_period;
    float_rgb_color_bounds self_illumination_animation_color_bonds;
    uint32_t unused4[3];
    float_vector2d map_scale;
    struct tag_reference base_map;
    uint32_t unused5[2];
    struct tag_reference multipurpose_map;
    uint32_t unused6[2];
    uint16_t detail_function;
    uint16_t detail_mask;
    float detail_map_scale;
    struct tag_reference detail_map;
    float detail_map_v_scale;
    uint32_t unused7[3];
    struct shader_texture_animation animation;
    uint32_t unused8[2];
    float reflection_falloff_distance;
    float reflection_cutoff_distance;
    float_argb_color reflection_view_perpendicular_color;
    float_argb_color reflection_view_parallel_color;
    struct tag_reference reflection_map;
    uint32_t unused9[4];
    float reflection_bump_map_scale;
    struct tag_reference reflection_bump_map;
    uint32_t unused10[8];
};
static_assert(sizeof(struct _shader_model) == 400);

struct shader_model {
    struct _shader shader;
    struct _shader_model model;
};
static_assert(sizeof(struct shader_model) == 440);

#pragma pack(pop)

bool shader_model_postprocess(TagID tag, struct tag_data_instance *tag_data);
