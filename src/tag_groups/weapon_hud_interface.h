#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "hud_types.h"

enum {
    WEAPON_HUD_OVERLAY_TYPE_FLAGS_ON_FLASHING_BIT,
    WEAPON_HUD_OVERLAY_TYPE_FLAGS_ON_EMPTY_BIT,
    WEAPON_HUD_OVERLAY_TYPE_FLAGS_ON_RELOAD_BIT,
    WEAPON_HUD_OVERLAY_TYPE_FLAGS_ON_DEFAULT_BIT,
    WEAPON_HUD_OVERLAY_TYPE_FLAGS_ON_ALWAYS_BIT,
    NUMBER_OF_WEAPON_HUD_OVERLAY_TYPE_FLAGS
};

enum {
    WEAPON_HUD_OVERLAY_FLAGS_FLASHES_BIT,
    NUMBER_OF_WEAPON_HUD_OVERLAY_FLAGS
};
static_assert(NUMBER_OF_WEAPON_HUD_OVERLAY_FLAGS == 1);

enum {
    WEAPON_HUD_OVERLAY_RUNTIME_FLAGS_INVALID_BIT = NUMBER_OF_WEAPON_HUD_OVERLAY_FLAGS,
    NUMBER_OF_WEAPON_HUD_OVERLAY_RUNTIME_FLAGS
};

enum {
    WEAPON_HUD_CROSSHAIR_FLAGS_FLASHES_BIT,
    WEAPON_HUD_CROSSHAIR_FLAGS_NOT_A_SPRITE_BIT,
    WEAPON_HUD_CROSSHAIR_FLAGS_NOT_ON_DEFAULT_ZOOM_BIT,
    WEAPON_HUD_CROSSHAIR_FLAGS_SHOW_SNIPER_DATA_BIT,
    WEAPON_HUD_CROSSHAIR_FLAGS_HIDE_OUTSIDE_AREA_BIT,
    WEAPON_HUD_CROSSHAIR_FLAGS_ONE_ZOOM_LEVEL_BIT,
    WEAPON_HUD_CROSSHAIR_FLAGS_ONLY_ON_DEFAULT_ZOOM_BIT,
    NUMBER_OF_WEAPON_HUD_CROSSHAIR_FLAGS
};
static_assert(NUMBER_OF_WEAPON_HUD_CROSSHAIR_FLAGS == 7);

enum {
    WEAPON_HUD_CROSSHAIR_RUNTIME_FLAGS_INVALID_BIT = NUMBER_OF_WEAPON_HUD_CROSSHAIR_FLAGS,
    NUMBER_OF_WEAPON_HUD_CROSSHAIR_RUNTIME_FLAGS
};

enum {
    WEAPON_HUD_ELEMENT_RUNTIME_FLAGS_INVALID_BIT,
    NUMBER_OF_WEAPON_HUD_RUNTIME_FLAGS
};

enum {
    WEAPON_HUD_FLASHING_FLAGS_USE_PARENT_FLASH_PARAMETERS_BIT,
    NUMBER_OF_WEAPON_HUD_FLASHING_FLAGS,
};

enum {
    WEAPON_NUMBER_DIVIDE_BY_CLIP_SIZE_BIT,
    NUMBER_OF_WEAPON_NUMBER_FLAGS,
};

enum {
    WEAPON_HUD_STATE_TOTAL_AMMO,
    WEAPON_HUD_STATE_LOADED_AMMO,
    WEAPON_HUD_STATE_HEAT,
    WEAPON_HUD_STATE_AGE,
    WEAPON_HUD_STATE_SECONDARY_TOTAL_AMMO,
    WEAPON_HUD_STATE_SECONDARY_LOADED_AMMO,
    WEAPON_HUD_STATE_DISTANCE_TO_TARGET,
    WEAPON_HUD_STATE_ELEVATION_TO_TARGET,
    NUMBER_OF_WEAPON_HUD_STATES
};

enum {
    WEAPON_HUD_CROSSHAIR_STATE_AIM,
    WEAPON_HUD_CROSSHAIR_STATE_ZOOM,
    WEAPON_HUD_CROSSHAIR_STATE_CHARGE,
    WEAPON_HUD_CROSSHAIR_STATE_FLASH_AMMO,
    WEAPON_HUD_CROSSHAIR_STATE_FLASH_HEAT,
    WEAPON_HUD_CROSSHAIR_STATE_FLASH_TOTAL_AMMO,
    WEAPON_HUD_CROSSHAIR_STATE_FLASH_TOTAL_BATTERY,
    WEAPON_HUD_CROSSHAIR_STATE_RELOAD,
    WEAPON_HUD_CROSSHAIR_STATE_FIRED_WITH_NO_AMMO,
    WEAPON_HUD_CROSSHAIR_STATE_THREW_WITH_NO_GRENADE,
    WEAPON_HUD_CROSSHAIR_STATE_FLASH_AMMO_NONE_FOR_RELOAD,
    WEAPON_HUD_CROSSHAIR_STATE_FLASH_SECONDARY_AMMO,
    WEAPON_HUD_CROSSHAIR_STATE_FLASH_SECONDARY_TOTAL_AMMO,
    WEAPON_HUD_CROSSHAIR_STATE_SECONDARY_RELOAD,
    WEAPON_HUD_CROSSHAIR_STATE_FIRED_SECONDARY_WITH_NO_AMMO,
    WEAPON_HUD_CROSSHAIR_STATE_FLASH_SECONDARY_AMMO_NONE_FOR_RELOAD,
    WEAPON_HUD_CROSSHAIR_STATE_PRIMARY_TRIGGER_READY,
    WEAPON_HUD_CROSSHAIR_STATE_SECONDARY_TRIGGER_READY,
    WEAPON_HUD_CROSSHAIR_STATE_FLASH_FIRED_BATTERY_DEPLETED,
    NUMBER_OF_WEAPON_HUD_CROSSHAIR_STATES,
};

