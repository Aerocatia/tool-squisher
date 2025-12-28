#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "../data_types.h"
#include "../cache/cache.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"

#include "scenario.h"
#include "scenario_structure_bsp.h"

static void *structure_bsp_resolve_cached_pointer(
    Pointer32 data_pointer,
    size_t needed_size,
    struct scenario_structure_bsp_reference *reference,
    struct cache_file_instance *cache_file) {

    assert(reference && cache_file && cache_file->valid);
    if(data_pointer < reference->address || reference->address < cache_file->tag_data.data_load_address) {
        return nullptr;
    }

    size_t offset = data_pointer - reference->address;
    size_t bsp_end = reference->offset + reference->size;
    if(offset + needed_size > bsp_end || bsp_end > cache_file->size) {
        return nullptr;
    }

    return cache_file->data + reference->offset + offset;
}

static void *structure_bsp_get_cached_reflexive_element(
    struct tag_reflexive *reflexive,
    uint32_t index,
    size_t size,
    struct scenario_structure_bsp_reference *reference,
    struct cache_file_instance *cache_file) {

    assert(reflexive && reference && cache_file && cache_file->valid);
    if(index >= reflexive->count) {
        return nullptr;
    }

    return structure_bsp_resolve_cached_pointer(reflexive->address + index * size, size, reference, cache_file);
}

#define structure_bsp_get_cached_node(bsp, index, reference, cache_file) \
    structure_bsp_get_cached_reflexive_element(&(bsp)->nodes, index, sizeof(struct structure_node), reference, cache_file)
#define structure_bsp_get_cached_lightmap(bsp, index, reference, cache_file) \
    structure_bsp_get_cached_reflexive_element(&(bsp)->lightmaps, index, sizeof(struct structure_lightmap), reference, cache_file)
#define structure_bsp_get_cached_material(lightmap, index, reference, cache_file) \
    structure_bsp_get_cached_reflexive_element(&(lightmap)->materials, index, sizeof(struct structure_material), reference, cache_file)

