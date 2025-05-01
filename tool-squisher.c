/**
 * # Tool Squisher
 *
 * ## Shader Squishing
 *
 * Truncates shader types into 8-bit values. This is used to fix maps compiled
 * with the leaked 1.10 beta build, enabling them to work with the official
 * Halo Custom Edition release.
 *
 * The reason these maps break on official releases is because the leaked 1.10
 * build reads and writes the shader type enum as an 8-bit value, with the
 * other 8 bits now padded with a 0xFF byte when stored.
 *
 * Official releases read these enum fields as 16-bit, thus they will read a
 * shader value like 0x0005 (shader_transparent_generic) as 0xFF05, leading to
 * all sorts of fun undefined behavior, like non-shader_environment tags being
 * read as shader_environment, which leads to a crash.
 *
 * Note that the shader struct exists in more tags than just shader tags, but
 * only shader tags are impacted by this.
 *
 * ## Scenario Squishing
 *
 * Fixes variant numbers in conversations being incorrectly set to 0 when they
 * are unused. This was due to a bug in the leaked 1.10 beta build's tool.exe
 * where it didn't initialize the variant numbers to 0xFFFF. This breaks even
 * on the leaked 1.10 build (thus conversations will ALWAYS be broken on the tag
 * build which doesn't use cache files that could be fixed with this tool).
 *
 * ## Lens Flare Squishing
 *
 * Fixes Halo Editing Kit's tool.exe setting rotation scaling to 360 radians,
 * which is likely higher (~58 times) than what the developer intended, 360
 * degrees.
 *
 * ## Sound Squishing
 *
 * Fixes Halo Editing Kit's tool.exe not defaulting sound distance bounds.
 * This was not done by Xbox tool.exe either, but was stated by the developers
 * as intended behavior, and was fixed in both the leaked 1.10 build and MCC.
 *
 * ## Weapon HUD Interface Squishing
 *
 * Fixes Weapon HUD Interface tags causing undefined behavior when they have a
 * crosshair overlay that is conditionally visible depending on if the weapon
 * is zoomed in.
 *
 * The reason this occurs is due to an oversight in tool.exe. When processing
 * Weapon HUD Interface tags, tool.exe sets a bitfield that maps all crosshair
 * types, where its respective bit is true if that crosshair type is present
 * in the tag.
 *
 * These flags toggle behavior in the overlay, and this includes the ability
 * to hide/show crosshair overlays depending on if the weapon is zoomed in,
 * even if that overlay is not a zoom crosshair. The fix is to detect these
 * overlays and also set the flag if they are present.
 *
 * For reference, zoom overlays are just for displaying a different bitmap
 * depending on the current zoom level (e.g. current magnification level, a
 * smaller crosshair for higher zoom levels, etc.). You may want additional
 * bitmaps to be shown only when zoomed, but you might want it to be the same
 * on all zoom levels (otherwise you would have to have multiple copies of the
 * bitmap to avoid accessing out-of-bounds data). The stock sniper rifle even
 * does this, and if you were to remove the 2x/8x magnification level and then
 * build the map with tool.exe, other parts of the HUD will be broken since
 * the flag won't be set anymore.
 *
 * --
 *
 * This software is licensed under version 3 of the GNU General Public License
 * as published by the Free Software Foundation in 2007. It is not licensed
 * under any other license, even including later or earlier versions of the GPL.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

static bool meme(const char *path);
int main(int argc, const char **argv) {
    if(argc == 1) {
        fprintf(stderr, "Usage: %s <map> [map [...]]\n", argv[0]);
        return 1;
    }

    bool success = false;
    for(int i = 1; i < argc; i++) {
        success = meme(argv[i]) || success;
    }
    return success ? 0 : 1;
}

static bool do_it(const char *path, uint8_t *bytes, size_t length);
static bool meme(const char *path) {
    FILE *f = fopen(path, "rb");
    if(!f) {
        fprintf(stderr, "%s: Cannot open for reading!\n", path);
        return false;
    }

    fseek(f, 0, SEEK_END);
    size_t len = ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8_t *a = calloc(len, 1);
    if(!a) {
        fprintf(stderr, "%s: Not enough RAM to read!\n", path);
        fclose(f);
        return false;
    }
    fread(a, len, 1, f);
    fclose(f);

    bool success = do_it(path, a, len);
    if(success) {
        FILE *f = fopen(path, "wb");
        if(f) {
            if(fwrite(a, len, 1, f)) {
                printf("%s: Saved!\n", path);
            }
            else {
                fprintf(stderr, "%s: Write failed. The map is likely fucked now! LOL\n", path);
                success = false;
            }
            fclose(f);
        }
        else {
            fprintf(stderr, "%s: Cannot open for writing!\n", path);
            success = false;
        }
    }
    free(a);

    return success;
}

static bool fix_crc32(
    const char *path,
    uint8_t *map_data,
    size_t map_data_length,
    uint8_t *tag_data,
    size_t tag_data_length
);

static bool fix_tag_data(const char *path, uint8_t *tag_data, size_t tag_data_length);
static bool do_it(const char *path, uint8_t *bytes, size_t length) {
    if(length < 0x800 || *(uint32_t *)(bytes + 0x0) != 0x68656164 || *(uint32_t *)(bytes + 0x7FC) != 0x666F6F74 || *(uint32_t *)(bytes + 0x4) != 0x261) {
        // Avoid printing errors into the terminal if it's a resource map and not an actual cache file
        size_t path_len = strlen(path);
        if(path_len >= 7 && strcmp(path + path_len - 7, "loc.map") == 0) {
            return false;
        }
        if(path_len >= 10 && strcmp(path + path_len - 10, "sounds.map") == 0) {
            return false;
        }
        if(path_len >= 11 && strcmp(path + path_len - 11, "bitmaps.map") == 0) {
            return false;
        }
        fprintf(stderr, "%s: Not a Custom Edition map!\n", path);
        return false;
    }
    const char *build_string = (const char *)bytes + 0x40;

    const char *expected_build_string_a = "01.00.00.0609";
    const char *expected_build_string_b = "01.00.10.0621";

    if(build_string[0x1F] != 0) {
        fprintf(stderr, "%s: Invalid build string!\n", path);
        return false;
    }
    if(strncmp(build_string, expected_build_string_a, 32) != 0 && strncmp(build_string, expected_build_string_b, 32) != 0) {
        fprintf(stderr, "%s: Build string `%s` is not `%s` or `%s`, so this tool would be ineffective\n", path, build_string, expected_build_string_a, expected_build_string_b);
        return false;
    }

    size_t tag_data_offset = (size_t)*(uint32_t *)(bytes + 0x10);
    size_t tag_data_length = (size_t)*(uint32_t *)(bytes + 0x14);

    uint8_t *tag_data = bytes + tag_data_offset;
    if(tag_data < bytes) {
        fprintf(stderr, "%s: Invalid tag data offset (overflows)\n", path);
        return false;
    }
    if(tag_data_length + tag_data_offset > length) {
        fprintf(stderr, "%s: Invalid tag data length (length out-of-bounds)\n", path);
        return false;
    }
    if(tag_data_length + tag_data_offset < tag_data_length) {
        fprintf(stderr, "%s: Invalid tag data length (overflows)\n", path);
        return false;
    }
    if(tag_data_length < 256) {
        fprintf(stderr, "%s: Invalid tag data length too small for tag data\n", path);
        return false;
    }

    if(!fix_tag_data(path, bytes + tag_data_offset, tag_data_length)) {
        return false;
    }

    if(!fix_crc32(path, bytes, length, bytes + tag_data_offset, tag_data_length)) {
        return false;
    }

    return true;
}

static void *resolve_tag_data_offset(
    const char *path,
    uint8_t *tag_data,
    size_t tag_data_length,
    uint32_t address,
    size_t needed_length
);

typedef struct Tag {
    uint32_t primary;
    uint32_t secondary;
    uint32_t tertiary;
    uint32_t tag_id;
    uint32_t tag_path_addr;
    uint32_t tag_data_addr;
    uint32_t external;
    uint32_t padding;
} Tag;

const uint32_t LENS_FLARE_GROUP_FOURCC = 0x6C656E73;
const uint32_t SCENARIO_GROUP_FOURCC = 0x73636E72;
const uint32_t SHADER_GROUP_FOURCC = 0x73686472;
const uint32_t SOUND_GROUP_FOURCC = 0x736E6421;
const uint32_t WEAPON_HUD_INTERFACE_GROUP_FOURCC = 0x77706869;

static bool fix_shader_struct(uint8_t *shader_data);
static bool fix_scenario(const char *path, uint8_t *tag_data, size_t tag_data_length, uint8_t *scenario_data);
static bool fix_lens_flare(uint8_t *lens_data);
static bool fix_sound(uint8_t *sound_data);
static bool fix_weapon_hud_interface(const char *path, uint8_t *tag_data, size_t tag_data_length, uint8_t *weapon_hud_interface_data);

static bool fix_tag_data(const char *path, uint8_t *tag_data, size_t tag_data_length) {
    size_t tag_count = (size_t)*(uint32_t *)(tag_data + 0xC);
    if(tag_count > 65535) {
        fprintf(stderr, "%s: Invalid tag count (%u)\n", path, tag_count);
        return NULL;
    }

    Tag *tags = resolve_tag_data_offset(path, tag_data, tag_data_length, *(uint32_t *)tag_data, sizeof(*tags) * tag_count);
    if(!tags) {
        return false;
    }

    bool changes_made = false;

    for(size_t i = 0; i < tag_count; i++) {
        Tag *tag = tags + i;

        // the base sound struct is always in the map, even if external
        if(tag->primary == SOUND_GROUP_FOURCC) {
            if(fix_sound(resolve_tag_data_offset(path, tag_data, tag_data_length, tag->tag_data_addr, 0x10))) {
                changes_made = true;
            }
        }

        if(tag->external) {
            continue;
        }

        if(      /* 👩 */
            /* 🙉 */    /* 🛢️ */     /* 🛢️ */
            tag->primary   == SHADER_GROUP_FOURCC
                           || /* 🛢️ */  /* 🛢️ */
            tag->secondary == SHADER_GROUP_FOURCC
                           ||        /* 🧑 */
            tag->tertiary  == SHADER_GROUP_FOURCC
        ) {
            if(fix_shader_struct(resolve_tag_data_offset(path, tag_data, tag_data_length, tag->tag_data_addr, 0x28))) {
                changes_made = true;
            }
        }

        if(tag->primary == SCENARIO_GROUP_FOURCC) {
            if(fix_scenario(path, tag_data, tag_data_length, resolve_tag_data_offset(path, tag_data, tag_data_length, tag->tag_data_addr, 0x468+0xC))) {
                changes_made = true;
            }
        }

        if(tag->primary == LENS_FLARE_GROUP_FOURCC) {
            if(fix_lens_flare(resolve_tag_data_offset(path, tag_data, tag_data_length, tag->tag_data_addr, 0x88))) {
                changes_made = true;
            }
        }

        if(tag->primary == WEAPON_HUD_INTERFACE_GROUP_FOURCC) {
            if(fix_weapon_hud_interface(path, tag_data, tag_data_length, resolve_tag_data_offset(path, tag_data, tag_data_length, tag->tag_data_addr, 0xA0))) {
                changes_made = true;
            }
        }
    }

    if(!changes_made) {
        printf("%s: No changes necessary\n", path);
    }

    return changes_made;
}

