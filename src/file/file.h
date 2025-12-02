#pragma once

#include <stdint.h>

void file_read_into_buffer(const char *path, uint8_t **buffer, size_t *buffer_size);
bool file_write_from_buffer(const char *path, uint8_t *buffer, size_t buffer_size);
bool file_path_is_resource_map(const char *path);
