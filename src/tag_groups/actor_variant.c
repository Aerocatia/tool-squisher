#include <stdint.h>
#include <stdio.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"

#include "actor_variant.h"
#include "unit.h"

bool actor_variant_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct actor_variant *actor_variant = tag_get(tag, TAG_FOURCC_ACTOR_VARIANT, tag_data);
    if(!actor_variant) {
        fprintf(stderr, "tag data for \"%s.%s\" is invalid\n",
            tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_ACTOR_VARIANT)
        );
        return false;
    }

    // These can be invalid due to Bungie changing the struct after some stock tags were made, and tool.exe will not check them.
    if(actor_variant->grenade_combat.grenade_type >= NUMBER_OF_UNIT_GRENADE_TYPES) {
        actor_variant->grenade_combat.grenade_type = UNIT_GRENADE_TYPE_HUMAN_FRAGMENTATION;
    }
    if(actor_variant->grenade_combat.trajectory_type >= NUMBER_OF_ACTOR_VARIANT_GRENADE_TRAJECTORIES) {
        actor_variant->grenade_combat.trajectory_type = ACTOR_VARIANT_GRENADE_TRAJECTORY_TOSS;
    }
    if(actor_variant->grenade_combat.stimulus_type >= NUMBER_OF_ACTOR_VARIANT_GRENADE_STIMULI) {
        actor_variant->grenade_combat.stimulus_type = ACTOR_VARIANT_GRENADE_STIMULUS_NONE;
    }
    actor_variant->grenade_combat.minimum_enemy_count = FLOOR(actor_variant->grenade_combat.minimum_enemy_count, 0);

    // Added in MCC CEA
    unit_process_metagame_properties(&actor_variant->metagame_properties);

    return true;
}
