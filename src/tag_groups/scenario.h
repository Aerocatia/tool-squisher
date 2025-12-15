#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"

enum {
    SCENARIO_FLAGS_CORTANA_HACK_BIT,
    SCENARIO_FLAGS_DEMO_UI_BIT,
    SCENARIO_FLAGS_NTSC_TO_SRGB_COLOR_CORRECTION_BIT,
    SCENARIO_FLAGS_DO_NOT_APPLY_BUNGIE_CAMPAIGN_TAG_PATCHES_BIT,
    NUMBER_OF_SCENARIO_FLAGS
};

enum {
    SCENARIO_TYPE_SOLO,
    SCENARIO_TYPE_MULTIPLAYER,
    SCENARIO_TYPE_MAIN_MENU,
    NUMBER_OF_SCENARIO_TYPES
};

#pragma pack(push, 1)

struct scenario_structure_bsp_reference {
    uint32_t offset;
    uint32_t size;
    Pointer32 address;
    uint32_t unused;
    struct tag_reference structure_bsp;
};
static_assert(sizeof(struct scenario_structure_bsp_reference) == 32);

struct scenario {
    struct tag_reference unused_structure_bsp;
    struct tag_reference unused_globals;
    struct tag_reference unused_sky;
    struct tag_reflexive sky_references;
    uint16_t type;
    uint16_t flags;
    struct tag_reflexive scenario_references;
    float local_north;
    uint32_t unused1[39];
    struct tag_reflexive predicted_ui_resources;
    struct tag_reflexive functions;
    struct tag_data editor_scenario_data;
    struct tag_reflexive comments;
    struct tag_reflexive scavenger_hunt_objects;
    uint32_t unused2[53];
    struct tag_reflexive object_names;
    struct tag_reflexive scenery;
    struct tag_reflexive scenery_palette;
    struct tag_reflexive bipeds;
    struct tag_reflexive biped_palette;
    struct tag_reflexive vehicles;
    struct tag_reflexive vehicle_palette;
    struct tag_reflexive equipment;
    struct tag_reflexive equipment_palette;
    struct tag_reflexive weapons;
    struct tag_reflexive weapon_palette;
    struct tag_reflexive device_groups;
    struct tag_reflexive machines;
    struct tag_reflexive machine_palette;
    struct tag_reflexive controls;
    struct tag_reflexive control_palette;
    struct tag_reflexive light_fixtures;
    struct tag_reflexive light_fixtures_palette;
    struct tag_reflexive sound_scenery;
    struct tag_reflexive sound_scenery_palette;
    struct tag_reflexive unused_blocks[7];
    struct tag_reflexive starting_profiles;
    struct tag_reflexive players;
    struct tag_reflexive trigger_volumes;
    struct tag_reflexive recorded_animations;
    struct tag_reflexive netgame_flags;
    struct tag_reflexive netgame_equipment;
    struct tag_reflexive scenario_starting_equipment;
    struct tag_reflexive bsp_switch_trigger_volumes;
    struct tag_reflexive decals;
    struct tag_reflexive decal_palette;
    struct tag_reflexive detail_object_collection_palette;
    uint32_t unused3[21];
    struct tag_reflexive ai_actor_palette;
    struct tag_reflexive ai_encounters;
    struct tag_reflexive ai_command_lists;
    struct tag_reflexive ai_animation_references;
    struct tag_reflexive ai_script_references;
    struct tag_reflexive ai_recording_references;
    struct tag_reflexive ai_conversations;
    struct tag_data hs_syntax_data;
    struct tag_data hs_string_constants;
    struct tag_reflexive hs_scripts;
    struct tag_reflexive hs_globals;
    struct tag_reflexive hs_references;
    struct tag_reflexive hs_source_files;
    uint32_t unused4[6];
    struct tag_reflexive cutscene_flags;
    struct tag_reflexive cutscene_camera_points;
    struct tag_reflexive cutscene_chapter_titles;
    uint32_t unused5[27];
    struct tag_reference custom_object_names;
    struct tag_reference ingame_help_text;
    struct tag_reference hud_messages;
    struct tag_reflexive structure_bsp_references;
};
static_assert(sizeof(struct scenario) == 1456);

#pragma pack(pop)

#define scenario_get_bsp_reference(scenario, index, data) tag_reflexive_get_element(&(scenario)->structure_bsp_references, index, sizeof(struct scenario_structure_bsp_reference), data)

bool scenario_final_postprocess(TagID tag, struct tag_data_instance *tag_data);
