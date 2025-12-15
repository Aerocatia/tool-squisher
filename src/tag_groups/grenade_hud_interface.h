#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"

#include "hud_types.h"
#include "weapon_hud_interface.h"

enum {
    GRENADE_HUD_OVERLAY_FLAGS_ON_FLASHING_BIT,
    GRENADE_HUD_OVERLAY_FLAGS_ON_EMPTY_BIT,
    GRENADE_HUD_OVERLAY_FLAGS_ON_DEFAULT_BIT,
    GRENADE_HUD_OVERLAY_FLAGS_ON_ALWAYS_BIT,
    NUMBER_OF_GRENADE_HUD_OVERLAY_FLAGS
};

enum {
    GRENADE_HUD_SOUND_STATE_LOW,
    GRENADE_HUD_SOUND_STATE_NONE,
    GRENADE_HUD_SOUND_STATE_THREW_WITH_NO_GRENADE,
    NUMBER_OF_GRENADE_HUD_SOUND_STATES,
};

struct grenade_count_panel {
    struct hud_static_element background;
    struct hud_number_element numbers;
    int16_t flash_cutoff;
    uint16_t pad;
};
static_assert(sizeof(struct grenade_count_panel) == 192);

struct grenade_hud_interface {
    struct hud_absolute_placement absolute_placement;
    struct hud_static_element background;
    struct grenade_count_panel grenade_count_panel;
    struct weapon_hud_overlay overlays;
    struct tag_reflexive warning_sounds;
    uint32_t unused0[17];
    struct tag_reference messaging_icon_bitmap;
    uint32_t unused1[12];
};
static_assert(sizeof(struct grenade_hud_interface) == 504);

bool grenade_hud_interface_final_postprocess(TagID tag, struct tag_data_instance *tag_data);
