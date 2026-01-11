#include <stdint.h>
#include <stdio.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"

#include "hud_types.h"
#include "unit_hud_interface.h"

bool unit_hud_interface_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct unit_hud_interface *unit_hud = tag_get(tag, TAG_FOURCC_UNIT_HUD_INTERFACE, tag_data);
    if(!unit_hud) {
        fprintf(stderr, "tag data for \"%s.%s\" is invalid\n",
            tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_UNIT_HUD_INTERFACE)
        );
        return false;
    }

    hud_process_absolute_placement(&unit_hud->absolute_placement);
    hud_process_absolute_placement(&unit_hud->auxiliary_panel.absolute_placement);
    hud_process_meter_element(&unit_hud->shield_meter.meter);
    hud_process_meter_element(&unit_hud->health_meter.meter);

    // Auxiliary meter elements
    for(size_t i = 0; i < unit_hud->auxiliary_meters.count; i++) {
        struct uint_hud_auxiliary_meter_element *meter_element = unit_hud_get_auxiliary_meter_element(unit_hud, i, tag_data);
        if(!meter_element) {
            fprintf(stderr, "auxiliary meter element %zu in \"%s.%s\" is out of bounds\n",
                i, tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_UNIT_HUD_INTERFACE)
            );
            return false;
        }

        hud_process_meter_element(&meter_element->panel.meter);
    }

    return true;
}
