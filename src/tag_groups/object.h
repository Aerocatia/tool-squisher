#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"

enum {
    OBJECT_FLAGS_DOES_NOT_CAST_SHADOW_BIT,
    OBJECT_FLAGS_TRANSPARENCY_SELF_OCCLUDES_BIT,
    OBJECT_FLAGS_ARTIFICIALLY_BRIGHT_BIT,
    OBJECT_FLAGS_NOT_PATHFINDING_OBSTACLE_BIT,
    NUMBER_OF_OBJECT_FLAGS
};

enum {
    OBJECT_RUNTIME_FLAGS_SCALED_CHANGE_COLORS_BIT,
    NUMBER_OF_OBJECT_RUNTIME_FLAGS
};

enum {
    OBJECT_FUNCTION_FLAGS_INVERT_FUNCTION_BIT,
    OBJECT_FUNCTION_FLAGS_ADDITIVE_BIT,
    OBJECT_FUNCTION_FLAGS_DOES_NOT_DEACTIVATE_BELOW_LOWER_BOUND_BIT,
    NUMBER_OF_OBJECT_FUNCTION_FLAGS
};

enum {
    OBJECT_FUNCTION_REFERENCE_NONE,
    OBJECT_FUNCTION_REFERENCE_A,
    OBJECT_FUNCTION_REFERENCE_B,
    OBJECT_FUNCTION_REFERENCE_C,
    OBJECT_FUNCTION_REFERENCE_D,
    NUMBER_OF_OBJECT_FUNCTION_REFERENCES
};

enum {
    OBJECT_FUNCTION_BOUNDS_MODE_CLIP_TO_BOUNDS,
    OBJECT_FUNCTION_BOUNDS_MODE_CLIP_TO_BOUNDS_AND_NORMALIZE,
    OBJECT_FUNCTION_BOUNDS_MODE_SCALE_TO_FIT_BOUNDS,
    NUMBER_OF_OBJECT_FUNCTION_BOUNDS_MODES
};

enum {
    OBJECT_FUNCTION_SCALE_BY_NONE,
    OBJECT_FUNCTION_SCALE_BY_A_IN,
    OBJECT_FUNCTION_SCALE_BY_B_IN,
    OBJECT_FUNCTION_SCALE_BY_C_IN,
    OBJECT_FUNCTION_SCALE_BY_D_IN,
    OBJECT_FUNCTION_SCALE_BY_A_OUT,
    OBJECT_FUNCTION_SCALE_BY_B_OUT,
    OBJECT_FUNCTION_SCALE_BY_C_OUT,
    OBJECT_FUNCTION_SCALE_BY_D_OUT,
    NUMBER_OF_OBJECT_FUNCTION_SCALE_BYS
};

enum {
    OBJECT_CHANGE_COLOR_A,
    OBJECT_CHANGE_COLOR_B,
    OBJECT_CHANGE_COLOR_C,
    OBJECT_CHANGE_COLOR_D,
    NUMBER_OF_OBJECT_CHANGE_COLORS
};

enum {
    OBJECT_CHANGE_COLOR_REFERENCE_NONE,
    OBJECT_CHANGE_COLOR_REFERENCE_A,
    OBJECT_CHANGE_COLOR_REFERENCE_B,
    OBJECT_CHANGE_COLOR_REFERENCE_C,
    OBJECT_CHANGE_COLOR_REFERENCE_D,
    NUMBER_OF_OBJECT_CHANGE_COLOR_REFERENCES
};

