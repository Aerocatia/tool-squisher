#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "cache.h"

#include "../data_types.h"
#include "../crc/crc.h"
#include "../file/file.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"
#include "../tag_groups/scenario.h"

static bool cache_file_verify_header(struct cache_file_header *header) {
    assert(header);
    if(header->header_signature != CACHE_FILE_HEADER_SIGNATURE ||
       header->footer_signature != CACHE_FILE_FOOTER_SIGNATURE ||
       header->name[31] != '\0' ||
       header->build_number[31] != '\0') {
        return false;
    }

    if(header->version == CACHE_FILE_VERSION_PC_RETAIL || header->version == CACHE_FILE_VERSION_CUSTOM_EDITION) {
        return true;
    }

    return false;
}

// Similar to the check in Invader and Chimera
static bool cache_file_tag_data_is_corrupt(struct tag_data_instance *tag_data) {
    assert(tag_data && tag_data->valid);

    // Check if the scenario tag is a scenario tag
    auto scenario_index = tag_data->header->scenario_tag.index;
    if(tag_data->tags[scenario_index].primary_group != TAG_FOURCC_SCENARIO) {
        return true;
    }

    // Get the scenario
    struct scenario *scenario_tag = tag_get(tag_data->header->scenario_tag, TAG_FOURCC_SCENARIO, tag_data);
    if(!scenario_tag) {
        return true;
    }

    size_t tag_array_bsp_count = 0;
    const size_t scenario_bsp_count = scenario_tag->structure_bsp_references.count;
    for(size_t t = 0; t < tag_data->header->tag_count; t++) {
        const char *tag_path = tag_path_get_maybe(tag_data->tags[t].tag_id, tag_data);
        if(!tag_path) {
            return true;
        }

        // Empty path is not allowed
        if(strcmp(tag_path, "") == 0) {
            return true;
        }

        auto tag_group = tag_data->tags[t].primary_group;

        // If it says it's a BSP, check if it's really a BSP
        if(tag_group == TAG_FOURCC_SCENARIO_STRUCTURE_BSP) {
            tag_array_bsp_count++;
            bool bsp_found = false;
            for(size_t i = 0; i < scenario_bsp_count && !bsp_found; i++) {
                struct scenario_structure_bsp_reference *bsp = scenario_get_bsp_reference(scenario_tag, i, tag_data);
                if(!bsp) {
                    return true;
                }

                bsp_found = bsp->structure_bsp.index.index == t;
            }

            if(!bsp_found) {
                return true;
            }
        }

        // This is an Invader thing but for the sake of this check we allow it
        if(tag_group == TAG_FOURCC_NONE) {
            continue;
        }

        // Tag group fourcc must be known
        if(!tag_fourcc_is_valid_tag(tag_group)) {
            return true;
        }

        // Check for duplicate tag paths
        for(size_t t2 = 0; t2 < t; t2++) {
            if(tag_group != tag_data->tags[t2].primary_group) {
                continue;
            }

            // We got this path earlier
            const char *compare_path = tag_path_get_maybe(tag_data->tags[t2].tag_id, tag_data);
            if(strcmp(tag_path, compare_path) == 0) {
                return true;
            }
        }
    }

    // Check for stealth/orphan BSPs (old Eschaton)
    if(tag_array_bsp_count != scenario_bsp_count) {
        return true;
    }

    return false;
}

uint16_t cache_file_resolve_build(struct cache_file_header *header) {
    assert(header);
    for(uint16_t i = 0; i < NUMBER_OF_CACHE_FILE_TRACKED_BUILDS; i++) {
        if(strcmp(header->build_number, cache_file_tracked_builds[i]) == 0) {
            return i;
        }
    }

    // Unknown
    return CACHE_FILE_TRACKED_BUILD_UNTRACKED;
}

