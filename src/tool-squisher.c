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
 * ## New Enum Squishing
 *
 * Fixes enums added to Halo's definitions after the release of Halo PC and its
 * tools (e.g. CEA Unit metagame data and MCC's extra HUD anchors) that have
 * been corrupted in maps compiled by older versions of tool.exe. When an older
 * version of tool.exe is used with tags containing this data, the undefined
 * big-endian field data will be directly copied into the map as is. The fix is
 * to flip the endianness and check if the flipped value is within the bounds
 * for the given field. This is safe since corrupt fields will always be largely
 * out of bounds and can never be ambiguously considered valid.
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

#include "crc/crc.h"

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
const uint32_t OBJECT_GROUP_FOURCC = 0x6F626A65;
const uint32_t SCENARIO_GROUP_FOURCC = 0x73636E72;
const uint32_t SHADER_GROUP_FOURCC = 0x73686472;
const uint32_t SOUND_GROUP_FOURCC = 0x736E6421;
const uint32_t UNIT_GROUP_FOURCC = 0x756E6974;
const uint32_t WEAPON_HUD_INTERFACE_GROUP_FOURCC = 0x77706869;

static bool fix_shader_struct(uint8_t *shader_data);
static bool fix_scenario(const char *path, uint8_t *tag_data, size_t tag_data_length, uint8_t *scenario_data);
static bool fix_lens_flare(uint8_t *lens_data);
static bool fix_sound(uint8_t *sound_data);
static bool fix_unit(uint8_t *sound_data);
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

        if(tag->secondary == UNIT_GROUP_FOURCC && tag->tertiary  == OBJECT_GROUP_FOURCC) {
            if(fix_unit(resolve_tag_data_offset(path, tag_data, tag_data_length, tag->tag_data_addr, 0x2A0))) {
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

static bool make_enum16_valid(uint16_t *value, uint16_t max_value) {
    if(!value) {
        return false;
    }
    if(*value > max_value) {
        uint16_t value_swapped = (*value>>8) | (*value<<8);
        if(value_swapped <= max_value) {
            *value = value_swapped;
        }
        else {
            *value = 0;
        }
        return true;
    }
    return false;
}

static bool fix_unit(uint8_t *unit_data) {
    if(!unit_data) {
        return false;
    }
    bool changes_made = false;
    uint16_t *metagame_type = (uint16_t *)(unit_data + 0x29C);
    if(make_enum16_valid(metagame_type, 43)) {
        changes_made = true;
    }

    uint16_t *metagame_class = (uint16_t *)(unit_data + 0x29E);
    if(make_enum16_valid(metagame_class, 7)) {
        changes_made = true;
    }
    return changes_made;
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

    bool changes_made = false;

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
                    changes_made = true;
                    variant_number[i] = meme_indices[i];
                }
            }
        }
    }
    return changes_made;
}

static bool fix_anchors_in_reflexive(const char *path, uint8_t *tag_data, size_t tag_data_length, uint32_t offset, uint32_t size, uint32_t count) {
    if(count == 0) {
        return false;
    }
    bool changes_made = false;
    uint8_t *elements = resolve_tag_data_offset(path, tag_data, tag_data_length, offset, size * (size_t)count);
    if(elements == NULL) {
        printf("%s: BAD WEAPON HUD INTERFACE REFLEXIVE!\n", path);
        exit(135);
    }
    for(uint32_t element = 0; element < count; element++, elements += size) {
        uint16_t *anchor = (uint16_t *)(elements + 0x6);
        changes_made = make_enum16_valid(anchor, 9);
    }
    return changes_made;
}

static bool fix_weapon_hud_interface(const char *path, uint8_t *tag_data, size_t tag_data_length, uint8_t *weapon_hud_interface_data) {
    if(!weapon_hud_interface_data) {
        return false;
    }
    bool changes_made = false;
    // Static Elements
    if(fix_anchors_in_reflexive(path, tag_data, tag_data_length, *(uint32_t *)(weapon_hud_interface_data + 0x60 + 4), 180, *(uint32_t *)(weapon_hud_interface_data + 0x60))) {
        changes_made = true;
    }

    // Meter Elements
    if(fix_anchors_in_reflexive(path, tag_data, tag_data_length, *(uint32_t *)(weapon_hud_interface_data + 0x6C + 4), 180, *(uint32_t *)(weapon_hud_interface_data + 0x6C))) {
        changes_made = true;
    }

    // Number Elements
    if(fix_anchors_in_reflexive(path, tag_data, tag_data_length, *(uint32_t *)(weapon_hud_interface_data + 0x78 + 4), 160, *(uint32_t *)(weapon_hud_interface_data + 0x78))) {
        changes_made = true;
    }

    // Overlay Elements
    if(fix_anchors_in_reflexive(path, tag_data, tag_data_length, *(uint32_t *)(weapon_hud_interface_data + 0x90 + 4), 104, *(uint32_t *)(weapon_hud_interface_data + 0x90))) {
        changes_made = true;
    }

    uint32_t *crosshair_types = (uint32_t *)(weapon_hud_interface_data + 0x9C);
    if(*crosshair_types & 2) {
        // zoom overlay bit is already set and nothing more to do
        return changes_made;
    }

    uint32_t crosshairs_size = 104;
    uint32_t crosshairs_count = *(uint32_t *)(weapon_hud_interface_data + 0x84);
    if(crosshairs_count == 0) {
        return changes_made;
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
    return changes_made;
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

    uint32_t crc;
    crc_new(&crc);
    for(size_t i = 0; i < bsp_count; i++) {
        BSP *bsp = bsps + i;
        if(bsp->file_offset > map_data_length || (uint64_t)bsp->file_offset + (uint64_t)bsp->file_size > map_data_length) {
            fprintf(stderr, "%s: Invalid BSP length/offset\n", path);
            return false;
        }
        crc_checksum_buffer(&crc, map_data + bsp->file_offset, bsp->file_size);
    }

    size_t model_data_offset = *(uint32_t *)(tag_data + 0x14);
    size_t model_data_size = *(uint32_t *)(tag_data + 0x20);

    if(model_data_offset > map_data_length || (uint64_t)model_data_offset + (uint64_t)model_data_size > map_data_length) {
        fprintf(stderr, "%s: Invalid model data length/offset\n", path);
        return false;
    }
    crc_checksum_buffer(&crc, map_data + model_data_offset, model_data_size);
    crc_checksum_buffer(&crc, tag_data, tag_data_length);

    *(uint32_t *)(map_data + 0x64) = crc;

    return true;
}
