#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"

enum {
    HUD_FLASH_FLAGS_REVERSE_COLORS_BIT,
    NUMBER_OF_HUD_FLASH_FLAGS,
};

enum {
    HUD_SCALE_FLAGS_DONT_SCALE_OFFSET_BIT,
    HUD_SCALE_FLAGS_DONT_SCALE_SIZE_BIT,
    NUMBER_OF_HUD_SCALE_FLAGS
};

enum {
    HUD_METER_FLAGS_SWITCH_COLOR_ON_STATE_CHANGE,
    HUD_METER_FLAGS_INTERPOLATES_BETWEEN_MIN_MAX_BIT,
    HUD_METER_FLAGS_INTERPOLATE_IN_HSV_SPACE_BIT,
    HUD_METER_FLAGS_INTERPOLATE_ALONG_FARTHEST_HUE_PATH_BIT,
    HUD_METER_FLAGS_INVERT_INTERPOLATION_VALUE_BIT,
    NUMBER_OF_HUD_METER_FLAGS,
};

enum {
    HUD_ICON_FLAGS_USE_TEXT_BIT,
    HUD_ICON_FLAGS_USE_COLOR_BIT,
    HUD_ICON_FLAGS_ABSOLUTE_WIDTH_BIT,
    NUMBER_OF_ICON_FLAGS,
};

enum {
    HUD_NUMBER_FLAGS_SHOW_ALL_LEADING_ZEROS_BIT,
    HUD_NUMBER_FLAGS_SHOW_ONLY_WHEN_ZOOMED,
    HUD_NUMBER_FLAGS_SHOW_TRAILING_M,
    NUMBER_OF_HUD_NUMBER_FLAGS
};

enum {
    HUD_SCREEN_EFFECT_MASK_FLAGS_ONLY_WHEN_ZOOMED_BIT,
    NUMBER_OF_HUD_SCREEN_EFFECT_MASK_FLAGS
};

enum {
    HUD_SCREEN_EFFECT_CONVOLUTION_FLAGS_ONLY_WHEN_ZOOMED_BIT,
    NUMBER_OF_HUD_SCREEN_EFFECT_CONVOLUTION_FLAGS
};

enum {
    HUD_SCREEN_EFFECT_LIGHT_ENHANCEMENT_FLAGS_ONLY_WHEN_ZOOMED_BIT,
    HUD_SCREEN_EFFECT_LIGHT_ENHANCEMENT_FLAGS_CONNECT_TO_FLASHLIGHT_BIT,
    HUD_SCREEN_EFFECT_LIGHT_ENHANCEMENT_FLAGS_USES_CONVOLUTION_MASK_BIT,
    NUMBER_OF_HUD_SCREEN_EFFECT_LIGHT_ENHANCEMENT_FLAGS
};

enum {
    HUD_SCREEN_EFFECT_DESATURATION_FLAGS_ONLY_WHEN_ZOOMED_BIT,
    HUD_SCREEN_EFFECT_DESATURATION_FLAGS_CONNECT_TO_FLASHLIGHT_BIT,
    HUD_SCREEN_EFFECT_DESATURATION_FLAGS_IS_ADDITIVE_BIT,
    HUD_SCREEN_EFFECT_DESATURATION_FLAGS_USES_CONVOLUTION_MASK_BIT,
    NUMBER_OF_HUD_SCREEN_EFFECT_DESATURATION_FLAGS
};

enum {
    HUD_ANCHOR_TOP_LEFT,
    HUD_ANCHOR_TOP_RIGHT,
    HUD_ANCHOR_BOTTOM_LEFT,
    HUD_ANCHOR_BOTTOM_RIGHT,
    HUD_ANCHOR_CENTER,
    HUD_ANCHOR_TOP_CENTER,
    HUD_ANCHOR_BOTTOM_CENTER,
    HUD_ANCHOR_LEFT_CENTER,
    HUD_ANCHOR_RIGHT_CENTER,
    NUMBER_OF_HUD_ANCHORS
};

enum {
    HUD_CHILD_ANCHOR_FROM_PARENT,
    HUD_CHILD_ANCHOR_TOP_LEFT,
    HUD_CHILD_ANCHOR_TOP_RIGHT,
    HUD_CHILD_ANCHOR_BOTTOM_LEFT,
    HUD_CHILD_ANCHOR_BOTTOM_RIGHT,
    HUD_CHILD_ANCHOR_CENTER,
    HUD_CHILD_ANCHOR_TOP_CENTER,
    HUD_CHILD_ANCHOR_BOTTOM_CENTER,
    HUD_CHILD_ANCHOR_LEFT_CENTER,
    HUD_CHILD_ANCHOR_RIGHT_CENTER,
    NUMBER_OF_HUD_CHILD_ANCHORS
};

