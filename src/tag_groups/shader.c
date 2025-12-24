#include <stdint.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"

#include "shader.h"

bool shader_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct shader *shader = tag_get(tag, TAG_FOURCC_SHADER, tag_data);
    if(!shader) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n", tag_path_get(tag, tag_data), tag_extension_get(tag, tag_data));
        return false;
    }

    // If the map was compiled with 1.10 "digsite" tool_play.exe or tool_symbols.exe the shader type will be set as an 8-bit
    // value with the other 8-bits padded with 0xFF. This will crash the standard client that reads these as a 16-bit value.
    // Using a switch statment here allows us to also check for other funky stuff like using the base shader struct as a
    // stand-alone tag (could be done with kornman00.exe).
    // the tag index here should be valid since we resolved tag data.
    switch(tag_data->tags[tag.index].primary_group) {
        case TAG_FOURCC_SHADER_ENVIRONMENT:
            shader->type = SHADER_TYPE_ENVIRONMENT;
            break;
        case TAG_FOURCC_SHADER_MODEL:
            shader->type = SHADER_TYPE_MODEL;
            break;
        case TAG_FOURCC_SHADER_TRANSPARENT_GENERIC:
            shader->type = SHADER_TYPE_TRANSPARENT_GENERIC;
            break;
        case TAG_FOURCC_SHADER_TRANSPARENT_CHICAGO:
            shader->type = SHADER_TYPE_TRANSPARENT_CHICAGO;
            break;
        case TAG_FOURCC_SHADER_TRANSPARENT_CHICAGO_EXTENDED:
            shader->type = SHADER_TYPE_TRANSPARENT_CHICAGO_EXTENDED;
            break;
        case TAG_FOURCC_SHADER_TRANSPARENT_WATER:
            shader->type = SHADER_TYPE_TRANSPARENT_WATER;
            break;
        case TAG_FOURCC_SHADER_TRANSPARENT_GLASS:
            shader->type = SHADER_TYPE_TRANSPARENT_GLASS;
            break;
        case TAG_FOURCC_SHADER_TRANSPARENT_METER:
            shader->type = SHADER_TYPE_TRANSPARENT_METER;
            break;
        case TAG_FOURCC_SHADER_TRANSPARENT_PLASMA:
            shader->type = SHADER_TYPE_TRANSPARENT_PLASMA;
            break;
        default:
            fprintf(stderr,"tag \"%s.%s\" is not valid for a shader tag\n", tag_path_get(tag, tag_data), tag_extension_get(tag, tag_data));
            return false;
    }

    return true;
}
