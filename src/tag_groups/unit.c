#include <stdint.h>
#include <stdio.h>

#include "unit.h"

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"
#include "../tag/tag_processing.h"

bool uint_final_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct unit *unit = tag_get(tag, TAG_FOURCC_UNIT, tag_data);
    if(!unit) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n", tag_path_get(tag, tag_data), tag_extension_get(tag, tag_data));
        return false;
    }

    tag_process_enum16(&unit->unit.metagame_properties.metagame_type, NUMBER_OF_UNIT_METAGAME_TYPES, UNIT_METAGAME_TYPE_BRUTE);
    tag_process_enum16(&unit->unit.metagame_properties.metagame_class, NUMBER_OF_UNIT_METAGAME_CLASSES, UNIT_METAGAME_CLASS_INFANTRY);

    return true;
}