static bool fix_shader_struct(uint8_t *shader_data) {
    if(!shader_data) {
        return false;
    }
    uint16_t *data = (uint16_t *)(shader_data + 0x24);
    if(*data < 0xFF00) {
        return false;
    }

    uint16_t new_data = (*data) & 0x00FF;
    if(new_data == *data) {
        return false;
    }
    *data = new_data;
    return true;
}

static bool fix_lens_flare(uint8_t *lens_data) {
    if(!lens_data) {
        return false;
    }
    float *scale = (float *)(lens_data + 0x84);
    if(*scale == 360.0) {
        *scale = 6.283185307179586;
        return true;
    }
    return false;
}

static bool fix_sound(uint8_t *sound_data) {
    if(!sound_data) {
        return false;
    }
    float *minimum_distance = (float *)(sound_data + 0x8);
    float *maximum_distance = (float *)(sound_data + 0xC);
    if(*minimum_distance > 0.0 && *maximum_distance > 0.0) {
        return false;
    }

    uint16_t sound_class = *(uint16_t *)(sound_data + 0x4);
    float minimum_default;
    float maximum_default;
    switch(sound_class) {
        case 0:  // projectile impact
        case 22: // vehicle collision
        case 23: // vehicle engine
            minimum_default = 1.4;
            maximum_default = 8.0;
            break;
        case 1:  // projectile detonation
            minimum_default = 8.0;
            maximum_default = 120.0;
            break;
        case 4:  // weapon fire
            minimum_default = 4.0;
            maximum_default = 70.0;
            break;
        case 5:  // weapon ready
        case 6:  // weapon reload
        case 7:  // weapon empty
        case 8:  // weapon charge
        case 9:  // weapon overheat
        case 10: // weapon idle
            minimum_default = 1.0;
            maximum_default = 9.0;
            break;
        case 13: // object impacts
        case 14: // particle impacts
        case 15: // slow particle impacts
        case 30: // device computers
        case 35: // ambient computers
        case 39: // first person damage
            minimum_default = 0.5;
            maximum_default = 3.0;
            break;
        case 18: // unit footsteps
            minimum_default = 0.9;
            maximum_default = 10.0;
            break;
        case 19: // unit dialog
        case 44: // scripted dialog player
        case 46: // scripted dialog other
        case 47: // scripted dialog force unspatialized
        case 50: // game event
            minimum_default = 3.0;
            maximum_default = 20.0;
            break;
        case 26: // device door
        case 27: // device force field
        case 28: // device machinery
        case 29: // device nature
        case 32: // music
        case 33: // ambient nature
        case 34: // ambient machinery
            minimum_default = 0.9;
            maximum_default = 5.0;
            break;
        case 45: // scripted effect
            minimum_default = 2.0;
            maximum_default = 5.0;
            break;
       default:
           return false;
    }
    if(*minimum_distance <= 0.0) {
        *minimum_distance = minimum_default;
    }
    if(*maximum_distance <= 0.0) {
        *maximum_distance = maximum_default;
    }
    return true;
}

