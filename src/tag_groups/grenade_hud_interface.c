#include <stdint.h>
#include <stdio.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"
#include "../tag/tag_processing.h"

#include "hud_types.h"
#include "grenade_hud_interface.h"

bool grenade_hud_interface_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct grenade_hud_interface *grenade_hud = tag_get(tag, TAG_FOURCC_GRENADE_HUD_INTERFACE, tag_data);
    if(!grenade_hud) {
        fprintf(stderr, "tag data for \"%s.%s\" is invalid\n",
            tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_GRENADE_HUD_INTERFACE)
        );
        return false;
    }

    // Absolute placement
    hud_process_absolute_placement(&grenade_hud->absolute_placement);

    return true;
}
