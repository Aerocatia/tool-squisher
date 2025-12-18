#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "tag.h"

#include "../data_types.h"
#include "tag_fourcc.h"

static const char *TAG_INVALID_PATH = "<invalid>";

void *tag_resolve_pointer(Pointer32 data_pointer, struct tag_data_instance *tag_data, size_t needed_size) {
    assert(tag_data && tag_data->data);
    if(data_pointer < tag_data->data_load_address) {
        return nullptr;
    }

    size_t offset = data_pointer - tag_data->data_load_address;
    if(offset + needed_size > tag_data->size) {
        return nullptr;
    }

    return tag_data->data + offset;
}

void *tag_reflexive_get_element(struct tag_reflexive *reflexive, uint32_t index, size_t size, struct tag_data_instance *tag_data) {
    assert(tag_data && tag_data->valid);
    if(index >= reflexive->count) {
        return nullptr;
    }

    // Totally not pointing to bullshit. Trust me...im a dolphin
    return tag_resolve_pointer(reflexive->address + index * size, tag_data, size);
}

void *tag_get(TagID tag_id, uint32_t tag_group, struct tag_data_instance *tag_data) {
    assert(tag_data && tag_data->valid);
    if(tag_id.whole_id == NULL_ID) {
        return nullptr;
    }

    auto tag_count = tag_data->header->tag_count;
    if(tag_id.index >= tag_count) {
        fprintf(stderr, "requested tag ID %#x is out of bounds for the number of tags in the map (%u >= %u)\n", tag_id.whole_id, tag_id.index, tag_count);
        return nullptr;
    }

    // The base struct for sound tags is always in the map
    if(tag_data->tags[tag_id.index].external && tag_group != TAG_FOURCC_SOUND) {
        fprintf(stderr, "requested tag ID %#x is external and not loaded in the map\n", tag_id.whole_id);
        return nullptr;
    }
    if(tag_data->tags[tag_id.index].tag_id.whole_id != tag_id.whole_id) {
        fprintf(stderr, "requested tag ID %#x did not match tag array ID %#x\n", tag_id.whole_id, tag_data->tags[tag_id.index].tag_id.whole_id);
        return nullptr;
    }

    // Matching any of these is valid
    auto primary_group = tag_data->tags[tag_id.index].primary_group;
    auto secondary_group = tag_data->tags[tag_id.index].secondary_group;
    auto tertiary_group = tag_data->tags[tag_id.index].tertiary_group;
    if(tag_group == primary_group || tag_group == secondary_group || tag_group == tertiary_group) {
        size_t needed_size = tag_fourcc_get_base_struct_size(tag_group);
        return tag_resolve_pointer(tag_data->tags[tag_id.index].base_address, tag_data, needed_size);
    }

    fprintf(stderr, "requested tag group \"%s\" is not a valid match for tag array group \"%s\"\n", tag_fourcc_to_extension(tag_group), tag_fourcc_to_extension(primary_group));
    return nullptr;
}

const char *tag_path_get_maybe(TagID tag, struct tag_data_instance *tag_data) {
    assert(tag_data && tag_data->valid);
    if(tag.whole_id == NULL_ID) {
        return nullptr;
    }

    auto tag_count = tag_data->header->tag_count;
    if(tag.index >= tag_count) {
        fprintf(stderr, "requested tag ID %#x is out of bounds for the number of tags in the map (%u >= %u)\n", tag.whole_id, tag.index, tag_count);
        return nullptr;
    }

    const char *tag_path = tag_resolve_pointer(tag_data->tags[tag.index].name_address, tag_data, 1);
    if(!tag_path) {
        return nullptr;
    }

    ptrdiff_t max_possible_length = tag_data->data + tag_data->size - (uint8_t *)tag_path;
    assert(max_possible_length > 0);
    if(max_possible_length > MAX_TAG_PATH_LENGTH) {
        max_possible_length = MAX_TAG_PATH_LENGTH;
    }

    for(size_t i = 0; i < (size_t)max_possible_length; i++) {
        if(tag_path[i] == '\0') {
            return tag_path;
        }
    }

    return nullptr;
}

const char *tag_path_get(TagID tag, struct tag_data_instance *tag_data) {
    assert(tag_data && tag_data->valid);
    const char *tag_path = tag_path_get_maybe(tag, tag_data);
    if(!tag_path) {
        return TAG_INVALID_PATH;
    }

    return tag_path;
}

const char *tag_extension_get(TagID tag, struct tag_data_instance *tag_data) {
    assert(tag_data && tag_data->valid);
    if(tag.whole_id == NULL_ID) {
        return TAG_INVALID_PATH;
    }

    auto tag_count = tag_data->header->tag_count;
    if(tag.index >= tag_count) {
        fprintf(stderr, "requested tag ID %#x is out of bounds for the number of tags in the map (%u >= %u)\n", tag.whole_id, tag.index, tag_count);
        return TAG_INVALID_PATH;
    }

    return tag_fourcc_to_extension(tag_data->tags[tag.index].primary_group);
}
