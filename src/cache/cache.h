#pragma once

#include <stdint.h>
#include "../data_types.h"
#include "../tag/tag.h"

#define CACHE_FILE_HEADER_SIGNATURE 0x68656164 // head
#define CACHE_FILE_FOOTER_SIGNATURE 0x666F6F74 // foot
#define CACHE_FILE_MINIMUM_SIZE 4096
#define CACHE_FILE_MAXIMUM_SIZE INT32_MAX

enum {
    CACHE_FILE_VERSION_XBOX = 5,
    CACHE_FILE_VERSION_PC_DEMO = 6,
    CACHE_FILE_VERSION_PC_RETAIL = 7,
    CACHE_FILE_VERSION_CUSTOM_EDITION = 609
};

static const char *cache_file_tracked_builds[] = {
    "01.00.00.0000", // We touched this already (or someone is being funny)
    "01.00.00.0563", // Released with dedicated server
    "01.00.00.0564", // Final retail release
    "01.00.00.0609", // Custom Edition 1.0
    "01.00.10.0621"  // 1.10, likely the digsite build
};

enum {
    CACHE_FILE_TRACKED_BUILD_TOOL_SQUISHER,
    CACHE_FILE_TRACKED_BUILD_0563,
    CACHE_FILE_TRACKED_BUILD_0564,
    CACHE_FILE_TRACKED_BUILD_0609,
    CACHE_FILE_TRACKED_BUILD_0621,
    CACHE_FILE_TRACKED_BUILD_UNTRACKED,
    NUMBER_OF_CACHE_FILE_TRACKED_BUILDS = CACHE_FILE_TRACKED_BUILD_UNTRACKED
};
static_assert(sizeof(cache_file_tracked_builds) / sizeof(char *) == NUMBER_OF_CACHE_FILE_TRACKED_BUILDS);

#pragma pack(push, 1)

struct cache_file_header {
    uint32_t header_signature;
    uint32_t version;
    uint32_t size;
    uint32_t compressed_file_padding;
    uint32_t tags_offset;
    uint32_t tags_size;
    uint32_t index_buffer_count; // Unused
    uint32_t index_buffers_offset; // Unused
    char name[32];
    char build_number[32];
    uint16_t scenario_type;
    uint16_t pad;
    uint32_t checksum;
    uint32_t flags; // Added in MCC
    uint32_t unused[484];
    uint32_t footer_signature;
};
static_assert(sizeof(struct cache_file_header) == 2048);

struct cache_file_structure_bsp_header {
    Pointer32 structure_bsp;
    uint32_t vertex_buffer_count;
    Pointer32 vertex_buffers;
    uint32_t lightmap_vertex_buffer_count;
    Pointer32 lightmap_vertex_buffers;
    uint32_t signature;
};
static_assert(sizeof(struct cache_file_structure_bsp_header) == 24);

#pragma pack(pop)

struct cache_file_instance {
    union {
        uint8_t *data;
        struct cache_file_header *header;
    };
    size_t size;
    struct tag_data_instance tag_data;
    bool valid;
    bool dirty;
};

uint16_t cache_file_resolve_build(struct cache_file_header *header);
void cache_file_force_checksum(uint32_t new_crc, struct cache_file_instance *cache_file);
void cache_file_load(const char *path, struct cache_file_instance *cache_file);
bool cache_file_update_header(struct cache_file_instance *cache_file, bool update_build_number);
bool cache_file_save(const char *path, struct cache_file_instance *cache_file);
void cache_file_unload(struct cache_file_instance *cache_file);
