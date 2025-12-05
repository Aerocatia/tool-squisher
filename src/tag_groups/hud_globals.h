#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "hud_types.h"

enum {
    HUD_WAYPOINT_FLAGS_DONT_ROTATE_OFFSCREEN_BIT,
    NUMBER_OF_WAYPOINT_FLAGS,
};

enum {
    HUD_WAYPOINT_TYPE_ON_SCREEN,
    HUD_WAYPOINT_TYPE_OFF_SCREEN,
    HUD_WAYPOINT_TYPE_OCCLUDED,
    NUMBER_OF_HUD_WAYPOINT_TYPES
};

struct hud_waypoint {
    float top_offset;
    float bottom_offset;
    float left_offset;
    float right_offset;
    uint32_t unused0[8];
    struct tag_reference arrow_bitmap;
    struct tag_reflexive arrows;
    uint32_t unused1[20];
};
static_assert(sizeof(struct hud_waypoint) == 156);

struct hud_waypoint_arrow {
    String32 name;
    uint32_t unused0[2];
    Pixel32 color;
    float opacity;
    float fade;
    int16_t sequence_indices[NUMBER_OF_HUD_WAYPOINT_TYPES];
    uint16_t pad;
    uint32_t unused1[4];
    uint32_t flags;
    uint32_t unused2[6];
};
static_assert(sizeof(struct hud_waypoint_arrow) == 104);

struct hud_damage_indicators {
    int16_t top_offset;
    int16_t bottom_offset;
    int16_t left_offset;
    int16_t right_offset;
    uint32_t unused[8];
    struct tag_reference indicator_bitmap;
    int16_t sequence_index;
    int16_t multiplayer_sequence_index;
    Pixel32 color;
    uint32_t unused2[4];
};
static_assert(sizeof(struct hud_damage_indicators) == 80);

struct hud_defaults {
    struct tag_reference default_weapon_hud;
    float motion_sensor_range;
    float motion_sensor_velocity_sensitivity;
    float motion_sensor_scale;
    rectangle2d default_title_bounds;
    uint32_t unused[11];
};
static_assert(sizeof(struct hud_defaults) == 80);

struct hud_timer {
    struct hud_color color;
    struct hud_color time_up_color;
    uint32_t unused[10];
};
static_assert(sizeof(struct hud_timer) == 104);

struct hud_globals {
    struct hud_messaging_parameters messaging;
    struct hud_waypoint waypoint;
    struct hud_multiplayer_parameters multiplayer;
    struct hud_defaults defaults;
    struct hud_damage_indicators damage_indicators;
    struct hud_timer timer;
    struct tag_reference carnage_report_bitmap;
    int16_t loading_begin_index;
    int16_t loading_end_index;
    int16_t checkpoint_begin_index;
    int16_t checkpoint_end_index;
    struct tag_reference checkpoint_sound;
    uint32_t unused2[24];
};
static_assert(sizeof(struct hud_globals) == 1104);
