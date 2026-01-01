#include <stdint.h>
#include <stdio.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"
#include "../resources/resources.h"

#include "sound.h"

static float_bounds sound_get_default_distance_values_for_class(uint16_t sound_class) {
    float_bounds defaults;
    switch(sound_class) {
        case SOUND_CLASS_PROJECTILE_IMPACT:
        case SOUND_CLASS_VEHICLE_IMPACT:
        case SOUND_CLASS_VEHICLE_ENGINE:
            defaults.lower = 1.4f;
            defaults.upper = 8.0f;
            break;
        case SOUND_CLASS_PROJECTILE_DETONATION:
            defaults.lower = 8.0f;
            defaults.upper = 120.0f;
            break;
        case SOUND_CLASS_WEAPON_FIRE:
            defaults.lower = 4.0f;
            defaults.upper = 70.0f;
            break;
        case SOUND_CLASS_WEAPON_READY:
        case SOUND_CLASS_WEAPON_RELOAD:
        case SOUND_CLASS_WEAPON_EMPTY:
        case SOUND_CLASS_WEAPON_CHARGE:
        case SOUND_CLASS_WEAPON_OVERHEAT:
        case SOUND_CLASS_WEAPON_IDLE:
            defaults.lower = 1.0f;
            defaults.upper = 9.0f;
            break;
        case SOUND_CLASS_OBJECT_IMPACTS:
        case SOUND_CLASS_PARTICLE_IMPACTS:
        case SOUND_CLASS_SLOW_PARTICLE_IMPACTS:
        case SOUND_CLASS_DEVICE_COMPUTERS:
        case SOUND_CLASS_AMBIENT_COMPUTERS:
        case SOUND_CLASS_PLAYER_HURT:
            defaults.lower = 0.5f;
            defaults.upper = 3.0f;
            break;
        case SOUND_CLASS_UNIT_FOOTSTEPS:
            defaults.lower = 0.9f;
            defaults.upper = 10.0f;
            break;
        case SOUND_CLASS_UNIT_DIALOG:
        case SOUND_CLASS_SCRIPTED_DIALOG_TO_PLAYER:
        case SOUND_CLASS_SCRIPTED_DIALOG_TO_OTHER:
        case SOUND_CLASS_SCRIPTED_DIALOG_FORCE_UNSPATIALIZED:
        case SOUND_CLASS_GAME_EVENT:
            defaults.lower = 3.0f;
            defaults.upper = 20.0f;
            break;
        case SOUND_CLASS_DEVICE_DOOR:
        case SOUND_CLASS_DEVICE_FORCE_FIELD:
        case SOUND_CLASS_DEVICE_MACHINERY:
        case SOUND_CLASS_DEVICE_NATURE:
        case SOUND_CLASS_MUSIC:
        case SOUND_CLASS_AMBIENT_NATURE:
        case SOUND_CLASS_AMBIENT_MACHINERY:
            defaults.lower = 0.9f;
            defaults.upper = 5.0f;
            break;
        case SOUND_CLASS_SCRIPTED_EFFECT:
            defaults.lower = 2.0f;
            defaults.upper = 5.0f;
            break;
       default:
           defaults.lower = 0.0f;
           defaults.upper = 0.0f;
    }

    return defaults;
}

bool sound_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct sound *sound = tag_get(tag, TAG_FOURCC_SOUND, tag_data);
    if(!sound) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n", tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SOUND));
        return false;
    }

    // Default these
    float_bounds defaults = sound_get_default_distance_values_for_class(sound->sound_class);
    if(sound->minimum_distance <= 0.0f) {
        sound->minimum_distance = defaults.lower;
    }
    if(sound->maximum_distance <= 0.0f) {
        sound->maximum_distance = defaults.upper;
    }

    // Nothing more to do?
    if(!tag_data->indexed_external_tags || tag_data->tags[tag.index].external) {
        return true;
    }

    // Fix maps using direct sounds.map data offsets if they should be using external tags
    // This happens due to an oversight in tool.exe when checking if tags are the same as the ones in the resource maps or not
    const char *tag_path = tag_path_get(tag, tag_data);
    bool make_external = false;
    for(size_t pr = 0; pr < sound->pitch_ranges.count; pr++) {
        struct sound_pitch_range *pitch_range = sound_get_pitch_range(sound, pr, tag_data);
        if(!pitch_range) {
            fprintf(stderr,"sound pitch range %zu in \"%s.%s\" is out of bounds\n", pr, tag_path, tag_fourcc_to_extension(TAG_FOURCC_SOUND));
            return false;
        }

        for(size_t p = 0; p < pitch_range->permutations.count; p++) {
            struct sound_permutation *permutation = sound_get_permutation(pitch_range, p, tag_data);
            if(!permutation) {
                fprintf(stderr,"sound permutation %zu of pitch range %zu in \"%s.%s\" is out of bounds\n", p, pr, tag_path, tag_fourcc_to_extension(TAG_FOURCC_SOUND));
                return false;
            }

            if(!permutation->samples.external) {
                continue;
            }

            if(!resources_sound_is_in_sounds_map(tag_path)) {
                fprintf(stderr,"sound \"%s\" has external sound sample offsets but does not map to the stock sounds.map by tag path\nThe map should be rebuilt\n", tag_path);
                return false;
            }

            make_external = true;
            goto done;
        }
    }
    done:

    // Please no ear destruction
    if(make_external) {
        // Make the base struct look as if tool.exe had made it external
        sound->sample_rate = SOUND_SAMPLE_RATE_22K;
        sound->encoding = SOUND_ENCODING_MONO;
        sound->compression = SOUND_COMPRESSION_TYPE_NONE;
        sound->runtime_maximum_play_time = 0;
        sound->pitch_ranges.address = 0; // but not the count?
        tag_data->tags[tag.index].external = 1;
        fprintf(stderr,"sound \"%s\" had external sound sample offsets and was changed to lookup tag data from sounds.map by tag path\n", tag_path);
    }

    return true;
}
