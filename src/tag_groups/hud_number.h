#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"

struct hud_number {
    struct tag_reference number_bitmap;
    int8_t character_width;
    int8_t screen_width;
    int8_t x_offset;
    int8_t y_offset;
    int8_t decimal_point_width;
    int8_t colon_width;
    uint16_t pad;
    uint32_t unused[19];
};
static_assert(sizeof(struct hud_number) == 100);
