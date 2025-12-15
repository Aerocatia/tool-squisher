#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"

enum {
    SOUND_FLAGS_FIT_TO_ADPCM_BLOCK_SIZE_BIT,
    SOUND_FLAGS_LINKED_PERMUTATIONS_BIT,
    SOUND_FLAGS_THIRSTY_GRUNT_BIT,
    NUMBER_OF_SOUND_FLAGS
};

enum {
    SOUND_CLASS_PROJECTILE_IMPACT,
    SOUND_CLASS_PROJECTILE_DETONATION,
    SOUND_CLASS_UNUSED0,
    SOUND_CLASS_UNUSED1,
    SOUND_CLASS_WEAPON_FIRE,
    SOUND_CLASS_WEAPON_READY,
    SOUND_CLASS_WEAPON_RELOAD,
    SOUND_CLASS_WEAPON_EMPTY,
    SOUND_CLASS_WEAPON_CHARGE,
    SOUND_CLASS_WEAPON_OVERHEAT,
    SOUND_CLASS_WEAPON_IDLE,
    SOUND_CLASS_UNUSED2,
    SOUND_CLASS_UNUSED3,
    SOUND_CLASS_OBJECT_IMPACTS,
    SOUND_CLASS_PARTICLE_IMPACTS,
    SOUND_CLASS_SLOW_PARTICLE_IMPACTS,
    SOUND_CLASS_UNUSED4,
    SOUND_CLASS_UNUSED5,
    SOUND_CLASS_UNIT_FOOTSTEPS,
    SOUND_CLASS_UNIT_DIALOG,
    SOUND_CLASS_UNUSED6,
    SOUND_CLASS_UNUSED7,
    SOUND_CLASS_VEHICLE_IMPACT,
    SOUND_CLASS_VEHICLE_ENGINE,
    SOUND_CLASS_UNUSED8,
    SOUND_CLASS_UNUSED9,
    SOUND_CLASS_DEVICE_DOOR,
    SOUND_CLASS_DEVICE_FORCE_FIELD,
    SOUND_CLASS_DEVICE_MACHINERY,
    SOUND_CLASS_DEVICE_NATURE,
    SOUND_CLASS_DEVICE_COMPUTERS,
    SOUND_CLASS_UNUSED10,
    SOUND_CLASS_MUSIC,
    SOUND_CLASS_AMBIENT_NATURE,
    SOUND_CLASS_AMBIENT_MACHINERY,
    SOUND_CLASS_AMBIENT_COMPUTERS,
    SOUND_CLASS_UNUSED11,
    SOUND_CLASS_UNUSED12,
    SOUND_CLASS_UNUSED13,
    SOUND_CLASS_PLAYER_HURT,
    SOUND_CLASS_UNUSED14,
    SOUND_CLASS_UNUSED15,
    SOUND_CLASS_UNUSED16,
    SOUND_CLASS_UNUSED17,
    SOUND_CLASS_SCRIPTED_DIALOG_TO_PLAYER,
    SOUND_CLASS_SCRIPTED_EFFECT,
    SOUND_CLASS_SCRIPTED_DIALOG_TO_OTHER,
    SOUND_CLASS_SCRIPTED_DIALOG_FORCE_UNSPATIALIZED,
    SOUND_CLASS_UNUSED18,
    SOUND_CLASS_UNUSED19,
    SOUND_CLASS_GAME_EVENT,
    NUMBER_OF_SOUND_CLASSES
};

enum {
    SOUND_ENCODING_MONO,
    SOUND_ENCODING_STEREO,
    NUMBER_OF_SOUND_ENCODINGS
};

enum {
    SOUND_COMPRESSION_TYPE_NONE,
    SOUND_COMPRESSION_TYPE_XBOX_ADPCM,
    SOUND_COMPRESSION_TYPE_IMA_ADPCM,
    SOUND_COMPRESSION_TYPE_OGG_VORBIS,
    NUMBER_OF_SOUND_COMPRESSION_TYPES
};

enum {
    SOUND_SAMPLE_RATE_22K,
    SOUND_SAMPLE_RATE_44K,
    NUMBER_OF_SOUND_SAMPLE_RATES
};

#pragma pack(push, 1)

struct sound_permutation {
    String32 name;
    float skip_fraction;
    float gain;
    uint16_t duplicate_compression;
    int16_t next_permutation_index;
    Pointer32 cache_base_address; // moved from the below padding compared to Xbox. On Xbox this was an index
    uint32_t padding;
    TagID cache_tag_index;
    uint32_t buffer_size;
    TagID runtime_tag_index;
    struct tag_data samples;
    struct tag_data mouth_data;
    struct tag_data subtitle_data;
};
static_assert(sizeof(struct sound_permutation) == 124);

struct sound_pitch_range {
    String32 name;
    float natural_pitch;
    float_bounds bend_bounds;
    int16_t actual_permutation_count;
    uint16_t pad;
    float runtime_oo_natural_pitch;
    uint32_t runtime_permutation_flags;
    int16_t runtime_last_permutation_index;
    int16_t runtime_discarded_permutation_index;
    struct tag_reflexive permutations;
};
static_assert(sizeof(struct sound_pitch_range) == 72);

struct sound_scale_modifiers {
    float skip_fraction;
    float gain;
    float pitch;
    uint32_t unused[3];
};
static_assert(sizeof(struct sound_scale_modifiers) == 24);

struct sound {
    uint32_t flags;
    uint16_t sound_class;
    uint16_t sample_rate;
    float minimum_distance;
    float maximum_distance;
    float skip_fraction;
    float_bounds pitch_bounds;
    float inner_cone_angle;
    float outer_cone_angle;
    float outer_cone_gain;
    float gain;
    float maximum_bend;
    uint32_t unused[3];
    struct sound_scale_modifiers scale_lower_bound;
    struct sound_scale_modifiers scale_upper_bound;
    uint16_t encoding;
    uint16_t compression;
    struct tag_reference promotion_sound;
    int16_t promotion_count;
    uint16_t pad;
    uint32_t runtime_maximum_play_time;
    uint32_t runtime_promotion_counter;
    uint32_t runtime_promotion_time;
    uint32_t runtime_scripting_time;
    HaloID runtime_scripting_sound_index;
    struct tag_reflexive pitch_ranges;
};
static_assert(sizeof(struct sound) == 164);

#pragma pack(pop)

bool sound_final_postprocess(TagID tag, struct tag_data_instance *tag_data);