static const char *get_tag_path(const char *path, uint8_t *tag_data, size_t tag_data_length, uint32_t tag_id) {
    // this was already checked; just proceed
    size_t tag_count = (size_t)*(uint32_t *)(tag_data + 0xC);
    size_t tag_index = tag_id & 0x0000FFFF;
    if(tag_index >= tag_count) {
        printf("%s: BAD TAG ID %08X\n", path, tag_id);
        exit(135);
    }

    size_t MAX_PATH = 260;
    Tag *tags = resolve_tag_data_offset(path, tag_data, tag_data_length, *(uint32_t *)tag_data, sizeof(*tags) * tag_count);
    const char *tag_path = resolve_tag_data_offset(path, tag_data, tag_data_length, tags[tag_index].tag_path_addr, MAX_PATH);
    if(tag_path == NULL) {
        exit(135);
    }
    for(int i = 0; i < MAX_PATH; i++) {
        if(tag_path[i] == 0) {
            return tag_path;
        }
    }
    printf("%s: BAD TAG PATH %08X\n", path, tag_id);
    exit(135);
}

static bool fix_scenario(const char *path, uint8_t *tag_data, size_t tag_data_length, uint8_t *scenario_data) {
    if(!scenario_data) {
        return false;
    }
    uint32_t conversation_size = 116;
    uint32_t conversation_count = *(uint32_t *)(scenario_data + 0x468);
    if(conversation_count == 0) {
        return false;
    }

    uint8_t *conversations = resolve_tag_data_offset(path, tag_data, tag_data_length, *(uint32_t *)(scenario_data + 0x468 + 4), conversation_size * (size_t)conversation_count);
    if(conversations == NULL) {
        printf("%s: BAD AI CONVERSATIONS!\n", path);
        exit(135);
    }

    bool did_something = false;

    for(uint32_t conversation = 0; conversation < conversation_count; conversation++, conversations += conversation_size) {
        uint32_t participation_size = 84;
        uint32_t participant_count = *(uint32_t *)(conversations + 0x50);
        if(participant_count == 0) {
            continue;
        }
        uint8_t *participants = resolve_tag_data_offset(path, tag_data, tag_data_length, *(uint32_t *)(conversations + 0x50 + 4), participation_size * (size_t)participant_count);
        if(participants == NULL) {
            printf("%s: BAD PARTICIPANTS!\n", path);
            exit(135);
        }
        for(uint32_t participant = 0; participant < participant_count; participant++, participants += participation_size) {
            uint16_t *variant_number = (uint16_t *)(participants + 0x18);
            uint32_t line_size = 124;
            uint32_t line_count = *(uint32_t *)(conversations + 0x5C);
            uint16_t meme_indices[6] = {0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};

            if(line_count > 0) {
                uint8_t *lines = resolve_tag_data_offset(path, tag_data, tag_data_length, *(uint32_t *)(conversations + 0x5C + 4), line_size * (size_t)line_count);
                if(lines == NULL) {
                    printf("%s: BAD LINES!\n", path);
                    exit(135);
                }
                for(uint32_t line = 0; line < line_count; line++, lines += line_size) {
                    if(*(uint16_t *)(lines + 0x2) != participant) {
                        continue;
                    }
                    // this can match multiple participants, which can lead to ambiguous variant numbers; we'll just do the last one
                    for(size_t i = 0; i < 6; i++) {
                        uint32_t variant_id = *(uint32_t *)(lines + 0x1C + 0xC + 0x10 * i);
                        if(variant_id != 0xFFFFFFFF) {
                            const char *tag_path = get_tag_path(path, tag_data, tag_data_length, variant_id);

                            #define IF_THING_IS_CONTAINED_IN_THE_THING(thing, number) \
                                if(strstr(tag_path, thing) != NULL) { meme_indices[i] = number; } \

                            // code is from invader
                            IF_THING_IS_CONTAINED_IN_THE_THING("bisenti", 2)
                            else IF_THING_IS_CONTAINED_IN_THE_THING("fitzgerald", 4)
                            else IF_THING_IS_CONTAINED_IN_THE_THING("jenkins", 4)
                            else IF_THING_IS_CONTAINED_IN_THE_THING("aussie", 5)
                            else IF_THING_IS_CONTAINED_IN_THE_THING("mendoza", 6)
                            else IF_THING_IS_CONTAINED_IN_THE_THING("sarge2", 101)
                            else IF_THING_IS_CONTAINED_IN_THE_THING("sarge", 100)
                            else IF_THING_IS_CONTAINED_IN_THE_THING("johnson", 100)
                            else IF_THING_IS_CONTAINED_IN_THE_THING("lehto", 101)
                            else {
                                meme_indices[i] = 0;
                            }
                        }
                    }
                }
            }

            for(size_t i = 0; i < 6; i++) {
                if(variant_number[i] != meme_indices[i]) {
                    did_something = true;
                    variant_number[i] = meme_indices[i];
                }
            }
        }
    }
    return did_something;
}

