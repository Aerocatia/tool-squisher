#include <stdint.h>

#include "global_options.h"

const char *global_option_long_names[] = {
    GLOBAL_OPTION_ARG_HELP_STRING,
    GLOBAL_OPTION_ARG_NO_PRESERVE_CRC_STRING,
    GLOBAL_OPTION_ARG_RELAXED_STRING,
    GLOBAL_OPTION_ARG_VERSION_STRING
};
static_assert(sizeof(global_option_long_names) / sizeof(char *) == NUMBER_OF_GLOBAL_OPTION_ARGS);

const char *global_option_short_names[] = {
    "h",
    "n",
    "r",
    "v"
};
static_assert(sizeof(global_option_long_names) / sizeof(char *) == NUMBER_OF_GLOBAL_OPTION_ARGS);

const char *global_option_help[] = {
    "Print this help text",
    "Do not forge the cache file crc32 after processing",
    "Relax some cache file integrity checks",
    "Print the version"
};
static_assert(sizeof(global_option_long_names) / sizeof(char *) == NUMBER_OF_GLOBAL_OPTION_ARGS);

uint32_t global_option_flags = 0;
