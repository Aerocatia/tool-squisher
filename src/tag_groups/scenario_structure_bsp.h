#pragma once

#include <stdint.h>

#include "../data_types.h"
#include "../cache/cache.h"
#include "../tag/tag.h"
#include "model_types.h"

enum {
    STRUCTURE_MATERIAL_FLAGS_COPLANAR_BIT,
    STRUCTURE_MATERIAL_FLAGS_FOG_PLANE_BIT,
    NUMBER_OF_STRUCTURE_MATERIAL_FLAGS
};

enum {
    CLUSTER_PORTAL_FLAGS_AI_DEAFENING_BIT,
    NUMBER_OF_CLUSTER_PORTAL_FLAGS
};

#pragma pack(push, 1)

struct structure_collision_material {
    struct tag_reference shader;
    uint16_t pad;
    uint16_t runtime_physics_material_type;
};
static_assert(sizeof(struct structure_collision_material) == 20);

struct structure_node {
    uint8_rectangle3d bounds;
};
static_assert(sizeof(struct structure_node) == 6);

struct structure_leaf {
    uint8_rectangle3d bounds;
    uint16_t pad;
    int16_t cluster_index;
    int16_t surface_reference_count;
    int32_t first_surface_reference_index;
};
static_assert(sizeof(struct structure_leaf) == 16);

struct structure_surface {
    uint16_t vertex_indices[3];
};
static_assert(sizeof(struct structure_surface) == 6);

struct structure_surface_reference {
    int32_t surface_index;
    int32_t bsp3d_node_index;
};
static_assert(sizeof(struct structure_surface_reference) == 8);

struct structure_lightmap {
    int16_t bitmap_index;
    uint16_t pad;
    uint32_t unused[4];
    struct tag_reflexive materials;
};
static_assert(sizeof(struct structure_lightmap) == 32);

struct structure_material {
    struct tag_reference shader;
    int16_t permutation_index;
    uint16_t flags;
    int32_t first_surface_index;
    int32_t surface_count;
    float_point3d centroid;
    struct render_lighting lighting;
    float_plane3d plane;
    int16_t breakable_surface_index;
    uint16_t pad;
    struct vertex_buffer vertices;
    struct vertex_buffer lightmap_vertices;
    struct tag_data uncompressed_vertex_data;
    struct tag_data compressed_vertex_data;
};
static_assert(sizeof(struct structure_material) == 256);

struct structure_lens_flare {
    struct tag_reference lens_flare;
};
static_assert(sizeof(struct structure_lens_flare) == 16);

struct structure_lens_flare_marker {
    float_point3d position;
    int8_t i_direction;
    int8_t j_direction;
    int8_t k_direction;
    uint8_t lens_flare_index;
};
static_assert(sizeof(struct structure_lens_flare_marker) == 16);

struct structure_subcluster {
    float_rectangle3d world_bounds;
    struct tag_reflexive surface_indices;
};
static_assert(sizeof(struct structure_subcluster) == 36);

struct structure_cluster {
    int16_t sky_index;
    int16_t fog_designator;
    int16_t background_sound_palette_index;
    int16_t sound_environment_palette_index;
    int16_t weather_palette_index;
    int16_t transitions_to_structure_bsp_index;
    int16_t first_runtime_decal_index;
    uint16_t runtime_decal_count;
    uint32_t unused1[6];
    struct tag_reflexive predicted_resources;
    struct tag_reflexive subclusters;
    uint16_t first_lens_flare_marker_index;
    uint16_t lens_flare_marker_count;
    struct tag_reflexive surface_indices;
    struct tag_reflexive mirrors;
    struct tag_reflexive portal_indices;
};
static_assert(sizeof(struct structure_cluster) == 104);

struct structure_mirror {
    float_plane3d plane;
    uint32_t unused[5];
    struct tag_reference shader;
    struct tag_reflexive points;
};
static_assert(sizeof(struct structure_mirror) == 64);

struct cluster_portal {
    int16_t cluster_indices[2];
    int32_t plane_index;
    float_point3d centroid;
    float bounding_radius;
    uint32_t flags;
    uint32_t unused[6];
    struct tag_reflexive vertices;
};
static_assert(sizeof(struct cluster_portal) == 64);

struct structure_fog_plane {
    int16_t region_index;
    uint16_t runtime_material_type;
    float_plane3d plane;
    struct tag_reflexive vertices;
};
static_assert(sizeof(struct structure_fog_plane) == 32);

