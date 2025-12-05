#include <stdint.h>
#include <stdio.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"

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

bool sound_final_postprocess(TagID tag, struct tag_data_instance *tag_data) {
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

    return true;
}
