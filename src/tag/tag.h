#pragma once

#include <stdint.h>
#include "../data_types.h"

#define TAG_DATA_LOAD_ADDRESS 0x40440000
#define MAX_TAG_PATH_LENGTH 260

enum {
    TAG_DATA_FLAGS_EXTERNAL_BIT, // if the data is in a resource map (only used by sounds)
    NUMBER_OF_TAG_DATA_FLAGS
};

#pragma pack(push, 1)

struct tag_data_header {
	Pointer32 tag_instances;
	TagID scenario_tag;
	uint32_t tag_data_checksum;
	uint32_t tag_count;
	uint32_t vertex_buffer_count;
	uint32_t vertex_buffers_offset; // File offset
	uint32_t index_buffer_count;
	uint32_t index_buffers_offset; // Offset from vertex buffers
	uint32_t model_data_size;
	uint32_t signature;
};
static_assert(sizeof(struct tag_data_header) == 40);

struct tag_instance {
    uint32_t primary_group;
    uint32_t secondary_group;
    uint32_t tertiary_group;
	TagID tag_id;
	Pointer32 name_address;
	Pointer32 base_address;
	uint32_t external;
	uint32_t padding;
};
static_assert(sizeof(struct tag_instance) == 32);

struct tag_reflexive {
	uint32_t count;
	Pointer32 address; // 32-bit pointer to array
	Pointer32 definition; // 32-bit pointer to tag definition (in-engine only)
};
static_assert(sizeof(struct tag_reflexive) == 12);

struct tag_data {
	uint32_t size;
    uint32_t flags;
	uint32_t file_offset; // Not in loose tags
	Pointer32 address; // 32-bit pointer to data
	Pointer32 definition;// 32-bit pointer to data definition (in-engine only)
};
static_assert(sizeof(struct tag_data) == 20);

struct tag_reference {
	uint32_t tag_group;
	Pointer32 name; // 32-bit pointer to name
	uint32_t name_length;
	TagID index; // Tag index (two-part tag id)
};
static_assert(sizeof(struct tag_reference) == 16);

#pragma pack(pop)

struct tag_data_instance {
    union {
        uint8_t *data;
        struct tag_data_header *header;
    };
    size_t size;
    struct tag_instance *tags;
    Pointer32 data_load_address;
    bool indexed_external_tags;
    bool valid;
};

bool tag_id_is_valid_tag(TagID tag, struct tag_data_instance *tag_data);
bool tag_is_external(TagID tag, struct tag_data_instance *tag_data);
void *tag_resolve_pointer(Pointer32 data_pointer, size_t needed_size, struct tag_data_instance *tag_data);
void *tag_reflexive_get_element(struct tag_reflexive *reflexive, uint32_t index, size_t element_size, struct tag_data_instance *tag_data);
bool tag_reflexive_erase_element_data(struct tag_reflexive *reflexive, size_t element_size, struct tag_data_instance *tag_data);
void *tag_get(TagID tag_id, uint32_t tag_group, struct tag_data_instance *tag_data);
const char *tag_path_get_maybe(TagID tag, struct tag_data_instance *tag_data);
const char *tag_path_get(TagID tag, struct tag_data_instance *tag_data);
const char *tag_extension_get(TagID tag, struct tag_data_instance *tag_data);
void tag_null_reference(struct tag_reference *reference, uint32_t tag_group);
