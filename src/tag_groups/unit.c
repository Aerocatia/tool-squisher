#include <stdint.h>
#include <stdio.h>

#include "unit.h"

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"

bool uint_final_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct unit *unit = tag_get(tag, TAG_FOURCC_UNIT, tag_data);
    if(!unit) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n", tag_path_get(tag, tag_data), tag_extension_get(tag, tag_data));
        return false;
    }

    byteswap16(&unit->unit.metagame_properties.metagame_type);
    if(unit->unit.metagame_properties.metagame_type >= NUMBER_OF_UNIT_METAGAME_TYPES) {
        unit->unit.metagame_properties.metagame_type = UNIT_METAGAME_TYPE_BRUTE;
    }

    byteswap16(&unit->unit.metagame_properties.metagame_class);
    if(unit->unit.metagame_properties.metagame_class >= NUMBER_OF_UNIT_METAGAME_CLASSES) {
        unit->unit.metagame_properties.metagame_class = UNIT_METAGAME_CLASS_INFANTRY;
    }

    return true;
}
