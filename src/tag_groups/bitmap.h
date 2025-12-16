#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"

enum {
    BITMAP_FLAGS_DIFFUSION_DITHER_BIT,
    BITMAP_FLAGS_DISABLE_VECTOR_COMPRESSION_BIT,
    BITMAP_FLAGS_UNIFORM_SPRITE_SEQUENCES_BIT,
    BITMAP_FLAGS_EXTRACT_SPRITES_FILTHY_BUG_FIX_BIT,
    BITMAP_FLAGS_HALF_HUD_SCALE_BIT,
    BITMAP_FLAGS_INVERT_DETAIL_FADE_BIT,
    BITMAP_FLAGS_USE_AVERAGE_COLOR_FOR_DETAIL_FADE_BIT,
    BITMAP_FLAGS_FORCE_HUD_USE_HIGHRES_SCALE_BIT,
    NUMBER_OF_BITMAP_FLAGS
};

enum {
    BITMAP_SPRITE_FLAGS_NO_ALPHA_BLEED_BIT,
    NUMBER_OF_BITMAP_SPRITE_FLAGS
};

enum {
    BITMAP_DATA_FLAGS_HAS_POWER_OF_TWO_DIMENSIONS_BIT,
    BITMAP_DATA_FLAGS_COMPRESSED_BIT,
    BITMAP_DATA_FLAGS_PALETTIZED_BIT,
    BITMAP_DATA_FLAGS_SWIZZLED_BIT,
    BITMAP_DATA_FLAGS_LINEAR_BIT,
    BITMAP_DATA_FLAGS_FORMAT_V16U16_BIT,
    BITMAP_DATA_FLAGS_RUNTIME_CREATED_BIT,
    BITMAP_DATA_FLAGS_CACHED_BIT,
    BITMAP_DATA_FLAGS_EXTERNAL_BIT,
    BITMAP_DATA_FLAGS_ENVIRONMENT_BIT,
    NUMBER_OF_BITMAP_DATA_FLAGS
};

enum {
    BITMAP_TYPE_2D_TEXTURES,
    BITMAP_TYPE_3D_TEXTURES,
    BITMAP_TYPE_CUBE_MAPS,
    BITMAP_TYPE_SPRITES,
    BITMAP_TYPE_INTERFACE_BITMAPS,
    NUMBER_OF_BITMAP_TYPES
};

enum {
    BITMAP_FORMAT_COMPRESSED_COLOR_KEY_TRANSPARENCY,
    BITMAP_FORMAT_COMPRESSED_EXPLICIT_ALPHA,
    BITMAP_FORMAT_COMPRESSED_INTERPOLATED_ALPHA,
    BITMAP_FORMAT_16BIT_COLOR,
    BITMAP_FORMAT_32BIT_COLOR,
    BITMAP_FORMAT_MONOCHROME,
    BITMAP_FORMAT_HIGH_QUALITY_COMPRESSION,
    NUMBER_OF_BITMAP_FORMATS
};

enum {
    BITMAP_USAGE_ALPHA_BLEND,
    BITMAP_USAGE_DEFAULT,
    BITMAP_USAGE_HEIGHT_MAP,
    BITMAP_USAGE_DETAIL_MAP,
    BITMAP_USAGE_LIGHT_MAP,
    BITMAP_USAGE_VECTOR_MAP,
    NUMBER_OF_BITMAP_USAGES
};

enum {
    BITMAP_SPRITE_BUDGET_32,
    BITMAP_SPRITE_BUDGET_64,
    BITMAP_SPRITE_BUDGET_128,
    BITMAP_SPRITE_BUDGET_256,
    BITMAP_SPRITE_BUDGET_512,
    BITMAP_SPRITE_BUDGET_1024,
    NUMBER_OF_BITMAP_SPRITE_BUDGETS
};

enum {
    BITMAP_SPRITE_USAGE_BLEND_ADD_SUB_MAX,
    BITMAP_SPRITE_USAGE_MUL_MIN,
    BITMAP_SPRITE_USAGE_DOUBLE_MULTIPLY,
    NUMBER_OF_BITMAP_SPRITE_USAGES
};

