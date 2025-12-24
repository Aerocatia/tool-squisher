#include <stdint.h>
#include <stdio.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"

#include "meter.h"

bool meter_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct meter *meter = tag_get(tag, TAG_FOURCC_METER, tag_data);
    if(!meter) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n", tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_METER));
        return false;
    }

    // No encoded stencil data (likely extracted and re-built)
    if(meter->encoded_stencil.size == 0) {
        return true;
    }

    uint8_t *stencil = tag_resolve_pointer(meter->encoded_stencil.address, meter->encoded_stencil.size, tag_data);
    if(!stencil) {
        fprintf(stderr,"encoded stencil data in \"%s.%s\" is out of bounds\n", tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_METER));
        return false;
    }

    // Go through each encoded pixel and zero out the padding between the mask and meter level
    // When maps are built by tool.exe this data will be uninitialized, breaking reproducible map builds
    size_t row_index = 0;
    uint8_t *end = stencil + meter->encoded_stencil.size;
    struct meter_encoded_row *row = (struct meter_encoded_row *)stencil;
    while((uint8_t *)row < end) {
        if((uint8_t *)row + sizeof(struct meter_encoded_row) > end) {
            fprintf(stderr,"encoded stencil row data %zu in \"%s.%s\" is out of bounds\n", row_index, tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_METER));
            return false;
        }

        // Check origin (so we are more sure this is what we think it is)
        if(row->origin.x + row->pixel_count > meter->runtime_width || row->origin.y > meter->runtime_height) {
            fprintf(stderr,"encoded stencil row data %zu origin in \"%s.%s\" is out of bounds for meter dimensions\n", row_index, tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_METER));
            return false;
        }

        struct meter_encoded_pixel *pixel = row->pixels;
        struct meter_encoded_pixel *last_pixel = &row->pixels[row->pixel_count];
        if((uint8_t *)last_pixel > end) {
            fprintf(stderr,"encoded stencil pixel data for row %zu in \"%s.%s\" is out of bounds\n", row_index, tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_METER));
            return false;
        }

        while(pixel < last_pixel) {
            pixel->pad = 0;
            pixel++;
        }

        row = (struct meter_encoded_row *)last_pixel;
        row_index++;
    }

    return true;
}
