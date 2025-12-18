#include <stdint.h>
#include <assert.h>

#include "../data_types.h"
#include "../tag/tag_processing.h"

#include "hud_types.h"

void hud_process_absolute_placement(struct hud_absolute_placement *absolute_placement) {
    assert(absolute_placement);

    // Nothing supports this extension as of this time but might as well handle it for now
    tag_process_enum16(&absolute_placement->canvas_size, NUMBER_OF_HUD_CANVAS_SIZES, HUD_CANVAS_SIZE_480P);
}

void hud_process_meter_element(struct hud_meter_element *meter) {
    assert(meter);

    // Fix min_alpha
    tag_process_float(&meter->min_alpha);
    meter->min_alpha = PIN(meter->min_alpha, 0.0f, 1.0f);
}
