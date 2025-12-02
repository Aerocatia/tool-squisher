#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

void file_read_into_buffer(const char *path, uint8_t **buffer, size_t *buffer_size) {
    FILE *f = fopen(path, "rb");
    if(!f) {
        fprintf(stderr, "%s: Failed to open\n", path);
        *buffer = nullptr;
        return;
    }

    fseek(f, 0, SEEK_END);
    size_t input_buffer_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8_t *input_buffer = calloc(input_buffer_size, 1);
    if(!input_buffer) {
        fprintf(stderr, "%s: Failed to allocate memory\n", path);
        fclose(f);
        *buffer = nullptr;
        return;
    }

    fread(input_buffer, input_buffer_size, 1, f);
    fclose(f);
    *buffer = input_buffer;
    *buffer_size = input_buffer_size;
}

bool file_write_from_buffer(const char *path, uint8_t *buffer, size_t buffer_size) {
    bool success = true;
    FILE *f = fopen(path, "wb");
    if(f) {
        if(!fwrite(buffer, buffer_size, 1, f)) {
            fprintf(stderr, "%s: Write failed. The map is likely fucked now! LOL\n", path);
            success = false;
        }
        fclose(f);
    }
    else {
        fprintf(stderr, "%s: Can not open file for writing\n", path);
        success = false;
    }

    return success;
}

bool file_path_is_resource_map(const char *path) {
    size_t path_len = strlen(path);
    if(path_len >= 7 && strcmp(path + path_len - 7, "loc.map") == 0) {
        return true;
    }
    if(path_len >= 10 && strcmp(path + path_len - 10, "sounds.map") == 0) {
        return true;
    }
    if(path_len >= 11 && strcmp(path + path_len - 11, "bitmaps.map") == 0) {
        return true;
    }
    return false;
}
