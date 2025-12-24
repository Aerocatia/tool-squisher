#include <stdint.h>
#include <stdio.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"

#include "model.h"

bool gbxmodel_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct model *gbxmodel = tag_get(tag, TAG_FOURCC_GBXMODEL, tag_data);
    if(!gbxmodel) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n", tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_GBXMODEL));
        return false;
    }

    for(size_t g = 0; g < gbxmodel->geometries.count; g++) {
        struct model_geometry *geometry = model_get_geometry(gbxmodel, g, tag_data);
        if(!geometry) {
            fprintf(stderr,"geometry %zu in \"%s.%s\" is out of bounds\n", g, tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_GBXMODEL));
            return false;
        }

        for(size_t gp = 0; gp < geometry->parts.count; gp++) {
            struct gbxmodel_geometry_part *part = gbxmodel_get_geometry_part(geometry, gp, tag_data);
            if(!part) {
                fprintf(stderr,"geometry part %zu of geometry %zu in \"%s.%s\" is out of bounds\n", gp, g, tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_GBXMODEL));
                return false;
            }

            // This contains a stale pointer from when the map was built, so zeroing it allows model tag data between map builds to be reproducible
            part->vertex_buffer.base_address = 0;
        }
    }

    return true;
}
