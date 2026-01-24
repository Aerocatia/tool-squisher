#pragma once

#include <stdint.h>
#include <limits.h>

#define GLOBAL_OPTION_ARG_HELP_STRING "help"
#define GLOBAL_OPTION_ARG_NO_PRESERVE_CRC_STRING "no-preserve-crc"
#define GLOBAL_OPTION_ARG_RELAXED_STRING "relaxed"
#define GLOBAL_OPTION_ARG_VERSION_STRING "version"

enum {
    GLOBAL_OPTON_FLAGS_NO_PRESERVE_CRC_BIT,
    GLOBAL_OPTON_FLAGS_RELAXED_BIT,
    NUMBER_OF_GLOBAL_OPTION_FLAGS
};
static_assert(NUMBER_OF_GLOBAL_OPTION_FLAGS <= sizeof(uint32_t) * CHAR_BIT);

enum {
    GLOBAL_OPTION_ARG_HELP,
    GLOBAL_OPTION_ARG_NO_PRESERVE_CRC,
    GLOBAL_OPTION_ARG_RELAXED,
    GLOBAL_OPTION_ARG_VERSION,
    NUMBER_OF_GLOBAL_OPTION_ARGS
};

extern uint32_t global_option_flags;
extern const char *global_option_long_names[];
extern const char *global_option_short_names[];
extern const char *global_option_help[];