static bool fix_weapon_hud_interface(const char *path, uint8_t *tag_data, size_t tag_data_length, uint8_t *weapon_hud_interface_data) {
    if(!weapon_hud_interface_data) {
        return false;
    }
    uint32_t *crosshair_types = (uint32_t *)(weapon_hud_interface_data + 0x9C);
    if(*crosshair_types & 2) {
        // zoom overlay bit is already set
        return false;
    }

    uint32_t crosshairs_size = 104;
    uint32_t crosshairs_count = *(uint32_t *)(weapon_hud_interface_data + 0x84);
    if(crosshairs_count == 0) {
        return false;
    }

    uint8_t *crosshairs = resolve_tag_data_offset(path, tag_data, tag_data_length, *(uint32_t *)(weapon_hud_interface_data + 0x84 + 4), crosshairs_size * (size_t)crosshairs_count);
    if(crosshairs == NULL) {
        printf("%s: BAD CROSSHAIRS!\n", path);
        exit(135);
    }
    for(uint32_t crosshair = 0; crosshair < crosshairs_count; crosshair++, crosshairs += crosshairs_size) {
        uint32_t crosshair_overlays_size = 108;
        uint32_t crosshair_overlays_count = *(uint32_t *)(crosshairs + 0x34);
        if(crosshair_overlays_count == 0) {
            continue;
        }

        uint8_t *crosshair_overlays = resolve_tag_data_offset(path, tag_data, tag_data_length, *(uint32_t *)(crosshairs + 0x34 + 4), crosshair_overlays_size * (size_t)crosshair_overlays_count);
        if(crosshair_overlays == NULL) {
            printf("%s: BAD CROSSHAIR OVERLAYS!\n", path);
            exit(135);
        }
        for(uint32_t crosshair_overlay = 0; crosshair_overlay < crosshair_overlays_count; crosshair_overlay++, crosshair_overlays += crosshair_overlays_size) {
            uint32_t flags = *(uint32_t *)(crosshair_overlays + 0x48);
            // if `show only when zoomed`, `one zoom level`, or ` don't show when zoomed` is set, set the flag for having a `zoom overlay` crosshair type
            if(flags & 4  ||
               flags & 32 ||
               flags & 64) {
                *crosshair_types |= 2;
                return true;
            }
        }
    }
    return false;
}