enum {
    HUD_CANVAS_SIZE_480P,
    HUD_CANVAS_SIZE_960P,
    NUMBER_OF_HUD_CANVAS_SIZES
};

enum {
    HUD_USE_ON_MAP_TYPE_ANY,
    HUD_USE_ON_MAP_TYPE_SOLO,
    HUD_USE_ON_MAP_TYPE_MULTIPLAYER,
    NUMBER_OF_HUD_USE_ON_MAP_TYPES,
};

enum {
    HUD_BLIP_TYPE_MEDIUM,
    HUD_BLIP_TYPE_SMALL,
    HUD_BLIP_TYPE_LARGE,
    NUMBER_OF_HUD_BLIP_TYPES
};

enum {
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_TYPE_TINT,
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_TYPE_HORIZONTAL_OFFSET,
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_TYPE_VERTICAL_OFFSET,
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_TYPE_ALPHA,
    NUMBER_OF_HUD_MULTITEXTURE_OVERLAY_EFFECTOR_TYPES
};

enum {
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_DESTINATION_GEOMETRY_OFFSET,
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_DESTINATION_PRIMARY_MAP,
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_DESTINATION_SECONDARY_MAP,
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_DESTINATION_TERTIARY_MAP,
    NUMBER_OF_HUD_MULTITEXTURE_OVERLAY_EFFECTOR_DESTINATIONS
};

enum {
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_SOURCE_PLAYER_PITCH,
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_SOURCE_PLAYER_PITCH_TANGENT,
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_SOURCE_PLAYER_YAW,
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_SOURCE_WEAPON_AMMO_LOADED,
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_SOURCE_WEAPON_AMMO_TOTAL,
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_SOURCE_WEAPON_HEAT,
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_SOURCE_EXPLICIT,
    HUD_MULTITEXTURE_OVERLAY_EFFECTOR_SOURCE_ZOOM_LEVEL,
    NUMBER_OF_HUD_MULTITEXTURE_OVERLAY_EFFECTOR_SOURCES
};

enum {
    HUD_MULTITEXTURE_OVERLAY_HUD_ELEMENT_BLEND_FUNCTION_ADD,
    HUD_MULTITEXTURE_OVERLAY_HUD_ELEMENT_BLEND_FUNCTION_SUBTRACT,
    HUD_MULTITEXTURE_OVERLAY_HUD_ELEMENT_BLEND_FUNCTION_MULTIPLY,
    HUD_MULTITEXTURE_OVERLAY_HUD_ELEMENT_BLEND_FUNCTION_MULTIPLY2X,
    HUD_MULTITEXTURE_OVERLAY_HUD_ELEMENT_BLEND_FUNCTION_DOT,
    NUMBER_OF_HUD_MULTITEXTURE_OVERLAY_BLEND_MODES
};

enum {
    HUD_MULTITEXTURE_OVERLAY_HUD_ELEMENT_ORIGIN_TEXTURE,
    HUD_MULTITEXTURE_OVERLAY_HUD_ELEMENT_ORIGIN_SCREEN,
    NUMBER_OF_MULTITEXTURE_ANCHORS
};

enum {
    HUD_MULTITEXTURE_OVERLAY_HUD_ELEMENT_ADDRESS_CLAMP,
    HUD_MULTITEXTURE_OVERLAY_HUD_ELEMENT_ADDRESS_WRAP,
    NUMBER_OF_MULTITEXTURE_ADDRESSES
};

struct hud_absolute_placement {
    uint16_t anchor;
    uint16_t canvas_size;
    uint32_t unused[8];
};
static_assert(sizeof(struct hud_absolute_placement) == 36);

struct hud_placement {
    point2d offset;
    float_vector2d scale;
    uint16_t multiplayer_scaling_flags;
    uint16_t pad;
    uint32_t unused0[5];
};
static_assert(sizeof(struct hud_placement) == 36);

struct hud_objective {
    int16_t up_ticks;
    int16_t fade_ticks;
};
static_assert(sizeof(struct hud_objective) == 4);

struct hud_color {
    Pixel32 color;
    Pixel32 flash_color;
    float flash_period;
    float flash_delay;
    int16_t number_of_flashes;
    uint16_t flash_flags;
    float flash_length;
    Pixel32 disabled_color;
    struct hud_objective objective;
};
static_assert(sizeof(struct hud_color) == 32);

