#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "object.h"

enum {
    UNIT_SEAT_FLAGS_IS_INVISIBLE_BIT,
    UNIT_SEAT_FLAGS_IS_LOCKED_BIT,
    UNIT_SEAT_FLAGS_IS_DRIVER_BIT,
    UNIT_SEAT_FLAGS_IS_GUNNER_BIT,
    UNIT_SEAT_FLAGS_HAS_THIRD_PERSON_CAMERA_BIT,
    UNIT_SEAT_FLAGS_ALLOWS_WEAPONS_BIT,
    UNIT_SEAT_FLAGS_THIRD_PERSON_ON_ENTER_BIT,
    UNIT_SEAT_FLAGS_SLAVE_FIRST_PERSON_CAMERA_BIT,
    UNIT_SEAT_FLAGS_ALLOW_VEHICLE_COMMUNICATION_ANIMATIONS_BIT,
    UNIT_SEAT_FLAGS_NOT_VALID_WITHOUT_DRIVER_BIT,
    UNIT_SEAT_FLAGS_ALLOW_NONCOMBATANTS_BIT,
    NUMBER_OF_UNIT_SEAT_FLAGS
};

enum {
    UNIT_FLAGS_HAS_CIRCULAR_AIMING_ANGLES_BIT,
    UNIT_FLAGS_IS_DESTROYED_AFTER_DYING_BIT,
    UNIT_FLAGS_HAS_SLOW_ANIMATION_INTERPOLATION_BIT,
    UNIT_FLAGS_FIRES_FROM_CAMERA_BIT,
    UNIT_FLAGS_HAS_ENTRANCE_POINTS_INSIDE_BOUNDING_SPHERE_BIT,
    UNIT_FLAGS_UNUSED_BIT,
    UNIT_FLAGS_CAUSES_PASSENGER_DIALOGUE_BIT,
    UNIT_FLAGS_RESISTS_PINGS_BIT,
    UNIT_FLAGS_MELEE_ATTACK_IS_FATAL_BIT,
    UNIT_FLAGS_DOES_NOT_REORIENT_DURING_PINGS_BIT,
    UNIT_FLAGS_HAS_NO_AIMING_BIT,
    UNIT_FLAGS_SIMPLE_CREATURE_BIT,
    UNIT_FLAGS_IMPACT_MELEE_ATTACHES_BIT,
    UNIT_FLAGS_IMPACT_MELEE_DIE_ON_SHIELD_BIT,
    UNIT_FLAGS_CANNOT_OPEN_DOORS_AUTOMATICALLY_BIT,
    UNIT_FLAGS_MELEE_ATTACKERS_CANNOT_ATTACH_BIT,
    UNIT_FLAGS_NEVER_UNSUSPECTING_BIT,
    UNIT_FLAGS_SHIELD_SAPPING_BIT,
    UNIT_FLAGS_RUNS_AROUND_FLAMING_BIT,
    UNIT_FLAGS_IS_INCONSEQUENTIAL_BIT,
    UNIT_FLAGS_IS_SPECIAL_BIT,
    UNIT_FLAGS_IGNORED_BY_AUTOAIMING_BIT,
    UNIT_FLAGS_SHIELDS_FRY_INFECTION_FORMS_BIT,
    UNIT_FLAGS_INTEGRATED_LIGHT_CONTROLS_WEAPON_DIRECTLY_BIT,
    UNIT_FLAGS_INTEGRATED_LIGHT_LASTS_FOREVER_BIT,
    NUMBER_OF_UNIT_FLAGS
};

enum {
    UNIT_DEFAULT_TEAM_NONE,
    UNIT_DEFAULT_TEAM_PLAYER,
    UNIT_DEFAULT_TEAM_HUMAN,
    UNIT_DEFAULT_TEAM_COVENANT,
    UNIT_DEFAULT_TEAM_FLOOD,
    UNIT_DEFAULT_TEAM_SENTINEL,
    UNIT_DEFAULT_TEAM_UNUSED6,
    UNIT_DEFAULT_TEAM_UNUSED7,
    UNIT_DEFAULT_TEAM_UNUSED8,
    UNIT_DEFAULT_TEAM_UNUSED9,
    NUMBER_OF_UNIT_DEFAULT_TEAMS
};

enum {
    UNIT_CAMERA_TRACK_LOOSE,
    UNIT_CAMERA_TRACK_TIGHT,
    NUMBER_OF_UNIT_CAMERA_TRACKS
};

enum {
    UNIT_POWERED_SEAT_DRIVER,
    UNIT_POWERED_SEAT_GUNNER,
    NUMBER_OF_UNIT_POWERED_SEATS
};