const uint32_t BASE_MEMORY_ADDRESS = 0x40440000;
static void *resolve_tag_data_offset(
    const char *path,
    uint8_t *tag_data,
    size_t tag_data_length,
    uint32_t address,
    size_t needed_length
) {
    if(needed_length == 0) {
        fprintf(stderr, "%s: Invalid needed length (0)\n", path);
        return NULL;
    }
    if(address < BASE_MEMORY_ADDRESS) {
        fprintf(stderr, "%s: Invalid address: underflows\n", path);
        return NULL;
    }

    size_t offset = (size_t)(address - BASE_MEMORY_ADDRESS);
    if(offset > tag_data_length) {
        fprintf(stderr, "%s: Invalid address: out-of-bounds starting address\n", path);
        return NULL;
    }
    if((uint64_t)offset + (uint64_t)needed_length > (uint64_t)tag_data_length) {
        fprintf(stderr, "%s: Invalid address: out-of-bounds ending address\n", path);
        return NULL;
    }

    return tag_data + offset;
}

typedef struct BSP {
    uint32_t file_offset;
    uint32_t file_size;
    char lol[24];
} BSP;

static uint32_t crc32(uint32_t crc, const void *buf, size_t size);

static bool fix_crc32(
    const char *path,
    uint8_t *map_data,
    size_t map_data_length,
    uint8_t *tag_data,
    size_t tag_data_length
) {
    size_t scenario_tag_index = (size_t)(*(uint32_t *)(tag_data + 0x4) & 0xFFFF);
    Tag *scenario_tag = resolve_tag_data_offset(path, tag_data, tag_data_length, *(uint32_t *)tag_data + sizeof(*scenario_tag) * scenario_tag_index, sizeof(*scenario_tag));
    if(!scenario_tag) {
        return false;
    }

    uint8_t *scenario_tag_data = resolve_tag_data_offset(path, tag_data, tag_data_length, scenario_tag->tag_data_addr, 0x5A8);
    if(!scenario_tag_data) {
        return false;
    }

    size_t bsp_count = (size_t)*(uint32_t *)(scenario_tag_data + 0x5A4);
    BSP *bsps = resolve_tag_data_offset(path, tag_data, tag_data_length, *(uint32_t *)(scenario_tag_data + 0x5A8), sizeof(*bsps) * bsp_count);

    uint32_t crc = 0xFFFFFFFF;
    for(size_t i = 0; i < bsp_count; i++) {
        BSP *bsp = bsps + i;
        if(bsp->file_offset > map_data_length || (uint64_t)bsp->file_offset + (uint64_t)bsp->file_size > map_data_length) {
            fprintf(stderr, "%s: Invalid BSP length/offset\n", path);
            return false;
        }
        crc = crc32(crc, map_data + bsp->file_offset, bsp->file_size);
    }

    size_t model_data_offset = *(uint32_t *)(tag_data + 0x14);
    size_t model_data_size = *(uint32_t *)(tag_data + 0x20);

    if(model_data_offset > map_data_length || (uint64_t)model_data_offset + (uint64_t)model_data_size > map_data_length) {
        fprintf(stderr, "%s: Invalid model data length/offset\n", path);
        return false;
    }
    crc = crc32(crc, map_data + model_data_offset, model_data_size);
    crc = crc32(crc, tag_data, tag_data_length);

    *(uint32_t *)(map_data + 0x64) = crc;

    return true;
}