enum {
    BITMAP_DATA_TYPE_2D,
    BITMAP_DATA_TYPE_3D,
    BITMAP_DATA_TYPE_CUBE_MAP,
    NUMBER_OF_BITMAP_DATA_TYPES
};

enum {
    BITMAP_DATA_FORMAT_A8,
    BITMAP_DATA_FORMAT_Y8,
    BITMAP_DATA_FORMAT_AY8,
    BITMAP_DATA_FORMAT_A8Y8,
    BITMAP_DATA_FORMAT_UNUSED1,
    BITMAP_DATA_FORMAT_UNUSED2,
    BITMAP_DATA_FORMAT_R5G6B5,
    BITMAP_DATA_FORMAT_UNUSED3,
    BITMAP_DATA_FORMAT_A1R5G5B5,
    BITMAP_DATA_FORMAT_A4R4G4B4,
    BITMAP_DATA_FORMAT_X8R8G8B8,
    BITMAP_DATA_FORMAT_A8R8G8B8,
    BITMAP_DATA_FORMAT_UNUSED4,
    BITMAP_DATA_FORMAT_UNUSED5,
    BITMAP_DATA_FORMAT_DXT1,
    BITMAP_DATA_FORMAT_DXT3,
    BITMAP_DATA_FORMAT_DXT5,
    BITMAP_DATA_FORMAT_P8_BUMP,
    BITMAP_DATA_FORMAT_BC7,
    NUMBER_OF_BITMAP_DATA_FORMATS
};

#pragma pack(push, 1)

struct bitmap_sprite {
    int16_t bitmap_index;
    uint16_t pad;
    uint32_t unused;
    float_rectangle2d bounds;
    float_point2d registration_point;
};
static_assert(sizeof(struct bitmap_sprite) == 32);

struct bitmap_sequence {
    String32 name;
    int16_t first_bitmap_index;
    int16_t bitmap_count;
    uint32_t unused[4];
    struct tag_reflexive sprites;
};
static_assert(sizeof(struct bitmap_sequence) == 64);

struct bitmap_data {
    uint32_t signature;
    int16_t width;
    int16_t height;
    int16_t depth;
    uint16_t type;
    uint16_t format;
    uint16_t flags;
    point2d registration_point;
    int16_t mipmap_count;
    uint16_t pad;
    uint32_t pixels_offset;
    uint32_t pixels_size;
    TagID tag_index;
    int32_t cache_block_index;
    Pointer32 hardware_format;
    Pointer32 base_address;
};
static_assert(sizeof(struct bitmap_data) == 48);

struct bitmap {
    uint16_t type;
    uint16_t format;
    uint16_t usage;
    uint16_t flags;
    float detail_fade;
    float sharpen_amount;
    float bump_height;
    uint16_t sprite_budget_size;
    int16_t sprite_budget_count;
    uint16_t import_width;
    uint16_t import_height;
    struct tag_data import_bitmap;
    struct tag_data pixel_data;
    float smoothing_filter_size;
    float alpha_bias;
    int16_t mipmap_count;
    uint16_t sprite_usage;
    int16_t sprite_spacing;
    uint16_t pad;
    struct tag_reflexive sequences;
    struct tag_reflexive bitmaps;
};
static_assert(sizeof(struct bitmap) == 108);

#pragma pack(pop)

#define bitmap_get_sequence(bitmap, index, data) tag_reflexive_get_element(&(bitmap)->sequences, index, sizeof(struct bitmap_sequence), data)
#define bitmap_get_sprite(sequence, index, data) tag_reflexive_get_element(&(sequence)->sprites, index, sizeof(struct bitmap_sprite), data)
#define bitmap_get_data(bitmap, index, data) tag_reflexive_get_element(&(bitmap)->bitmaps, index, sizeof(struct bitmap_data), data)

bool bitmap_final_postprocess(TagID tag, struct tag_data_instance *tag_data);
