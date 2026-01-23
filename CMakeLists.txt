cmake_minimum_required(VERSION 4.0)

project(ToolSquisher
    VERSION 1.5.2
    DESCRIPTION "Tool to fix Halo CE map errors caused by tool.exe bugs"
    LANGUAGES C
)

add_executable(tool-squisher
    src/cache/cache.c
    src/crc/crc.c
    src/crc/crc_forcer.c
    src/file/file.c
    src/resources/resources.c
    src/tag/tag.c
    src/tag/tag_fourcc.c
    src/tag/tag_processing.c
    src/tag_groups/actor_variant.c
    src/tag_groups/bitmap.c
    src/tag_groups/decal.c
    src/tag_groups/decal_extent.c
    src/tag_groups/grenade_hud_interface.c
    src/tag_groups/hud_globals.c
    src/tag_groups/hud_types.c
    src/tag_groups/lens_flare.c
    src/tag_groups/meter.c
    src/tag_groups/model.c
    src/tag_groups/scenario.c
    src/tag_groups/scenario_structure_bsp.c
    src/tag_groups/shader.c
    src/tag_groups/shader_model.c
    src/tag_groups/sound.c
    src/tag_groups/unit.c
    src/tag_groups/unit_hud_interface.c
    src/tag_groups/weapon_hud_interface.c
    src/main.c
)

target_compile_options(tool-squisher PRIVATE -Wall -Wextra -Werror)

if(WIN32)
    target_sources(tool-squisher PRIVATE src/windows.rc)
endif()

option(TOOL_SQUISHER_TRUST_ME_BRO "Relax cache-file checks for fixing questionable maps" OFF)
if(${TOOL_SQUISHER_TRUST_ME_BRO})
    target_compile_definitions(tool-squisher PUBLIC TOOL_SQUISHER_TRUST_ME_BRO)
endif()
