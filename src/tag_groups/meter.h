#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"

enum {
    METER_COLOR_ANCHOR_BOTH,
    METER_COLOR_ANCHOR_EMPTY,
    METER_COLOR_ANCHOR_FULL,
    NUMBER_OF_METER_COLOR_ANCHORS
};

enum {
    METER_COLOR_INTERPOLATION_MODE_LINEAR,
    METER_COLOR_INTERPOLATION_MODE_NEAR_EMPTY,
    METER_COLOR_INTERPOLATION_MODE_NEAR_FULL,
    METER_COLOR_INTERPOLATION_MODE_RANDOM,
    NUMBER_OF_METER_COLOR_INTERPOLATION_MODES
};

#pragma pack(push, 1)

struct meter_encoded_pixel {
    uint8_t mask;
    uint8_t pad;
    uint16_t level;
};
static_assert(sizeof(struct meter_encoded_pixel) == 4);

struct meter_encoded_row {
    point2d origin;
    uint16_t pixel_count;
    struct meter_encoded_pixel pixels[];
};
static_assert(sizeof(struct meter_encoded_row) == 6);

struct meter {
    uint32_t flags;
    struct tag_reference stencil_bitmaps;
    struct tag_reference source_bitmap;
    int16_t stencil_sequence_index;
    int16_t source_sequence_index;
    uint32_t unused0[4];
    uint32_t color_interpolation_flags;
    uint16_t color_interpolation_mode;
    uint16_t color_anchor;
    uint32_t unused1[2];
    float_argb_color empty_color;
    float_argb_color full_color;
    uint32_t unused2[5];
    float mask_in_meter_distance;
    float mask_out_meter_distance;
    uint32_t unused3[3];
    point2d runtime_registration_point;
    int16_t runtime_width;
    int16_t runtime_height;
    struct tag_data encoded_stencil;
};
static_assert(sizeof(struct meter) == 172);

#pragma pack(pop)

bool meter_final_postprocess(TagID tag, struct tag_data_instance *tag_data);
