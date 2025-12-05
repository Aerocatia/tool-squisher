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
#include <memory.h>
#include <libgen.h>

#include "cache/cache.h"
#include "crc/crc.h"
#include "file/file.h"
#include "tag/tag.h"
#include "tag/tag_fourcc.h"
#include "tag_groups/tag_groups.h"

static bool postprocess_map(const char *path);
int main(int argc, const char **argv) {
    char *executable_path = strdup(argv[0]);
    if(!executable_path) {
        return 1;
    }
    char *executable = basename(executable_path);
    if(argc == 1) {
        fprintf(stderr, "Usage: %s <map> [map [...]]\n", executable);
        return 1;
    }

    free(executable_path);

    bool success = false;
    for(int i = 1; i < argc; i++) {
        success = postprocess_map(argv[i]) || success;
    }
    return success ? 0 : 1;
}

static bool postprocess_tag_data(struct tag_data_instance *tag_data);
static bool postprocess_map(const char *path) {
    // It's less annoying to just skip these
    if(file_path_is_resource_map(path)) {
        printf("%s: Skipped (assuming it's a resource map)\n", path);
        return true;
    }

    static struct cache_file_instance cache_file = {};
    cache_file_load(path, &cache_file);
    if(!cache_file.valid) {
        fprintf(stderr, "%s: Not a valid cache file\n", path);
        return false;
    }

    bool success;
    auto cache_build = cache_file_resolve_build(cache_file.header);
    switch(cache_build) {
        case CACHE_FILE_TRACKED_BUILD_TOOL_SQUISHER:
            fprintf(stderr, "%s: Has already been squished\n", path);
            cache_file_unload(&cache_file);
            return true;
        case CACHE_FILE_TRACKED_BUILD_0563:
        case CACHE_FILE_TRACKED_BUILD_0564:
        case CACHE_FILE_TRACKED_BUILD_0609:
        case CACHE_FILE_TRACKED_BUILD_0621:
            success = postprocess_tag_data(&cache_file.tag_data);
            break;
        case CACHE_FILE_TRACKED_BUILD_UNTRACKED:
            fprintf(stderr, "%s: Unsupported build \"%s\"\n", path, cache_file.header->build_number);
            cache_file_unload(&cache_file);
            return false;
        default:
            abort();
    }

    if(!success) {
        fprintf(stderr, "%s: Could not process\n", path);
        cache_file_unload(&cache_file);
        return false;
    }

    // Set build string to a specific value so we do not process this again.
    strncpy(cache_file.header->build_number, cache_file_tracked_builds[CACHE_FILE_TRACKED_BUILD_TOOL_SQUISHER], sizeof(cache_file.header->build_number));
    if(!cache_file_fix_checksum(&cache_file)) {
        fprintf(stderr, "%s: Failed to calculate cache file checksum\n", path);
        cache_file_unload(&cache_file);
        return false;
    }

    success = file_write_from_buffer(path, cache_file.data, cache_file.size);
    if(success) {
        printf("%s: Saved!\n", path);
    }

    cache_file_unload(&cache_file);
    return success;
}

static bool postprocess_tag_data(struct tag_data_instance *tag_data) {
    if(!tag_data->valid) {
        return false;
    }

    for(size_t i = 0; i < tag_data->header->tag_count; i++) {
        struct tag_instance *tag = &tag_data->tags[i];

        // The base sound struct is always in the map, even if external
        if(tag->primary_group == TAG_FOURCC_SOUND) {
            if(sound_final_postprocess(tag->tag_id, tag_data)) {
                continue;
            }
            else {
                return false;
            }
        }

        // No tag data
        if(tag->external) {
            continue;
        }

        // Process shaders
        if(tag->primary_group == TAG_FOURCC_SHADER || tag->secondary_group == TAG_FOURCC_SHADER || tag->tertiary_group == TAG_FOURCC_SHADER) {
            if(!shader_final_postprocess(tag->tag_id, tag_data)) {
                return false;
            }
        }
        // Process units
        else if(tag->secondary_group == TAG_FOURCC_UNIT && tag->tertiary_group == TAG_FOURCC_OBJECT) {
            if(!uint_final_postprocess(tag->tag_id, tag_data)) {
                return false;
            }
        }

        // Process other tags
        switch(tag->primary_group) {
            case TAG_FOURCC_LENS_FLARE:
                if(!lens_flare_final_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
                break;
            case TAG_FOURCC_SCENARIO:
                if(!scenario_final_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
                break;
            case TAG_FOURCC_WEAPON_HUD_INTERFACE:
                if(!weapon_hud_interface_final_postprocess(tag->tag_id, tag_data)) {
                    return false;
                }
        }
    }

    return true;
}
