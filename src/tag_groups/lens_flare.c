#include <stdint.h>
#include <stdio.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"

#include "lens_flare.h"

bool lens_flare_final_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct lens_flare *lens_flare = tag_get(tag, TAG_FOURCC_LENS_FLARE, tag_data);
    if(!lens_flare) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n", tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_LENS_FLARE));
        return false;
    }

    // Fix broken default value of 360 radians
    if(lens_flare->corona_rotation_function_scale == 360.0) {
        lens_flare->corona_rotation_function_scale = HALO_TWO_PI;
    }

    return true;
}
