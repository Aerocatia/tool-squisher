#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"

#include "hud_types.h"
#include "hud_globals.h"

bool hud_globals_final_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct hud_globals *hud_globals = tag_get(tag, TAG_FOURCC_HUD_GLOBALS, tag_data);
    if(!hud_globals) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n", tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_HUD_GLOBALS));
        return false;
    }

    // There is no way this field can be valid if it was built with the expected tool versions.
    // If these have a count, then it was likely truncated in a way where the rest of the tag is fine
    // as these are the last thing defined in the tag. We zero it out here so it can be extracted.
    if(hud_globals->bitmap_remaps.count != 0) {
        memset(&hud_globals->bitmap_remaps, 0, sizeof(struct tag_reflexive));
        fprintf(stderr,"Warning: HUD globals tag \"%s.%s\" had MCC CEA bitmap remaps\nThis was likely corrupted by the older tool.exe so the reflexive was zeroed out\n", tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_HUD_GLOBALS));
    }

    // Absolute placement
    hud_process_absolute_placement(&hud_globals->messaging.absolute_placement);

    return true;
}
