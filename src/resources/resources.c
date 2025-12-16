#include <stdint.h>
#include <string.h>

#include "resources.h"

uint32_t resources_get_bitmap_index(const char *tag_path) {
    if(!tag_path) {
        return RESOURCE_MAP_NO_MATCH;
    }

    for(uint32_t i = 0; i < NUMBER_OF_BITMAPS_MAP_RESOURCES; i++) {
        if(strcmp(tag_path, bitmaps_map_resource_list[i]) == 0) {
            return i + i + 1;
        }
    }

    return RESOURCE_MAP_NO_MATCH;
}

bool resources_sound_is_in_sounds_map(const char *tag_path) {
    if(!tag_path) {
        return false;
    }

    for(uint32_t i = 0; i < NUMBER_OF_SOUNDS_MAP_RESOURCES; i++) {
        if(strcmp(tag_path, sounds_map_resource_list[i]) == 0) {
            return true;
        }
    }

    return false;
}
