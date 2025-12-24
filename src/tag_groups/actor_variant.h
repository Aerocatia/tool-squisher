#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "unit.h"

enum {
    ACTOR_VARIANT_FLAGS_CAN_SHOOT_WHILE_FLYING_BIT,
    ACTOR_VARIANT_FLAGS_COLOR_INTERPOLATE_HSV_BIT,
    ACTOR_VARIANT_FLAGS_HAS_UNLIMITED_GRENADES_BIT,
    ACTOR_VARIANT_FLAGS_MOVEMENT_SWITCHING_TRY_TO_STAY_WITH_FRIENDS_BIT,
    ACTOR_VARIANT_FLAGS_ACTIVE_CAMOUFLAGE_BIT,
    ACTOR_VARIANT_FLAGS_SUPER_ACTIVE_CAMOUFLAGE_BIT,
    ACTOR_VARIANT_FLAGS_CANNOT_USE_RANGED_WEAPONS_BIT,
    ACTOR_VARIANT_FLAGS_PREFER_PASSENGER_SEAT_BIT,
    NUMBER_OF_ACTOR_VARIANT_FLAGS
};

enum {
    ACTOR_VARIANT_GRENADE_TRAJECTORY_TOSS,
    ACTOR_VARIANT_GRENADE_TRAJECTORY_LOB,
    ACTOR_VARIANT_GRENADE_TRAJECTORY_BOUNCE,
    NUMBER_OF_ACTOR_VARIANT_GRENADE_TRAJECTORIES
};

enum {
    ACTOR_VARIANT_GRENADE_STIMULUS_NONE,
    ACTOR_VARIANT_GRENADE_STIMULUS_VISIBLE_TARGET,
    ACTOR_VARIANT_GRENADE_STIMULUS_SEEK_COVER,
    NUMBER_OF_ACTOR_VARIANT_GRENADE_STIMULI
};

enum {
    ACTOR_VARIANT_MOVEMENT_SWITCHING_TYPE_ALWAYS_RUN,
    ACTOR_VARIANT_MOVEMENT_SWITCHING_TYPE_ALWAYS_CROUCH,
    ACTOR_VARIANT_MOVEMENT_SWITCHING_TYPE_SWITCH_TYPES,
    NUMBER_OF_ACTOR_VARIANT_MOVEMENT_SWITCHING_TYPES
};

enum {
    ACTOR_VARIANT_SPECIAL_FIRE_MODE_NONE,
    ACTOR_VARIANT_SPECIAL_FIRE_MODE_OVERCHARGE,
    ACTOR_VARIANT_SPECIAL_FIRE_MODE_SECONDARY,
    NUMBER_OF_ACTOR_SPECIAL_FIRE_MODES
};

enum {
    ACTOR_VARIANT_SPECIAL_FIRE_SITUATION_NEVER,
    ACTOR_VARIANT_SPECIAL_FIRE_SITUATION_ENEMY_VISIBLE,
    ACTOR_VARIANT_SPECIAL_FIRE_SITUATION_ENEMY_OUT_OF_SIGHT,
    ACTOR_VARIANT_SPECIAL_FIRE_SITUATION_STRAFING,
    NUMBER_OF_ACTOR_VARIANT_SPECIAL_FIRE_SITUATIONS
};

#pragma pack(push, 1)

struct actor_variant_burst_geometry {
    float burst_origin_radius;
    float burst_origin_angle;
    float_bounds burst_return_length_bounds;
    float burst_return_angle;
    float_bounds burst_duration_bounds;
    float_bounds burst_separation_bounds;
    float burst_maximum_angular_velocity;
    uint32_t unused;
};
static_assert(sizeof(struct actor_variant_burst_geometry) == 44);

struct actor_variant_firing_pattern {
    float burst_duration_modifier;
    float burst_separation_modifier;
    float rate_of_fire_modifier;
    float error_angle_modifier;
    uint32_t unused[2];
};
static_assert(sizeof(struct actor_variant_firing_pattern) == 24);

