#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../data_types.h"
#include "../tag/tag.h"
#include "../tag/tag_fourcc.h"

#include "scenario.h"
#include "scenario/ai.h"

bool scenario_final_postprocess(TagID tag, struct tag_data_instance *tag_data) {
    struct scenario *scenario = tag_get(tag, TAG_FOURCC_SCENARIO, tag_data);
    if(!scenario) {
        fprintf(stderr,"tag data for \"%s.%s\" is invalid\n", tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO));
        return false;
    }

    // These can never be valid if the map was compiled with the expected tool versions, so zero it.
    if(scenario->scavenger_hunt_objects.count != 0) {
        memset(&scenario->scavenger_hunt_objects, 0, sizeof(struct tag_reflexive));
        fprintf(stderr,"Warning: Scenario tag \"%s.%s\" had scavenger hunt objects\nThis was likely corrupted by the older tool.exe so the reflexive was zeroed out\n", tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO));
    }

    for(size_t c = 0; c < scenario->ai_conversations.count; c++) {
        struct ai_conversation *conversation = scenario_get_ai_conversation(scenario, c, tag_data);
        if(!conversation) {
            fprintf(stderr,"ai conversation %zu in \"%s.%s\" is out of bounds\n", c, tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO));
            return false;
        }

        for(size_t p = 0; p < conversation->participants.count; p++) {
            struct ai_conversation_participant *participant = scenario_get_ai_conversation_participant(conversation, p, tag_data);
            if(!participant) {
                fprintf(stderr,"ai conversation participant %zu in \"%s.%s\" is out of bounds\n", p, tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO));
                return false;
            }

            int16_t variant_numbers[AI_CONVERSATION_DIALOGUE_VARIANT_COUNT] = {[0 ... (AI_CONVERSATION_DIALOGUE_VARIANT_COUNT - 1)] = NONE};
            for(size_t l = 0; l < conversation->lines.count; l++) {
                struct ai_conversation_line *line = scenario_get_ai_conversation_line(conversation, l, tag_data);
                if(!line) {
                    fprintf(stderr,"ai conversation line %zu in \"%s.%s\" is out of bounds\n", l, tag_path_get(tag, tag_data), tag_fourcc_to_extension(TAG_FOURCC_SCENARIO));
                    return false;
                }

                // Not our line
                if(line->participant_index != p) {
                    continue;
                }

                // this can match multiple participants, which can lead to ambiguous variant numbers; we'll just do the last one
                for(size_t i = 0; i < AI_CONVERSATION_DIALOGUE_VARIANT_COUNT; i++) {
                    if(line->dialogue[i].index.whole_id != NULL_ID) {
                        const char *tag_path = tag_path_get(line->dialogue[i].index, tag_data);
                        #define IF_THING_IS_CONTAINED_IN_THE_THING(thing, number) \
                            if(strstr(tag_path, thing) != nullptr) { variant_numbers[i] = number; } \

                        // code is from invader
                        IF_THING_IS_CONTAINED_IN_THE_THING("bisenti", 2)
                        else IF_THING_IS_CONTAINED_IN_THE_THING("fitzgerald", 4)
                        else IF_THING_IS_CONTAINED_IN_THE_THING("jenkins", 4)
                        else IF_THING_IS_CONTAINED_IN_THE_THING("aussie", 5)
                        else IF_THING_IS_CONTAINED_IN_THE_THING("mendoza", 6)
                        else IF_THING_IS_CONTAINED_IN_THE_THING("sarge2", 101)
                        else IF_THING_IS_CONTAINED_IN_THE_THING("sarge", 100)
                        else IF_THING_IS_CONTAINED_IN_THE_THING("johnson", 100)
                        else IF_THING_IS_CONTAINED_IN_THE_THING("lehto", 101)
                        else {
                            variant_numbers[i] = 0;
                        }
                    }
                }
            }
            memcpy(participant->dialogue_variants, variant_numbers, sizeof(variant_numbers));
        }
    }

    return true;
}