bool scenario_structure_bsp_postprocess_all_in_cache(struct cache_file_instance *cache_file) {
    assert(cache_file && cache_file->valid);

    struct tag_data_instance *tag_data = &cache_file->tag_data;
    assert(tag_data->valid);

    auto scenario_id = tag_data->header->scenario_tag;
    struct scenario *scenario = tag_get(scenario_id, TAG_FOURCC_SCENARIO, tag_data);
    if(!scenario) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n", tag_path_get(scenario_id, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO));
        return false;
    }

    for(size_t i = 0; i < scenario->structure_bsp_references.count; i++) {
        struct scenario_structure_bsp_reference *bsp_reference = scenario_get_bsp_reference(scenario, i, tag_data);
        if(!bsp_reference) {
            fprintf(stderr,"BSP reference %zu in \"%s.%s\" is out of bounds\n", i, tag_path_get(scenario_id, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO));
            return false;
        }

        auto bsp_id = bsp_reference->structure_bsp.index;
        if(bsp_reference->size < sizeof(struct cache_file_structure_bsp_header)) {
            fprintf(stderr,"cache data for \"%s.%s\" is too small to be a BSP\n", tag_path_get(bsp_id, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO_STRUCTURE_BSP));
            return false;
        }

        if(bsp_reference->offset + bsp_reference->size > cache_file->size) {
            fprintf(stderr,"cache data for \"%s.%s\" is out of bounds\n", tag_path_get(bsp_id, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO_STRUCTURE_BSP));
            return false;
        }

        struct cache_file_structure_bsp_header *bsp_header = (struct cache_file_structure_bsp_header *)(cache_file->data + bsp_reference->offset);
        if(bsp_header->signature != TAG_FOURCC_SCENARIO_STRUCTURE_BSP) {
            fprintf(stderr,"BSP header for \"%s.%s\" is invalid\n", tag_path_get(bsp_id, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO_STRUCTURE_BSP));
            return false;
        }

        struct structure_bsp *bsp = structure_bsp_resolve_cached_pointer(bsp_header->structure_bsp, sizeof(struct structure_bsp), bsp_reference, cache_file);
        if(!bsp) {
            fprintf(stderr,"tag data for \"%s.%s\" is out of bounds\n", tag_path_get(bsp_id, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO_STRUCTURE_BSP));
            return false;
        }

        for(size_t l = 0; l < bsp->lightmaps.count; l++) {
            struct structure_lightmap *lightmap = structure_bsp_get_cached_lightmap(bsp, l, bsp_reference, cache_file);
            if(!lightmap) {
                fprintf(stderr,"lightmap %zu in \"%s.%s\" is out of bounds\n", l, tag_path_get(bsp_id, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO_STRUCTURE_BSP));
                return false;
            }

            for(size_t m = 0; m < lightmap->materials.count; m++) {
                struct structure_material *material = structure_bsp_get_cached_material(lightmap, m, bsp_reference, cache_file);
                if(!material) {
                    fprintf(stderr,"material %zu of lightmap %zu in \"%s.%s\" is out of bounds\n", m, l, tag_path_get(bsp_id, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO_STRUCTURE_BSP));
                    return false;
                }

                // Set the vertex buffer types to a consistent state.
                // This will be set correctly by the game when the BSP is loaded, but here can be set to whatever was in the loose tag (different depending on what tool last touched it)
                material->vertices.type = RASTERIZER_VERTEX_TYPE_ENVIRONMENT_UNCOMPRESSED;
                if(material->lightmap_vertices.count > 0) {
                    material->lightmap_vertices.type = RASTERIZER_VERTEX_TYPE_ENVIRONMENT_LIGHTMAP_UNCOMPRESSED;
                }

                // Zero stale pointers. These are set when loaded, so anything here will be from a previous load.
                material->vertices.base_address = 0;
                material->vertices.hardware_format = 0;
                material->lightmap_vertices.base_address = 0;
                material->lightmap_vertices.hardware_format = 0;
            }
        }

        // Check for HEK+ damage in node bounds
        // Node bounds are a set of relative floats compressed to an unsigned 8-bit integer
        bool inverted_nodes = false;
        for(size_t n = 0; n < bsp->nodes.count; n++) {
            struct structure_node *node = structure_bsp_get_cached_node(bsp, n, bsp_reference, cache_file);
            if(!node) {
                fprintf(stderr,"node %zu in \"%s.%s\" is out of bounds\n", n, tag_path_get(bsp_id, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO_STRUCTURE_BSP));
                return false;
            }

            // This never happens on normal tags, so we can assume HEK+ did it and try to fix it
            if(node->bounds.x0 > node->bounds.x1 || node->bounds.y0 > node->bounds.y1 || node->bounds.y0 > node->bounds.y1) {
                uint8_rectangle3d temp = node->bounds;
                node->bounds.x0 = temp.x1;
                node->bounds.x1 = temp.x0;
                node->bounds.y0 = temp.y1;
                node->bounds.y1 = temp.y0;
                node->bounds.z0 = temp.z1;
                node->bounds.z1 = temp.z0;
                if(node->bounds.x0 > node->bounds.x1 || node->bounds.y0 > node->bounds.y1 || node->bounds.y0 > node->bounds.y1) {
                    fprintf(stderr,"node %zu in \"%s.%s\" has invalid bounds and can not be fixed\n", n, tag_path_get(bsp_id, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO_STRUCTURE_BSP));
                    return false;
                }

                inverted_nodes = true;
            }
        }

        if(inverted_nodes) {
            fprintf(stderr,"fixed inverted node bounds in \"%s.%s\"\nthis was likely caused by HEK+, if you are sure this BSP was not touched by HEK+ then this might be a bug!\n", tag_path_get(bsp_id, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO_STRUCTURE_BSP));
        }
    }

    return true;
}