struct hud_static_element {
    struct hud_placement placement;
    struct tag_reference interface_bitmap;
    struct hud_color colors;
    int16_t sequence_index;
    uint16_t pad;
    struct tag_reflexive multitexture_overlays;
    uint32_t unused0[1];
};
static_assert(sizeof(struct hud_static_element) == 104);

struct hud_meter_element {
    struct hud_placement placement;
    struct tag_reference meter_bitmap;
    Pixel32 min_color;
    Pixel32 max_color;
    Pixel32 flash_color;
    Pixel32 empty_color;
    uint8_t meter_flags;
    uint8_t minimum_value;
    int16_t sequence_index;
    uint8_t alpha_multiplier;
    uint8_t alpha_bias;
    int16_t value_scale;
    float opacity;
    float fade;
    Pixel32 disabled_color;
    float min_alpha;
    uint32_t unused0[3];
};
static_assert(sizeof(struct hud_meter_element) == 104);

struct hud_number_element {
    struct hud_placement placement;
    struct hud_color colors;
    int8_t digits;
    uint8_t number_flags;
    int8_t fractional_digits;
    uint8_t pad;
    uint32_t unused1[3];
};
static_assert(sizeof(struct hud_number_element) == 84);

struct hud_icon_element {
    int16_t sequence_index;
    int16_t width_offset;
    point2d offset;
    Pixel32 color;
    int8_t frame_rate;
    uint8_t flags;
    int16_t text_index;
};
static_assert(sizeof(struct hud_icon_element) == 16);

struct hud_screen_effect {
    uint32_t unused1;
    uint16_t mask_flags;
    uint16_t mask_pad;
    uint32_t mask_unused[4];
    struct tag_reference mask_fullscreen;
    struct tag_reference mask_splitscreen;
    uint32_t unused2[2];
    uint16_t convolution_flags;
    uint16_t convolution_pad;
    float_bounds convolution_radius_in_bounds;
    float_bounds convolution_radius_out_bounds;
    uint32_t unused3[6];
    uint16_t light_enhancement_flags;
    int16_t light_enhancement_script_source;
    float light_enhancement_intensity;
    uint32_t unused4[6];
    uint16_t desaturation_flags;
    int16_t desaturation_script_source;
    float desaturation_intensity;
    float_rgb_color desaturation_tint;
    uint32_t unused5[6];
};
static_assert(sizeof(struct hud_screen_effect) == 184);

struct hud_multitexture_overlay_element_effector {
    uint32_t unused1[16];
    uint16_t destination_type;
    uint16_t destination;
    uint16_t source;
    uint16_t pad1;
    float_bounds in_bounds;
    float_bounds out_bounds;
    uint32_t unused2[16];
    float_rgb_color_bounds tint_color_bounds;
    uint16_t periodic_function;
    uint16_t pad2;
    float periodic_function_period;
    float periodic_function_phase;
    uint32_t unused3[8];
};
static_assert(sizeof(struct hud_multitexture_overlay_element_effector) == 220);

struct hud_multitexture_overlay_element {
    uint16_t flags;
    uint16_t type;
    uint16_t framebuffer_blend_function;
    uint16_t pad1;
    uint32_t unused1[8];
    uint16_t map_flags[3];
    uint16_t map_blending_function[2];
    uint16_t pad2;
    float_vector2d map_scale[3];
    float_vector2d map_offset[3];
    struct tag_reference map[3];
    uint16_t map_clamp[3];
    uint16_t pad3;
    uint32_t unused2[46];
    struct tag_reflexive effectors;
    uint32_t unused3[32];
};
static_assert(sizeof(struct hud_multitexture_overlay_element) == 480);

struct hud_messaging_parameters {
    struct hud_absolute_placement absolute_placement;
    struct hud_placement placement;
    struct tag_reference single_player_font;
    struct tag_reference multi_player_font;
    float up_time;
    float fade_time;
    float_argb_color state_color;
    float_argb_color text_color;
    float spacing;
    struct tag_reference hud_item_messages;
    struct tag_reference messaging_icons;
    struct tag_reference alternate_icon_text;
    struct tag_reflexive button_icons;
    struct hud_color color;
    struct tag_reference hud_messages;
    struct hud_color objective_color;
};
static_assert(sizeof(struct hud_messaging_parameters) == 288);

struct hud_multiplayer_parameters {
    float hud_scale;
    uint32_t unused[64];
};
static_assert(sizeof(struct hud_multiplayer_parameters) == 260);
