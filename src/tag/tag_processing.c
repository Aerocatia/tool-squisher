#include <stdint.h>
#include <math.h>
#include <assert.h>

#include "../data_types.h"

static inline void byteswap16(void *value) {
    assert(value);
    *(uint16_t *)value = __builtin_bswap16(*(uint16_t *)value);
}

static inline void byteswap32(void *value) {
    assert(value);
    *(uint32_t *)value = __builtin_bswap32(*(uint32_t *)value);
}

void tag_process_enum16(uint16_t *field, uint16_t option_count, uint16_t option_default) {
    assert(field);
    byteswap16(field);
    if(*field >= option_count) {
        *field = option_default;
    }
}

void tag_process_float(float *field) {
    assert(field);
    byteswap32(field);
    if(!isfinite(*field)) {
        *field = 0.0f;
    }
}