/*-
 *  COPYRIGHT (C) 1986 Gary S. Brown.  You may use this program, or
 *  code or tables extracted from it, as desired without restriction.
 *
 *  First, the polynomial itself and its table of feedback terms.  The
 *  polynomial is
 *  X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0
 *
 *  Note that we take it "backwards" and put the highest-order term in
 *  the lowest-order bit.  The X^32 term is "implied"; the LSB is the
 *  X^31 term, etc.  The X^0 term (usually shown as "+1") results in
 *  the MSB being 1
 *
 *  Note that the usual hardware shift register implementation, which
 *  is what we're using (we're merely optimizing it by doing eight-bit
 *  chunks at a time) shifts bits into the lowest-order term.  In our
 *  implementation, that means shifting towards the right.  Why do we
 *  do it this way?  Because the calculated CRC must be transmitted in
 *  order from highest-order term to lowest-order term.  UARTs transmit
 *  characters in order from LSB to MSB.  By storing the CRC this way
 *  we hand it to the UART in the order low-byte to high-byte; the UART
 *  sends each low-bit to hight-bit; and the result is transmission bit
 *  by bit from highest- to lowest-order term without requiring any bit
 *  shuffling on our part.  Reception works similarly
 *
 *  The feedback terms table consists of 256, 32-bit entries.  Notes
 *
 *      The table can be generated at runtime if desired; code to do so
 *      is shown later.  It might not be obvious, but the feedback
 *      terms simply represent the results of eight shift/xor opera
 *      tions for all combinations of data and CRC register values
 *
 *      The values must be right-shifted by eight bits by the "updcrc
 *      logic; the shift must be unsigned (bring in zeroes).  On some
 *      hardware you could probably optimize the shift in assembler by
 *      using byte-swap instructions
 *      polynomial $edb88320
 *
 *
 * CRC32 code derived from work by Gary S. Brown.
 */

 #include <stdint.h>
 #include <stddef.h>

 static uint32_t crc32_tab[] = {
     0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
     0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
     0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
     0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
     0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
     0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
     0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
     0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
     0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
     0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
     0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
     0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
     0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
     0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
     0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
     0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
     0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
     0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
     0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
     0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
     0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
     0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
     0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
     0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
     0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
     0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
     0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
     0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
     0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
     0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
     0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
     0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
     0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
     0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
     0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
     0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
     0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
     0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
     0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
     0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
     0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
     0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
     0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
 };

 static uint32_t crc32(uint32_t crc, const void *buf, size_t size)
 {
     const uint8_t *p;

     p = buf;
    // Halo does not do this step with CRC32
    //  crc = crc ^ ~0U;

     while (size--)
         crc = crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);

    // Halo does not do this step with CRC32
    //  return crc ^ ~0U;
    return crc;
 }