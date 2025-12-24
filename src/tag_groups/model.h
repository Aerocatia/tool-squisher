#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "model_types.h"

enum {
    MODEL_GEOMETRY_PART_FLAGS_STRIPPED_BIT,
    NUMBER_OF_MODEL_GEOMETRY_PART_FLAGS
};

enum {
    MODEL_GEOMETRY_FLAGS_UNUSED_BIT,
    NUMBER_OF_MODEL_GEOMETRY_FLAGS
};

enum {
    MODEL_REGION_PERMUTATION_FLAGS_CANNOT_BE_CHOSEN_RANDOMLY_BIT,
    NUMBER_OF_MODEL_REGION_PERMUTATION_FLAGS
};

enum {
    MODEL_FLAGS_BLEND_SHARED_NORMALS_BIT,
    NUMBER_OF_MODEL_FLAGS
};

enum {
    GBXMODEL_FLAGS_BLEND_SHARED_NORMALS_BIT,
    GBXMODEL_FLAGS_PARTS_HAVE_LOCAL_NODES_BIT,
    GBXMODEL_FLAGS_IGNORE_SKINNING_BIT,
    NUMBER_OF_GBXMODEL_FLAGS
};

enum {
    MODEL_DETAIL_LEVEL_SUPER_LOW,
    MODEL_DETAIL_LEVEL_LOW,
    MODEL_DETAIL_LEVEL_MEDIUM,
    MODEL_DETAIL_LEVEL_HIGH,
    MODEL_DETAIL_LEVEL_SUPER_HIGH,
    NUMBER_OF_MODEL_DETAIL_LEVELS
};

#pragma pack(push, 1)

struct model_geometry_part {
    uint32_t flags;
    int16_t shader_index;
    int8_t prev_part_index;
    int8_t next_part_index;
    int16_t centroid_primary_node_index;
    int16_t centroid_secondary_node_index;
    float centroid_primary_node_weight;
    float centroid_secondary_node_weight;
    float_point3d centroid;
    struct tag_reflexive uncompressed_vertices;
    struct tag_reflexive compressed_vertices;
    struct tag_reflexive triangles;
    struct triangle_buffer triangle_buffer;
    struct vertex_buffer vertex_buffer;
};
static_assert(sizeof(struct model_geometry_part) == 104);

struct gbxmodel_geometry_part {
    struct model_geometry_part;
    uint8_t pad[3];
    uint8_t local_node_count;
    uint8_t local_node_indices[22];
    uint16_t pad1;
};
static_assert(sizeof(struct gbxmodel_geometry_part) == 132);

struct model_geometry {
    uint32_t flags;
    uint32_t unused[8];
    struct tag_reflexive parts;
};
static_assert(sizeof(struct model_geometry) == 48);

struct model_region_permutation_marker {
    String32 name;
    int16_t node_index;
    uint16_t pad;
    float_quaternion rotation;
    float_point3d translation;
    uint32_t unused[4];
};
static_assert(sizeof(struct model_region_permutation_marker) == 80);

struct model_region_permutation {
    String32 name;
    uint32_t flags;
    int16_t variant_number;
    uint16_t pad;
    uint32_t unused[6];
    int16_t geometry_indices[NUMBER_OF_MODEL_DETAIL_LEVELS];
    uint16_t pad2;
    struct tag_reflexive markers;
};
static_assert(sizeof(struct model_region_permutation) == 88);

struct model_region {
    String32 name;
    uint32_t unused[8];
    struct tag_reflexive permutations;
};
static_assert(sizeof(struct model_region) == 76);

struct model_node {
    String32 name;
    int16_t next_sibling_node_index;
    int16_t first_child_node_index;
    int16_t parent_node_index;
    uint16_t pad;
    float_point3d default_translation;
    float_quaternion default_rotation;
    float node_distance_from_parent;
    uint32_t unused[8];
    float_matrix4x3 runtime_default_inverse_matrix;
};
static_assert(sizeof(struct model_node) == 156);

struct model_shader_reference {
    struct tag_reference shader;
    int16_t permutation_index;
    uint16_t pad;
    uint32_t unused[3];
};
static_assert(sizeof(struct model_shader_reference) == 32);

struct model_marker_instance {
    uint8_t region_index;
    uint8_t permutation_index;
    uint8_t node_index;
    uint8_t pad;
    float_point3d translation;
    float_quaternion rotation;
};
static_assert(sizeof(struct model_marker_instance) == 32);

struct model_marker {
    String32 name;
    uint32_t unused[5];
    struct tag_reflexive instances;
};
static_assert(sizeof(struct model_marker) == 64);

struct model {
    uint32_t flags;
    uint32_t node_list_checksum;
    float detail_cutoff_pixels[NUMBER_OF_MODEL_DETAIL_LEVELS];
    int16_t node_counts[NUMBER_OF_MODEL_DETAIL_LEVELS];
    uint16_t pad;
    uint32_t unused1[2];
    float_vector2d base_map_scale;
    uint32_t unused2[29];
    struct tag_reflexive runtime_markers;
    struct tag_reflexive nodes;
    struct tag_reflexive regions;
    struct tag_reflexive geometries;
    struct tag_reflexive shaders;
};
static_assert(sizeof(struct model) == 232);

#pragma pack(pop)

#define model_get_geometry(model, index, data) tag_reflexive_get_element(&(model)->geometries, index, sizeof(struct model_geometry), data)
#define model_get_geometry_part(geometry, index, data) tag_reflexive_get_element(&(geometry)->parts, index, sizeof(struct model_geometry_part), data)
#define gbxmodel_get_geometry_part(geometry, index, data) tag_reflexive_get_element(&(geometry)->parts, index, sizeof(struct gbxmodel_geometry_part), data)

bool gbxmodel_postprocess(TagID tag, struct tag_data_instance *tag_data);
