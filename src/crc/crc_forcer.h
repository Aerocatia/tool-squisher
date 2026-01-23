#pragma once

#include <stdint.h>

void crc_force_buffer_checksum(uint32_t *crc_reference, uint32_t new_crc, uint8_t *buffer, size_t size, size_t offset);
