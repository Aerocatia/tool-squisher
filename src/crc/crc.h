#pragma once

#include <stdint.h>

#define CRC_NEW 0xFFFFFFFF

void crc_new(uint32_t *crc);
void crc_checksum_buffer(uint32_t *crc, const void *buffer, size_t size);
