#include <stdint.h>
#include <stdio.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"
#include "../tag/tag_processing.h"

#include "weapon_hud_interface.h"

#define PROCESS_CHILD_ANCHOR(anchor) tag_process_enum16(anchor, NUMBER_OF_HUD_CHILD_ANCHORS, HUD_CHILD_ANCHOR_FROM_PARENT)

bool weapon_hud_interface_final_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct weapon_hud_interface *weapon_hud = tag_get(tag, TAG_FOURCC_WEAPON_HUD_INTERFACE, tag_data);
    if(!weapon_hud) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n", tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_WEAPON_HUD_INTERFACE));
        return false;
    }

    // Nothing supports this extension as of this time but might as well handle it for now
    tag_process_enum16(&weapon_hud->absolute_placement.canvas_size, NUMBER_OF_HUD_CANVAS_SIZES, HUD_CANVAS_SIZE_480P);

    // Static elements
    for(size_t statics = 0; statics < weapon_hud->statics.count; statics++) {
        struct weapon_hud_static_element *static_element = weapon_hud_get_static_element(weapon_hud, statics, tag_data);
        if(!static_element) {
            fprintf(stderr,"static element %zu in \"%s.%s\" is out of bounds\n", statics, tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_WEAPON_HUD_INTERFACE));
            return false;
        }

        PROCESS_CHILD_ANCHOR(&static_element->header.child_anchor);
    }

    // Meter elements
    for(size_t meters = 0; meters < weapon_hud->meters.count; meters++) {
        struct weapon_hud_meter_element *meter_element = weapon_hud_get_meter_element(weapon_hud, meters, tag_data);
        if(!meter_element) {
            fprintf(stderr,"meter element %zu in \"%s.%s\" is out of bounds\n", meters, tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_WEAPON_HUD_INTERFACE));
            return false;
        }

        PROCESS_CHILD_ANCHOR(&meter_element->header.child_anchor);
        hud_process_meter_element(&meter_element->meter_element);
    }

    // Number elements
    for(size_t numbers = 0; numbers < weapon_hud->numbers.count; numbers++) {
        struct weapon_hud_number_element *number_element = weapon_hud_get_number_element(weapon_hud, numbers, tag_data);
        if(!number_element) {
            fprintf(stderr,"number element %zu in \"%s.%s\" is out of bounds\n", numbers, tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_WEAPON_HUD_INTERFACE));
            return false;
        }

        PROCESS_CHILD_ANCHOR(&number_element->header.child_anchor);
    }

    // Overlays elements
    for(size_t overlays = 0; overlays < weapon_hud->overlays.count; overlays++) {
        struct weapon_hud_overlays_element *overlays_element = weapon_hud_get_overlays_element(weapon_hud, overlays, tag_data);
        if(!overlays_element) {
            fprintf(stderr,"overlays element %zu in \"%s.%s\" is out of bounds\n", overlays, tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_WEAPON_HUD_INTERFACE));
            return false;
        }

        PROCESS_CHILD_ANCHOR(&overlays_element->header.child_anchor);
    }

    // Check for buggy zoom flag state
    if(weapon_hud->crosshairs.count == 0 || TEST_FLAG(weapon_hud->valid_crosshair_types_flags, WEAPON_HUD_CROSSHAIR_STATE_ZOOM)) {
        // no crosshairs or zoom overlay bit is already set. nothing more to do
        return true;
    }

    for(size_t crosshairs = 0; crosshairs < weapon_hud->crosshairs.count; crosshairs++) {
        struct weapon_hud_crosshairs_element *crosshairs_element = weapon_hud_get_crosshairs_element(weapon_hud, crosshairs, tag_data);
        if(!crosshairs_element) {
            fprintf(stderr,"crosshairs element %zu in \"%s.%s\" is out of bounds\n", crosshairs, tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_WEAPON_HUD_INTERFACE));
            return false;
        }
        for(size_t overlay = 0; overlay < crosshairs_element->crosshairs.items.count; overlay++) {
            struct weapon_hud_crosshair_item *overlay_element = weapon_hud_get_crosshairs_item(crosshairs_element, overlay, tag_data);
            if(!overlay_element) {
                fprintf(stderr,"crosshair overlay %zu in crosshairs element %zu in \"%s.%s\" is out of bounds\n", overlay, crosshairs, tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_WEAPON_HUD_INTERFACE));
                return false;
            }
            if(TEST_FLAG(overlay_element->flags, WEAPON_HUD_CROSSHAIR_FLAGS_NOT_ON_DEFAULT_ZOOM_BIT) ||
                TEST_FLAG(overlay_element->flags, WEAPON_HUD_CROSSHAIR_FLAGS_ONE_ZOOM_LEVEL_BIT) ||
                TEST_FLAG(overlay_element->flags, WEAPON_HUD_CROSSHAIR_FLAGS_ONLY_ON_DEFAULT_ZOOM_BIT)) {

                SET_FLAG(weapon_hud->valid_crosshair_types_flags, WEAPON_HUD_CROSSHAIR_STATE_ZOOM, true);
                return true;
            }
        }
    }

    return true;
}
