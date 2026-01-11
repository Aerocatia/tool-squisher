#include <stdint.h>
#include <string.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"

#include "shader_model.h"

bool shader_model_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct shader_model *shader = tag_get(tag, TAG_FOURCC_SHADER_MODEL, tag_data);
    if(!shader) {
        fprintf(stderr, "tag data for \"%s.%s\" is invalid\n",
            tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SHADER_MODEL)
        );
        return false;
    }

    // Partially removed field. This will be defaulted to 1.0 if zero in the tag file, otherwise it's copied in big-endian.
    shader->model.reflection_bump_map_scale = 1.0f;

    // This is always copied in big-endian so reset it.
    tag_null_reference(&shader->model.reflection_bump_map, TAG_FOURCC_BITMAP);

    return true;
}
