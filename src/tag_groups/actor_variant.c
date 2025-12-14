#include <stdint.h>
#include <stdio.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"

#include "actor_variant.h"
#include "unit.h"

bool actor_variant_final_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct actor_variant *actor_variant = tag_get(tag, TAG_FOURCC_ACTOR_VARIANT, tag_data);
    if(!actor_variant) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n", tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_ACTOR_VARIANT));
        return false;
    }

    // Added in MCC CEA
    unit_process_metagame_properties(&actor_variant->metagame_properties);

    return true;
}