bool cache_file_fix_checksum(struct cache_file_instance *cache_file) {
    assert(cache_file && cache_file->valid);
    struct scenario *scenario_tag = tag_get(cache_file->tag_data.header->scenario_tag, TAG_FOURCC_SCENARIO, &cache_file->tag_data);
    if(!scenario_tag) {
        return false;
    }

    crc_new(&cache_file->header->checksum);
    for(size_t i = 0; i < scenario_tag->structure_bsp_references.count; i++) {
        struct scenario_structure_bsp_reference *bsp = scenario_get_bsp_reference(scenario_tag, i, &cache_file->tag_data);
        if(!bsp && (bsp->offset > cache_file->size || (uint64_t)bsp->offset + (uint64_t)bsp->size > cache_file->size)) {
            return false;
        }
        crc_checksum_buffer(&cache_file->header->checksum, cache_file->data + bsp->offset, bsp->size);
    }

    size_t model_data_offset = cache_file->tag_data.header->vertex_buffers_offset;
    size_t model_data_size = cache_file->tag_data.header->model_data_size;
    if(model_data_offset > cache_file->size || (uint64_t)model_data_offset + (uint64_t)model_data_size > cache_file->size) {
        return false;
    }

    crc_checksum_buffer(&cache_file->header->checksum, cache_file->data + model_data_offset, model_data_size);
    crc_checksum_buffer(&cache_file->header->checksum, cache_file->tag_data.data, cache_file->tag_data.size);

    return true;
}

void cache_file_load(const char *path, struct cache_file_instance *cache_file) {
    assert(cache_file && !cache_file->data);
    file_read_into_buffer(path, &cache_file->data, &cache_file->size);
    if(!cache_file->data) {
        memset(cache_file, 0, sizeof(struct cache_file_instance));
        return;
    }

    if(cache_file->size < CACHE_FILE_MINIMUM_SIZE) {
        goto cleanup;
    }

    if(!cache_file_verify_header(cache_file->header)) {
        goto cleanup;
    }

    // Get tag data info
    if(cache_file->header->tags_offset > INT32_MAX) {
        fprintf(stderr, "%s: Tag data offset is too large\n", cache_file->header->name);
        goto cleanup;
    }
    if(cache_file->header->tags_size > INT32_MAX) {
        fprintf(stderr, "%s: Tag data size is too large\n", cache_file->header->name);
        goto cleanup;
    }
    if(cache_file->header->tags_offset > cache_file->size) {
        fprintf(stderr, "%s: Tag data offset is out of bounds for the cache file\n", cache_file->header->name);
        goto cleanup;
    }
    if(cache_file->header->tags_offset + cache_file->header->tags_size > cache_file->size) {
        fprintf(stderr, "%s: Tag data size is out of bounds for the cache file\n", cache_file->header->name);
        goto cleanup;
    }
    if(cache_file->header->tags_size < sizeof(struct tag_data_header)) {
        fprintf(stderr, "%s: Tag data size is too small to have valid tag data\n", cache_file->header->name);
        goto cleanup;
    }

    cache_file->tag_data.data = cache_file->data + cache_file->header->tags_offset;
    if(cache_file->tag_data.header->tag_count > INT16_MAX) {
        fprintf(stderr, "%s: Too many tags to have valid tag data\n", cache_file->header->name);
        goto cleanup;
    }

    cache_file->tag_data.size = cache_file->header->tags_size;
    cache_file->tag_data.data_load_address = TAG_DATA_LOAD_ADDRESS;

    // We do not allow this on non-mp maps because the other map types can hit conditions where stale tag data pointers are used (i.e. checkpoints)
    if(cache_file->header->version == CACHE_FILE_VERSION_CUSTOM_EDITION && cache_file->header->scenario_type == SCENARIO_TYPE_MULTIPLAYER) {
        cache_file->tag_data.indexed_external_tags = true;
    }

    cache_file->tag_data.tags = tag_resolve_pointer(cache_file->tag_data.header->tag_instances, sizeof(struct tag_instance) * cache_file->tag_data.header->tag_count, &cache_file->tag_data);
    if(!cache_file->tag_data.tags) {
        fprintf(stderr, "%s: Tag array is out of bounds\n", cache_file->header->name);
        goto cleanup;
    }

    // Valid enough to parse
    cache_file->valid = true;
    cache_file->tag_data.valid = true;

    // Check for basic corruption
    if(cache_file_tag_data_is_corrupt(&cache_file->tag_data)) {
        fprintf(stderr, "%s: Tag data appears to be corrupt\n", cache_file->header->name);
        goto cleanup;
    }

    return;

    cleanup:
    free(cache_file->data);
    memset(cache_file, 0, sizeof(struct cache_file_instance));
}

void cache_file_unload(struct cache_file_instance *cache_file) {
    assert(cache_file && cache_file->data);
    free(cache_file->data);
    memset(cache_file, 0, sizeof(struct cache_file_instance));
}
