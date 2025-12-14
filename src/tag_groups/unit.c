#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "unit.h"

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"
#include "../tag/tag_processing.h"

void unit_process_metagame_properties(struct unit_metagame_properties *metagame_properties) {
    if(!metagame_properties) {
        abort();
    }

    tag_process_enum16(&metagame_properties->metagame_type, NUMBER_OF_UNIT_METAGAME_TYPES, UNIT_METAGAME_TYPE_BRUTE);
    tag_process_enum16(&metagame_properties->metagame_class, NUMBER_OF_UNIT_METAGAME_CLASSES, UNIT_METAGAME_CLASS_INFANTRY);
}

bool uint_final_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct unit *unit = tag_get(tag, TAG_FOURCC_UNIT, tag_data);
    if(!unit) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n", tag_path_get(tag, tag_data), tag_extension_get(tag, tag_data));
        return false;
    }

    // Added in MCC CEA
    unit_process_metagame_properties(&unit->unit.metagame_properties);

    return true;
}