enum {
    OBJECT_FUNCTION_MODE_NONE,
    OBJECT_FUNCTION_MODE_BODY_VITALITY,
    OBJECT_FUNCTION_MODE_SHIELD_VITALITY,
    OBJECT_FUNCTION_MODE_RECENT_BODY_DAMAGE,
    OBJECT_FUNCTION_MODE_RECENT_SHIELD_DAMAGE,
    OBJECT_FUNCTION_MODE_RANDOM_CONSTANT,
    OBJECT_FUNCTION_MODE_UMBRELLA_SHIELD_VITALITY,
    OBJECT_FUNCTION_MODE_SHIELD_STUN,
    OBJECT_FUNCTION_MODE_RECENT_UMBRELLA_SHIELD_VITALITY,
    OBJECT_FUNCTION_MODE_UMBRELLA_SHIELD_STUN,
    OBJECT_FUNCTION_MODE_REGION_DAMAGE_0,
    OBJECT_FUNCTION_MODE_REGION_DAMAGE_1,
    OBJECT_FUNCTION_MODE_REGION_DAMAGE_2,
    OBJECT_FUNCTION_MODE_REGION_DAMAGE_3,
    OBJECT_FUNCTION_MODE_REGION_DAMAGE_4,
    OBJECT_FUNCTION_MODE_REGION_DAMAGE_5,
    OBJECT_FUNCTION_MODE_REGION_DAMAGE_6,
    OBJECT_FUNCTION_MODE_REGION_DAMAGE_7,
    OBJECT_FUNCTION_MODE_ALIVE,
    OBJECT_FUNCTION_MODE_COMPASS,
    NUMBER_OF_OBJECT_FUNCTION_MODES
};

#pragma pack(push, 1)

struct object_function {
    uint32_t flags;
    float period;
    int16_t scale_period_by_function_index;
    int16_t function_type;
    int16_t scale_function_by_function_index;
    int16_t wobble_function_type;
    float wobble_period;
    float wobble_magnitude;
    float square_wave_threshold;
    int16_t step_count;
    int16_t map_result_to_transition_function;
    int16_t sawtooth_count;
    int16_t add_function_index;
    int16_t scale_result_by_function_index;
    int16_t bounds_mode;
    float_bounds bounds;
    uint32_t pad2;
    uint16_t pad3;
    int16_t turn_off_with_function_index;
    float scale_by;
    uint32_t unused1[63];
    float runtime_reciprocal_bounds_range;
    float runtime_reciprocal_sawtooth_count;
    float runtime_reciprocal_step_count;
    float runtime_one_over_period;
    String32 usage;
};
static_assert(sizeof(struct object_function) == 360);

struct object_attachment {
    struct tag_reference type;
    String32 marker_name;
    int16_t primary_scale_function_reference;
    int16_t secondary_scale_function_reference;
    int16_t change_color_reference;
    uint16_t pad;
    uint32_t unused[4];
};
static_assert(sizeof(struct object_attachment) == 72);

struct object_change_colors_permutation {
    float weight;
    float_rgb_color_bounds color_bounds;
};
static_assert(sizeof(struct object_change_colors_permutation) == 28);

struct object_change_colors {
    int16_t darken_by;
    int16_t scaled_by;
    uint32_t scale_flags;
    float_rgb_color_bounds color_bounds;
    struct tag_reflexive permutations;
};
static_assert(sizeof(struct object_change_colors) == 44);

struct object_widget {
    struct tag_reference type;
    uint32_t unused[4];
};
static_assert(sizeof(struct object_widget) == 32);

struct _object {
    int16_t type;
    uint16_t flags;
    float bounding_radius;
    float_point3d bounding_offset;
    float_point3d origin_offset;
    float acceleration_scale;
    uint32_t runtime_flags;
    struct tag_reference model;
    struct tag_reference animation_graph;
    uint32_t unused3[10];
    struct tag_reference collision_model;
    struct tag_reference physics;
    struct tag_reference modifier_shader;
    struct tag_reference creation_effect;
    uint32_t unused1[21];
    float render_bounding_radius;
    int16_t function_mode_a_in;
    int16_t function_mode_b_in;
    int16_t function_mode_c_in;
    int16_t function_mode_d_in;
    uint32_t unused2[11];
    int16_t icon_text_index;
    int16_t forced_shader_permutation_index;
    struct tag_reflexive attachments;
    struct tag_reflexive widgets;
    struct tag_reflexive functions;
    struct tag_reflexive change_colors;
    struct tag_reflexive predicted_resources;
};
static_assert(sizeof(struct _object) == 380);

struct object {
    struct _object object;
};
static_assert(sizeof(struct object) == 380);

#pragma pack(pop)
