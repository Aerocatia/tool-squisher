#pragma once

#include <stdint.h>
#include "../data_types.h"

enum {
    RASTERIZER_VERTEX_TYPE_ENVIRONMENT_UNCOMPRESSED,
    RASTERIZER_VERTEX_TYPE_ENVIRONMENT_COMPRESSED,
    RASTERIZER_VERTEX_TYPE_ENVIRONMENT_LIGHTMAP_UNCOMPRESSED,
    RASTERIZER_VERTEX_TYPE_ENVIRONMENT_LIGHTMAP_COMPRESSED,
    RASTERIZER_VERTEX_TYPE_MODEL_UNCOMPRESSED,
    RASTERIZER_VERTEX_TYPE_MODEL_COMPRESSED,
    RASTERIZER_VERTEX_TYPE_DYNAMIC_UNLIT,
    RASTERIZER_VERTEX_TYPE_DYNAMIC_LIT,
    RASTERIZER_VERTEX_TYPE_DYNAMIC_SCREEN,
    RASTERIZER_VERTEX_TYPE_DEBUG,
    RASTERIZER_VERTEX_TYPE_DECAL,
    RASTERIZER_VERTEX_TYPE_DETAIL_OBJECT,
    RASTERIZER_VERTEX_TYPE_ENVIRONMENT_UNCOMPRESSED_FF,
    RASTERIZER_VERTEX_TYPE_ENVIRONMENT_LIGHTMAP_UNCOMPRESSED_FF,
    RASTERIZER_VERTEX_TYPE_MODEL_UNCOMPRESSED_FF,
    RASTERIZER_VERTEX_TYPE_MODEL_PROCESSED,
    RASTERIZER_VERTEX_TYPE_UNLIT_ZSPRITE,
    RASTERIZER_VERTEX_TYPE_WIDGET,
    NUMBER_OF_RASTERIZER_VERTEX_TYPES
};

enum {
    TRIANGLE_BUFFER_TYPE_TRIANGLES,
    TRIANGLE_BUFFER_TYPE_PRECOMPILED_STRIP,
    NUMBER_OF_TRIANGLE_BUFFER_TYPES
};

struct vertex_buffer {
    uint16_t type;
    uint16_t pad;
    uint32_t count;
    uint32_t offset;
    Pointer32 base_address;
    Pointer32 hardware_format;
};
static_assert(sizeof(struct vertex_buffer) == 20);

struct triangle_buffer {
    uint16_t type;
    uint16_t pad;
    uint32_t count;
    Pointer32 base_address;
    Pointer32 hardware_format;
};
static_assert(sizeof(struct triangle_buffer) == 16);

struct render_distant_light {
    float_rgb_color color;
    float_vector3d direction;
};
static_assert(sizeof(struct render_distant_light) == 24);

struct render_lighting {
    float_rgb_color ambient_color;
    int16_t distant_light_count;
    uint16_t pad;
    struct render_distant_light distant_lights[2];
    int16_t point_light_count;
    uint16_t pad1;
    int32_t point_light_indices[2];
    float_argb_color reflection_tint_color;
    float_vector3d shadow_vector;
    float_rgb_color shadow_color;
};
static_assert(sizeof(struct render_lighting) == 116);

struct bsp3d {
    struct tag_reflexive nodes;
    struct tag_reflexive planes;
};
static_assert(sizeof(struct bsp3d) == 24);

struct leaf_map {
    Pointer32 bsp;
    struct tag_reflexive leaves;
    struct tag_reflexive portals;
};
static_assert(sizeof(struct leaf_map) == 28);

struct map_leaf {
    struct tag_reflexive faces;
    struct tag_reflexive portal_designators;
};
static_assert(sizeof(struct map_leaf) == 24);

struct map_leaf_face {
    int32_t node_index;
    struct tag_reflexive vertices;
};
static_assert(sizeof(struct map_leaf_face) == 16);

struct leaf_portal {
    int32_t plane_index;
    int32_t leaf_indices[2];
    struct tag_reflexive vertices;
};
static_assert(sizeof(struct leaf_portal) == 24);