struct actor_variant_ranged_combat_properties {
    struct tag_reference weapon_reference;
    float maximum_firing_range;
    float rate_of_fire;
    float projectile_error_angle;
    float_bounds first_burst_delay_bounds;
    float new_target_pattern_time;
    float surprise_delay_time;
    float surprise_wildfire_time;
    float death_wildfire_chance;
    float death_wildfire_time;
    float_bounds combat_range_bounds;
    float_vector3d gun_offset_stand;
    float_vector3d gun_offset_crouch;
    float target_track_fraction;
    float target_lead_fraction;
    float weapon_damage_modifier;
    float damage_per_second;
    struct actor_variant_burst_geometry burst_geometry;
    float special_damage_modifier;
    float special_projectile_error;
    struct actor_variant_firing_pattern new_target_pattern;
    struct actor_variant_firing_pattern moving_pattern;
    struct actor_variant_firing_pattern berserk_pattern;
    float weapon_super_ballistic_range;
    float weapon_bombardment_range;
    float modified_vision_range;
    uint16_t special_fire_mode;
    uint16_t special_fire_situation;
    float special_fire_chance;
    float special_fire_delay;
    float melee_range;
    float melee_abort_range;
    float_bounds berserk_firing_range_bounds;
    float berserk_melee_range;
    float berserk_melee_abort_range;
    uint32_t unused[2];
};
static_assert(sizeof(struct actor_variant_ranged_combat_properties) == 284);

struct actor_variant_grenade_combat_properties {
    uint16_t grenade_type;
    uint16_t trajectory_type;
    uint16_t stimulus_type;
    int16_t minimum_enemy_count;
    float enemy_radius;
    float grenade_unused;
    float grenade_maximum_velocity;
    float_bounds grenade_range_bounds;
    float collateral_damage_radius;
    float throw_grenade_chance;
    float throw_grenade_delay;
    float encounter_grenade_timeout;
    uint32_t unused[5];
};
static_assert(sizeof(struct actor_variant_grenade_combat_properties) == 64);

struct actor_variant_item_properties {
    struct tag_reference equipment_reference;
    int16_bounds grenades_bounds;
    float dont_drop_grenades_chance;
    float_bounds weapon_loaded_bounds;
    int16_bounds weapon_ammo_bounds;
    uint32_t unused[7];
};
static_assert(sizeof(struct actor_variant_grenade_combat_properties) == 64);

struct actor_variant_unit_properties {
    float maximum_body_vitality;
    float maximum_shield_vitality;
    float shield_sapping_radius;
    int16_t forced_shader_permutation_index;
    uint16_t pad;
    uint32_t unused[4];
};
static_assert(sizeof(struct actor_variant_unit_properties) == 32);

struct actor_variant_movement_switching_properties {
    uint16_t movement_type;
    uint16_t pad;
    float initial_crouch_chance;
    float_bounds crouch_time_bounds;
    float_bounds run_time_bounds;
};
static_assert(sizeof(struct actor_variant_movement_switching_properties) == 24);

struct actor_variant_change_colors {
    float_rgb_color_bounds color_bounds;
    uint32_t unused[2];
};
static_assert(sizeof(struct actor_variant_change_colors) == 32);

struct actor_variant {
    uint32_t flags;
    struct tag_reference actor_reference;
    struct tag_reference unit_reference;
    struct tag_reference major_upgrade_reference;
    struct unit_metagame_properties metagame_properties;
    uint32_t unused[5];
    struct actor_variant_movement_switching_properties movement_switching;
    struct actor_variant_ranged_combat_properties ranged_combat;
    struct actor_variant_grenade_combat_properties grenade_combat;
    struct actor_variant_item_properties items;
    struct actor_variant_unit_properties unit;
    uint32_t unused2[3];
    struct tag_reflexive change_colors;
};
static_assert(sizeof(struct actor_variant) == 568);

#pragma pack(pop)

#define actor_variant_get_change_colors(variant, index, data) tag_reflexive_get_element(&(variant)->change_colors, index, sizeof(struct actor_variant_change_colors), data)

bool actor_variant_postprocess(TagID tag, struct tag_data_instance *tag_data);