enum {
    UNIT_HUD_TYPE_DEFAULT,
    UNIT_HUD_TYPE_MULTIPLAYER,
    NUMBER_OF_UNIT_HUD_TYPES
};

enum {
    UNIT_FUNCTION_MODE_NONE,
    UNIT_FUNCTION_MODE_DRIVER_SEAT_POWER,
    UNIT_FUNCTION_MODE_GUNNER_SEAT_POWER,
    UNIT_FUNCTION_MODE_AIMING_CHANGE,
    UNIT_FUNCTION_MODE_MOUTH_APERTURE,
    UNIT_FUNCTION_MODE_INTEGRATED_LIGHT_POWER,
    UNIT_FUNCTION_MODE_CAN_BLINK,
    UNIT_FUNCTION_MODE_SHIELD_SAPPING,
    NUMBER_OF_UNIT_FUNCTION_MODES
};

enum {
    UNIT_METAGAME_TYPE_BRUTE,
    UNIT_METAGAME_TYPE_GRUNT,
    UNIT_METAGAME_TYPE_JACKAL,
    UNIT_METAGAME_TYPE_SKIRMISHER,
    UNIT_METAGAME_TYPE_MARINE,
    UNIT_METAGAME_TYPE_SPARTAN,
    UNIT_METAGAME_TYPE_BUGGER,
    UNIT_METAGAME_TYPE_HUNTER,
    UNIT_METAGAME_TYPE_FLOOD_INFECTION,
    UNIT_METAGAME_TYPE_FLOOD_CARRIER,
    UNIT_METAGAME_TYPE_FLOOD_COMBAT,
    UNIT_METAGAME_TYPE_FLOOD_PURE,
    UNIT_METAGAME_TYPE_SENTINEL,
    UNIT_METAGAME_TYPE_ELITE,
    UNIT_METAGAME_TYPE_ENGINEER,
    UNIT_METAGAME_TYPE_MULE,
    UNIT_METAGAME_TYPE_TURRET,
    UNIT_METAGAME_TYPE_MONGOOSE,
    UNIT_METAGAME_TYPE_WARTHOG,
    UNIT_METAGAME_TYPE_SCORPION,
    UNIT_METAGAME_TYPE_HORNET,
    UNIT_METAGAME_TYPE_PELICAN,
    UNIT_METAGAME_TYPE_REVENANT,
    UNIT_METAGAME_TYPE_SERAPH,
    UNIT_METAGAME_TYPE_SHADE,
    UNIT_METAGAME_TYPE_WATCHTOWER,
    UNIT_METAGAME_TYPE_GHOST,
    UNIT_METAGAME_TYPE_CHOPPER,
    UNIT_METAGAME_TYPE_MAULER,
    UNIT_METAGAME_TYPE_WRAITH,
    UNIT_METAGAME_TYPE_BANSHEE,
    UNIT_METAGAME_TYPE_PHANTOM,
    UNIT_METAGAME_TYPE_SCARAB,
    UNIT_METAGAME_TYPE_GUNTOWER,
    UNIT_METAGAME_TYPE_TUNING_FORK,
    UNIT_METAGAME_TYPE_BROADSWORD,
    UNIT_METAGAME_TYPE_MAMMOTH,
    UNIT_METAGAME_TYPE_LICH,
    UNIT_METAGAME_TYPE_MANTIS,
    UNIT_METAGAME_TYPE_WASP,
    UNIT_METAGAME_TYPE_PHAETON,
    UNIT_METAGAME_TYPE_BISHOP,
    UNIT_METAGAME_TYPE_KNIGHT,
    UNIT_METAGAME_TYPE_PAWN,
    NUMBER_OF_UNIT_METAGAME_TYPES
};

enum {
    UNIT_METAGAME_CLASS_INFANTRY,
    UNIT_METAGAME_CLASS_LEADER,
    UNIT_METAGAME_CLASS_HERO,
    UNIT_METAGAME_CLASS_SPECIALIST,
    UNIT_METAGAME_CLASS_LIGHT_VEHICLE,
    UNIT_METAGAME_CLASS_HEAVY_VEHICLE,
    UNIT_METAGAME_CLASS_GIANT_VEHICLE,
    UNIT_METAGAME_CLASS_STANDARD_VEHICLE,
    NUMBER_OF_UNIT_METAGAME_CLASSES
};

enum {
    UNIT_GRENADE_TYPE_HUMAN_FRAGMENTATION,
    UNIT_GRENADE_TYPE_COVENANT_PLASMA,
    UNIT_GRENADE_TYPE_2,
    UNIT_GRENADE_TYPE_3,
    NUMBER_OF_UNIT_GRENADE_TYPES
};