struct structure_fog_region {
    uint32_t unused[9];
    int16_t fog_palette_index;
    int16_t weather_palette_index;
};
static_assert(sizeof(struct structure_fog_region) == 40);

struct structure_fog_palette_entry {
    String32 name;
    struct tag_reference fog;
    uint16_t pad;
    int16_t runtime_global_function_index;
    String32 global_function_name;
    uint32_t unused[13];
};
static_assert(sizeof(struct structure_fog_palette_entry) == 136);

struct structure_weather_palette_entry {
    String32 name;
    struct tag_reference particle_system;
    uint16_t pad1;
    int16_t runtime_particle_system_global_function_index;
    String32 particle_system_global_function_name;
    uint32_t unused0[11];
    struct tag_reference wind;
    float_vector3d wind_direction;
    float wind_magnitude;
    uint16_t pad2;
    int16_t wind_global_function_index;
    String32 wind_global_function_name;
    uint32_t unused1[11];
};
static_assert(sizeof(struct structure_weather_palette_entry) == 240);

struct structure_weather_polyhedron {
    float_point3d bounding_sphere_center;
    float bounding_sphere_radius;
    uint32_t unused;
    struct tag_reflexive planes;
};
static_assert(sizeof(struct structure_weather_polyhedron) == 32);

struct structure_sound_environment_palette_entry {
    String32 name;
    struct tag_reference sound_environment;
    uint32_t unused[8];
};

struct structure_background_sound_palette_entry {
    String32 name;
    struct tag_reference background_sound;
    uint16_t pad;
    int16_t runtime_global_function_index;
    String32 global_function_name;
    uint32_t unused[8];
};
static_assert(sizeof(struct structure_background_sound_palette_entry) == 116);

struct structure_breakable_surface {
    float_point3d centroid;
    float bounding_radius;
    int32_t collision_surface_index;
    uint32_t unused[7];
};
static_assert(sizeof(struct structure_breakable_surface) == 48);

struct structure_marker {
    String32 name;
    float_quaternion rotation;
    float_point3d translation;
};
static_assert(sizeof(struct structure_marker) == 60);

struct structure_detail_object_data {
    struct tag_reflexive cells;
    struct tag_reflexive detail_objects;
    struct tag_reflexive detail_objects_counts;
    struct tag_reflexive detail_object_z_reference_vectors;
    bool valid;
    uint8_t pad[3];
    uint32_t unused[3];
};
static_assert(sizeof(struct structure_detail_object_data) == 64);

struct structure_runtime_decal {
    float_point3d position;
    uint8_t palette_index;
    uint8_t unused;
    int8_t yaw;
    int8_t pitch;
};
static_assert(sizeof(struct structure_runtime_decal) == 16);

struct structure_bsp {
    struct tag_reference lightmap_group;
    float vehicle_floor;
    float vehicle_ceiling;
    uint32_t unused0[5];
    struct render_lighting default_lighting;
    uint32_t unused1;
    struct tag_reflexive collision_materials;
    struct tag_reflexive collision_bsp;
    struct tag_reflexive nodes;
    float_rectangle3d world_bounds;
    struct tag_reflexive leaves;
    struct tag_reflexive surface_references;
    struct tag_reflexive surfaces;
    struct tag_reflexive lightmaps;
    uint32_t unused2[3];
    struct tag_reflexive lens_flares;
    struct tag_reflexive lens_flare_markers;
    struct tag_reflexive clusters;
    struct tag_data cluster_data;
    struct tag_reflexive cluster_portals;
    uint32_t unused3[3];
    struct tag_reflexive breakable_surfaces;
    struct tag_reflexive fog_planes;
    struct tag_reflexive fog_regions;
    struct tag_reflexive fog_palette;
    uint32_t unused4[6];
    struct tag_reflexive weather_palette;
    struct tag_reflexive weather_polyhedra;
    uint32_t unused5[6];
    struct tag_reflexive pathfinding_surfaces;
    struct tag_reflexive pathfinding_edges;
    struct tag_reflexive background_sound_palette;
    struct tag_reflexive sound_environment_palette;
    struct tag_data sound_cluster_data;
    uint32_t unused6[6];
    struct tag_reflexive markers;
    struct tag_reflexive detail_object_data;
    struct tag_reflexive runtime_decals;
    uint32_t unused7[2];
    struct leaf_map leaf_map;
};
static_assert(sizeof(struct structure_bsp) == 648);

#pragma pack(pop)

bool scenario_structure_bsp_postprocess_all_in_cache(struct cache_file_instance *cache_file);
