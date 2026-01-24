/**
 * Tool Squisher
 *
 * This software is licensed under version 3 of the GNU General Public License
 * as published by the Free Software Foundation in 2007. It is not licensed
 * under any other license, even including later or earlier versions of the GPL.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <getopt.h>
#include <assert.h>

#include "data_types.h"
#include "global_options.h"
#include "cache/cache.h"
#include "crc/crc.h"
#include "file/file.h"
#include "tag/tag.h"
#include "tag/tag_fourcc.h"
#include "tag_groups/tag_groups.h"
#include "version.h"

static void print_usage(const char *executable);
static bool postprocess_map(const char *path);
static bool postprocess_tag_data(struct cache_file_instance *cache_file);

int main(int argc, char **argv) {
    if(argc == 1) {
        fprintf(stderr, "No input given\nUse --%s for usage\n",
            global_option_long_names[GLOBAL_OPTION_ARG_HELP]);
        return EXIT_FAILURE;
    }

    static const char *short_options = ":hnrv";
    static struct option long_options[] = {
        {GLOBAL_OPTION_ARG_HELP_STRING,            no_argument, nullptr, 'h'},
        {GLOBAL_OPTION_ARG_NO_PRESERVE_CRC_STRING, no_argument, nullptr, 'n'},
        {GLOBAL_OPTION_ARG_RELAXED_STRING,         no_argument, nullptr, 'r'},
        {GLOBAL_OPTION_ARG_VERSION_STRING,         no_argument, nullptr, 'v'},
        {0, 0, 0, 0}
    };

    while(true) {
        int option_index = 0;
        int opt = getopt_long(argc, argv, short_options, long_options, &option_index);
        if(opt == NONE) {
            break;
        }

        switch(opt) {
            case 'h':
                print_usage(argv[0]);
                return EXIT_SUCCESS;
            case 'n':
                SET_FLAG(global_option_flags, GLOBAL_OPTON_FLAGS_NO_PRESERVE_CRC_BIT, true);
                break;
            case 'r':
                SET_FLAG(global_option_flags, GLOBAL_OPTON_FLAGS_RELAXED_BIT, true);
                break;
            case 'v':
                    printf("tool-squisher %s, by Aerocatia\n", TOOL_SQUISHER_VERSION);
                    return EXIT_SUCCESS;
                break;
            case '?':
                fprintf(stderr, "Unknown option: %s\nUse --%s for usage\n",
                    argv[optind - 1], global_option_long_names[GLOBAL_OPTION_ARG_HELP]);
                return 1;
            default:
                abort();
        }
    }

    bool success = false;
    for(int i = optind; i < argc; i++) {
        success = postprocess_map(argv[i]) || success;
    }

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void print_usage(const char *executable_path) {
    char *path_copy = strdup(executable_path);
    if(!path_copy) {
        abort();
    }
    char *executable = basename(path_copy);
    printf("Usage: %s [options] <map> [map [...]]\n", executable);
    printf("Options:\n");
    for(int i = 0; i < NUMBER_OF_GLOBAL_OPTION_ARGS; i++) {
        printf("  -%s, --%-16s %s\n",
            global_option_short_names[i], global_option_long_names[i], global_option_help[i]);
    }

    free(path_copy);
}

static bool postprocess_map(const char *path) {
    assert(path);

    // It's less annoying to just skip these
    if(file_path_is_resource_map(path)) {
        fprintf(stderr, "%s: Skipped (assuming it's a resource map)\n", path);
        return true;
    }

    static struct cache_file_instance cache_file = {};
    cache_file_load(path, &cache_file);
    if(!cache_file.valid) {
        fprintf(stderr, "%s: Not a valid cache file\n", path);
        return false;
    }

    // Hold this
    uint32_t original_checksum = cache_file.header->checksum;

    bool success = true;
    auto cache_build = cache_file_resolve_build(cache_file.header);
    switch(cache_build) {
        case CACHE_FILE_TRACKED_BUILD_TOOL_SQUISHER:
            fprintf(stderr, "%s: Has already been squished\n", path);
            goto exit;
        case CACHE_FILE_TRACKED_BUILD_0563:
        case CACHE_FILE_TRACKED_BUILD_0564:
        case CACHE_FILE_TRACKED_BUILD_0609:
        case CACHE_FILE_TRACKED_BUILD_0621:
            success = postprocess_tag_data(&cache_file);
            break;
        case CACHE_FILE_TRACKED_BUILD_UNTRACKED:
            fprintf(stderr, "%s: Unsupported build \"%s\"\n", path, cache_file.header->build_number);
            success = false;
            goto exit;
        default:
            abort();
    }

    if(!success) {
        fprintf(stderr, "%s: Could not process\n", path);
        goto exit;
    }

    success = cache_file_update_header(&cache_file, true);
    if(!success) {
        fprintf(stderr, "%s: Could not update cache header\n", path);
        goto exit;
    }

    // Change the crc back
    if(!TEST_FLAG(global_option_flags, GLOBAL_OPTON_FLAGS_NO_PRESERVE_CRC_BIT)) {
        cache_file_forge_checksum(original_checksum, &cache_file);
    }

    // Save
    success = cache_file_save(path, &cache_file);
    if(success) {
        printf("%s: Saved!\n", path);
    }

    exit:
    cache_file_unload(&cache_file);
    return success;
}

static bool postprocess_tag_data(struct cache_file_instance *cache_file) {
    assert(cache_file && cache_file->valid);
    cache_file->dirty = true;

    // Fix the BSPs
    if(!scenario_structure_bsp_postprocess_all_in_cache(cache_file)) {
        return false;
    }

    struct tag_data_instance *tag_data = &cache_file->tag_data;
    assert(tag_data->valid);

    // Go through tag array and fix tags
    for(size_t i = 0; i < tag_data->header->tag_count; i++) {
        struct tag_instance *tag = &tag_data->tags[i];

        // Process shaders
        if(tag->primary_group == TAG_FOURCC_SHADER ||
            tag->secondary_group == TAG_FOURCC_SHADER ||
            tag->tertiary_group == TAG_FOURCC_SHADER
        ) {
            if(!shader_postprocess(tag->tag_id, tag_data)) {
                return false;
            }
        }
        // Process units
        else if(tag->secondary_group == TAG_FOURCC_UNIT && tag->tertiary_group == TAG_FOURCC_OBJECT) {
            if(!uint_postprocess(tag->tag_id, tag_data)) {
                return false;
            }
        }

        // Process other base tags
        switch(tag->primary_group) {
            case TAG_FOURCC_ACTOR_VARIANT:
                if(!actor_variant_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
                break;
            case TAG_FOURCC_BITMAP:
                if(!bitmap_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
                break;
            case TAG_FOURCC_DECAL:
                if(!decal_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
                break;
            case TAG_FOURCC_LENS_FLARE:
                if(!lens_flare_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
                break;
            case TAG_FOURCC_METER:
                if(!meter_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
                break;
            case TAG_FOURCC_GBXMODEL:
                if(!gbxmodel_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
                break;
            case TAG_FOURCC_SCENARIO:
                if(!scenario_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
                break;
            case TAG_FOURCC_SHADER_MODEL:
                if(!shader_model_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
                break;
            case TAG_FOURCC_SOUND:
                if(!sound_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
                break;
            case TAG_FOURCC_GRENADE_HUD_INTERFACE:
                if(!grenade_hud_interface_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
                break;
            case TAG_FOURCC_HUD_GLOBALS:
                if(!hud_globals_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
                break;
            case TAG_FOURCC_UNIT_HUD_INTERFACE:
                if(!unit_hud_interface_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
                break;
            case TAG_FOURCC_WEAPON_HUD_INTERFACE:
                if(!weapon_hud_interface_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
        }
    }

    return true;
}