#pragma pack(push, 1)

struct unit_camera {
    String32 marker_name;
    String32 submerged_marker_name;
    float pitch_autolevel;
    float pitch_minimum;
    float pitch_maximum;
    struct tag_reflexive unit_camera_tracks;
};
static_assert(sizeof(struct unit_camera) == 88);

struct unit_camera_track {
    struct tag_reference track;
    uint32_t unused[3];
};
static_assert(sizeof(struct unit_camera_track) == 28);

struct unit_seat {
    uint32_t flags;
    String32 label;
    String32 marker_name;
    uint32_t unused[8];
    float_vector3d acceleration_scale;
    uint32_t unused2[3];
    float yaw_rate;
    float pitch_rate;
    struct unit_camera camera;
    struct tag_reflexive seat_huds;
    uint32_t pad0;
    int16_t icon_text_index;
    uint16_t pad;
    float yaw_minimum;
    float yaw_maximum;
    struct tag_reference built_in_actor_reference;
    uint32_t unused3[5];
};
static_assert(sizeof(struct unit_seat) == 284);

struct unit_initial_weapon {
    struct tag_reference reference;
    uint32_t unused[5];
};
static_assert(sizeof(struct unit_initial_weapon) == 36);

struct unit_powered_seat {
    uint32_t pad;
    float powerup_time;
    float powerdown_time;
    uint32_t unused[14];
};
static_assert(sizeof(struct unit_powered_seat) == 68);

struct unit_dialogue_variant {
    int16_t variant_number;
    uint16_t pad;
    uint32_t unused;
    struct tag_reference dialogue_variant;
};
static_assert(sizeof(struct unit_dialogue_variant) == 24);

struct unit_hud_reference {
    struct tag_reference hud;
    uint32_t unused[8];
};
static_assert(sizeof(struct unit_hud_reference) == 48);

struct unit_metagame_properties {
    uint16_t metagame_type;
    uint16_t metagame_class;
};
static_assert(sizeof(struct unit_metagame_properties) == 4);

struct _unit {
    uint32_t flags;
    uint16_t default_team;
    uint16_t constant_sound;
    float child_damage_fraction;
    struct tag_reference integrated_light_toggle_effect;
    uint16_t function_mode_a_in;
    uint16_t function_mode_b_in;
    uint16_t function_mode_c_in;
    uint16_t function_mode_d_in;
    float camera_field_of_view;
    float camera_impulse_stiffness;
    struct unit_camera camera;
    float_vector3d acceleration_scale;
    float integrated_light_poweron_time;
    float integrated_light_poweroff_time;
    float integrated_light_lifespan;
    float soft_ping_threshold;
    float soft_ping_minimum_interrupt_time;
    float hard_ping_threshold;
    float hard_ping_minimum_interrupt_time;
    float hard_death_threshold;
    float feign_death_threshold;
    float feign_death_time;
    float evade_distance;
    float dive_distance;
    uint32_t movement_unused;
    float stunned_movement_threshold;
    float feign_death_chance;
    float feign_death_repeat_chance;
    struct tag_reference spawned_actor_variant;
    int16_bounds spawn_actor_count_bounds;
    float spawn_throw_velocity;
    float aiming_velocity_maximum;
    float aiming_acceleration_maximum;
    float casual_aiming_modifier;
    float looking_velocity_maximum;
    float looking_acceleration_maximum;
    uint32_t unused4[2];
    float ai_vehicle_avoidance_radius;
    float ai_danger_radius;
    struct tag_reference melee_damage;
    uint16_t blip_type;
    uint16_t pad;
    struct unit_metagame_properties metagame_properties;
    uint32_t unused[2];
    struct tag_reflexive huds;
    struct tag_reflexive dialogue_variants;
    float grenade_velocity;
    uint16_t grenade_type;
    int16_t grenade_count;
    int16_t runtime_soft_ping_minimum_interrupt_ticks;
    int16_t runtime_hard_ping_minimum_interrupt_ticks;
    struct tag_reflexive powered_seats;
    struct tag_reflexive initial_weapons;
    struct tag_reflexive seats;
};
static_assert(sizeof(struct _unit) == 372);

struct unit {
    struct _object object;
    struct _unit unit;
};
static_assert(sizeof(struct unit) == 752);

#pragma pack(pop)

void unit_process_metagame_properties(struct unit_metagame_properties *metagame_properties);
bool uint_postprocess(TagID tag, struct tag_data_instance *tag_data);