#pragma pack(push, 1)

struct weapon_hud_element_header {
    uint16_t state_type;
    uint16_t runtime_flags;
    uint16_t use_on_map_type;
    uint16_t child_anchor;
    uint32_t unused[7];
};
static_assert(sizeof(struct weapon_hud_element_header) == 36);

struct weapon_hud_overlay {
    struct tag_reference bitmap;
    struct tag_reflexive items;
};
static_assert(sizeof(struct weapon_hud_overlay) == 28);

struct weapon_hud_overlay_item {
    struct hud_placement placement;
    struct hud_color colors;
    int16_t frame_rate;
    uint16_t pad;
    int16_t sequence_index;
    uint16_t type;
    uint32_t flags;
    uint32_t unused[14];
};
static_assert(sizeof(struct weapon_hud_overlay_item) == 136);

struct weapon_hud_flash_state {
    uint16_t flags;
    uint16_t pad;
    int16_t total_ammo;
    int16_t loaded_ammo;
    int16_t heat;
    int16_t age;
    uint32_t unused[8];
};
static_assert(sizeof(struct weapon_hud_flash_state) == 44);

struct weapon_hud_static_element {
    struct weapon_hud_element_header header;
    struct hud_static_element static_element;
    uint32_t unused[10];
};
static_assert(sizeof(struct weapon_hud_static_element) == 180);

struct weapon_hud_meter_element {
    struct weapon_hud_element_header header;
    struct hud_meter_element meter_element;
    uint32_t unused[10];
};
static_assert(sizeof(struct weapon_hud_meter_element) == 180);

struct weapon_hud_number_element {
    struct weapon_hud_element_header header;
    struct hud_number_element number_element;
    uint16_t weapon_flags;
    uint16_t pad;
    uint32_t unused[9];
};
static_assert(sizeof(struct weapon_hud_number_element) == 160);

struct weapon_hud_overlays_element {
    struct weapon_hud_element_header header;
    struct weapon_hud_overlay overlays;
    uint32_t unused2[10];
};
static_assert(sizeof(struct weapon_hud_overlays_element) == 104);

struct weapon_hud_crosshair_item {
    struct hud_placement placement;
    struct hud_color colors;
    int16_t frame_rate;
    int16_t sequence_index;
    uint32_t flags;
    uint32_t unused[8];
};
static_assert(sizeof(struct weapon_hud_crosshair_item) == 108);

struct weapon_hud_crosshair {
    struct tag_reference bitmap;
    struct tag_reflexive items;
};
static_assert(sizeof(struct weapon_hud_crosshair) == 28);

struct weapon_hud_crosshairs_element {
    uint16_t crosshair_type;
    uint16_t runtime_flags;
    uint16_t use_on_map_type;
    uint16_t pad;
    uint32_t unused[7];
    struct weapon_hud_crosshair crosshairs;
    uint32_t unused2[10];
};
static_assert(sizeof(struct weapon_hud_crosshairs_element) == 104);

struct weapon_hud_interface {
    struct tag_reference parent_hud;
    struct weapon_hud_flash_state flash_cutoffs;
    struct hud_absolute_placement absolute_placement;
    struct tag_reflexive statics;
    struct tag_reflexive meters;
    struct tag_reflexive numbers;
    struct tag_reflexive crosshairs;
    struct tag_reflexive overlays;
    uint32_t valid_crosshair_types_flags;
    struct tag_reflexive warning_sounds;
    struct tag_reflexive screen_effects;
    uint32_t unused1[33];
    struct hud_icon_element messaging_icon;
    uint32_t unused2[12];
};
static_assert(sizeof(struct weapon_hud_interface) == 380);

#pragma pack(pop)

#define weapon_hud_get_static_element(hud, index, data) tag_reflexive_get_element(&(hud)->statics, index, sizeof(struct weapon_hud_static_element), data)
#define weapon_hud_get_meter_element(hud, index, data) tag_reflexive_get_element(&(hud)->meters, index, sizeof(struct weapon_hud_meter_element), data)
#define weapon_hud_get_number_element(hud, index, data) tag_reflexive_get_element(&(hud)->numbers, index, sizeof(struct weapon_hud_number_element), data)
#define weapon_hud_get_crosshairs_element(hud, index, data) tag_reflexive_get_element(&(hud)->crosshairs, index, sizeof(struct weapon_hud_crosshairs_element), data)
#define weapon_hud_get_crosshairs_item(crosshair, index, data) tag_reflexive_get_element(&(crosshair)->crosshairs.items, index, sizeof(struct weapon_hud_crosshair_item), data)
#define weapon_hud_get_overlays_element(hud, index, data) tag_reflexive_get_element(&(hud)->overlays, index, sizeof(struct weapon_hud_overlays_element), data)

bool weapon_hud_interface_postprocess(TagID tag, struct tag_data_instance *tag_data);
