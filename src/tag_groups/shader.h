#pragma once

#include <stdint.h>
#include <stdio.h>

#include "../data_types.h"
#include "../tag/tag.h"

enum {
    SHADER_RADIOSITY_FLAGS_SIMPLE_PARAMETERIZATION_BIT,
    SHADER_RADIOSITY_FLAGS_IGNORE_NORMALS_BIT,
    SHADER_RADIOSITY_FLAGS_TRANSPARENT_LIT_BIT,
    NUMBER_OF_SHADER_RADIOSITY_FLAGS
};

enum {
    SHADER_TYPE_SCREEN,
    SHADER_TYPE_EFFECT,
    SHADER_TYPE_DECAL,
    SHADER_TYPE_ENVIRONMENT,
    SHADER_TYPE_MODEL,
    SHADER_TYPE_TRANSPARENT_GENERIC,
    SHADER_TYPE_TRANSPARENT_CHICAGO,
    SHADER_TYPE_TRANSPARENT_CHICAGO_EXTENDED, // WTF Gearbox
    SHADER_TYPE_TRANSPARENT_WATER,
    SHADER_TYPE_TRANSPARENT_GLASS,
    SHADER_TYPE_TRANSPARENT_METER,
    SHADER_TYPE_TRANSPARENT_PLASMA,
    NUMBER_OF_SHADER_TYPES
};

enum {
    RADIOSITY_DETAIL_LEVEL_HIGH,
    RADIOSITY_DETAIL_LEVEL_MEDIUM,
    RADIOSITY_DETAIL_LEVEL_LOW,
    RADIOSITY_DETAIL_LEVEL_TURD,
    NUMBER_OF_SHADER_RADIOSITY_DETAIL_LEVELS
};

enum {
    SHADER_FRAMEBUFFER_BLEND_FUNCTION_ALPHA_BLEND,
    SHADER_FRAMEBUFFER_BLEND_FUNCTION_MULTIPLY,
    SHADER_FRAMEBUFFER_BLEND_FUNCTION_DOUBLE_MULTIPLY,
    SHADER_FRAMEBUFFER_BLEND_FUNCTION_ADD,
    SHADER_FRAMEBUFFER_BLEND_FUNCTION_REVERSE_SUBTRACT,
    SHADER_FRAMEBUFFER_BLEND_FUNCTION_MIN,
    SHADER_FRAMEBUFFER_BLEND_FUNCTION_MAX,
    SHADER_FRAMEBUFFER_BLEND_FUNCTION_ALPHA_MULTIPLY_ADD,
    NUMBER_OF_SHADER_FRAMEBUFFER_BLEND_FUNCTIONS
};

enum {
    SHADER_FRAMEBUFFER_FADE_MODE_NONE,
    SHADER_FRAMEBUFFER_FADE_MODE_FADE_WHEN_PERPENDICULAR,
    SHADER_FRAMEBUFFER_FADE_MODE_FADE_WHEN_PARALLEL,
    NUMBER_OF_SHADER_FRAMEBUFFER_FADE_MODES
};

#pragma pack(push, 1)

struct shader_radiosity_properties {
    uint16_t flags;
    uint16_t detail_level;
    float power;
    float_rgb_color color;
    float_rgb_color tint_color;
};
static_assert(sizeof(struct shader_radiosity_properties) == 32);

struct shader_physics_properties {
    uint16_t flags;
    uint16_t material_type;
};
static_assert(sizeof(struct shader_physics_properties) == 4);

struct shader {
    struct shader_radiosity_properties radiosity;
    struct shader_physics_properties physics;
    uint16_t type;
    uint16_t pad;
};
static_assert(sizeof(struct shader) == 40);

#pragma pack(pop)

bool shader_final_postprocess(TagID tag, struct tag_data_instance *tag_data);
