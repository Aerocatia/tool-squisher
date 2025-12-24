#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "hud_types.h"

enum {
    UNIT_HUD_AUXILIARY_PANEL_FLAGS_ONLY_SHOW_WHEN_ACTIVE_BIT,
    UNIT_HUD_AUXILIARY_PANEL_FLAGS_FLASH_ONCE_IF_ACTIVATED_WHEN_DISABLED_BIT,
    NUMBER_OF_UNIT_HUD_AUXILIARY_PANEL_FLAGS
};

enum {
    UNIT_HUD_AUXILIARY_OVERLAY_FLAGS_USE_TEAM_COLOR_BIT,
    NUMBER_OF_UNIT_HUD_AUXILIARY_OVERLAY_FLAGS
};

enum {
    UNIT_HUD_STATE_SHIELD_RECHARGING,
    UNIT_HUD_STATE_SHIELD_DAMAGE,
    UNIT_HUD_STATE_SHIELD_LOW,
    UNIT_HUD_STATE_SHIELD_EMPTY,
    UNIT_HUD_STATE_HEALTH_LOW,
    UNIT_HUD_STATE_HEALTH_EMPTY,
    UNIT_HUD_STATE_MINOR_DAMAGE,
    UNIT_HUD_STATE_MAJOR_DAMAGE,
    NUMBER_OF_UNIT_HUD_STATES
};

enum {
    UNIT_HUD_AUXILIARY_METER_INTEGRATED_LIGHT,
    NUMBER_OF_UNIT_HUD_AUXILIARY_METERS
};

enum {
    UNIT_HUD_AUXILIARY_OVERLAY_TYPE_TEAM,
    NUMBER_OF_UNIT_HUD_AUXILIARY_OVERLAY_TYPES
};

#pragma pack(push, 1)

struct uint_hud_shield_panel {
    struct hud_static_element background;
    struct hud_meter_element meter;
    Pixel32 overcharge_min_color;
    Pixel32 overcharge_max_color;
    Pixel32 overcharge_flash_color;
    Pixel32 overcharge_empty_color;
    uint32_t unused[4];
};

struct uint_hud_health_panel {
    struct hud_static_element background;
    struct hud_meter_element meter;
    Pixel32 mid_color;
    float max_cutoff;
    float min_cutoff;
    uint32_t unused[5];
};

struct uint_hud_auxiliary_meter_panel {
    struct hud_static_element background;
    struct hud_meter_element meter;
    float min_cutoff;
    uint32_t flags;
    uint32_t unused[6];
};

struct uint_hud_motion_sensor_panel {
    struct hud_static_element background;
    struct hud_static_element foreground;
    uint32_t unused[8];
};

struct uint_hud_auxiliary_panel {
    struct hud_absolute_placement absolute_placement;
    struct tag_reflexive auxiliary_overlays;
    uint32_t unused[4];
};
static_assert(sizeof(struct uint_hud_auxiliary_panel) == 64);

struct uint_hud_auxiliary_overlay_element {
    struct hud_static_element static_element;
    uint16_t type;
    uint16_t flags;
    uint32_t unused0[6];
};
static_assert(sizeof(struct uint_hud_auxiliary_overlay_element) == 132);

struct uint_hud_auxiliary_meter_element {
    uint16_t type;
    uint16_t pad;
    uint32_t unused0[4];
    struct uint_hud_auxiliary_meter_panel panel;
    uint32_t unused1[16];
};
static_assert(sizeof(struct uint_hud_auxiliary_meter_element) == 324);

struct unit_hud_interface {
    struct hud_absolute_placement absolute_placement;
    struct hud_static_element background;
    struct uint_hud_shield_panel shield_meter;
    struct uint_hud_health_panel health_meter;
    struct uint_hud_motion_sensor_panel motion_sensor;
    struct hud_placement blip_placement;
    struct uint_hud_auxiliary_panel auxiliary_panel;
    struct tag_reflexive warning_sounds;
    struct tag_reflexive auxiliary_meters;
    uint32_t unused[101];
};
static_assert(sizeof(struct unit_hud_interface) == 1388);

#pragma pack(pop)

#define unit_hud_get_auxiliary_meter_element(hud, index, data) tag_reflexive_get_element(&(hud)->auxiliary_meters, index, sizeof(struct uint_hud_auxiliary_meter_element), data)

bool unit_hud_interface_postprocess(TagID tag, struct tag_data_instance *tag_data);
