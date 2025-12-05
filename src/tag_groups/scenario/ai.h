#pragma once

#include <stdint.h>

#include "../../data_types.h"
#include "../../tag/tag.h"

#define AI_CONVERSATION_DIALOGUE_VARIANT_COUNT 6

enum {
    AI_CONVERSATION_FLAGS_STOP_IF_ANYONE_DIES_BIT,
    AI_CONVERSATION_FLAGS_STOP_IF_DAMAGED_BIT,
    AI_CONVERSATION_FLAGS_STOP_IF_VISIBLE_ENEMY_BIT,
    AI_CONVERSATION_FLAGS_STOP_IF_ALERTED_TO_ENEMY_BIT,
    AI_CONVERSATION_FLAGS_PLAYER_MUST_BE_VISIBLE_BIT,
    AI_CONVERSATION_FLAGS_STOP_OTHER_ACTIONS_BIT,
    AI_CONVERSATION_FLAGS_KEEP_TRYING_TO_PLAY_BIT,
    AI_CONVERSATION_FLAGS_PLAYER_MUST_BE_LOOKING_AT_BIT,
    NUMBER_OF_AI_CONVERSATION_FLAGS
};

enum {
    AI_CONVERSATION_PARTICIPANT_FLAGS_OPTIONAL_BIT,
    AI_CONVERSATION_PARTICIPANT_FLAGS_HAS_ALTERNATE_BIT,
    AI_CONVERSATION_PARTICIPANT_FLAGS_IS_ALTERNATE_BIT,
    NUMBER_OF_AI_CONVERSATION_PARTICIPANT_FLAGS
};

enum {
    AI_CONVERSATION_LINE_FLAGS_ADDRESSEE_LOOK_BACK_BIT,
    AI_CONVERSATION_LINE_FLAGS_EVERYONE_LOOK_AT_SPEAKER_BIT,
    AI_CONVERSATION_LINE_FLAGS_EVERYONE_LOOK_AT_ADDRESSEE_BIT,
    AI_CONVERSATION_LINE_FLAGS_WAIT_AFTER_UNTIL_TOLD_TO_ADVANCE_BIT,
    AI_CONVERSATION_LINE_FLAGS_WAIT_UNTIL_SPEAKER_NEARBY_BIT,
    AI_CONVERSATION_LINE_FLAGS_WAIT_UNTIL_EVERYONE_NEARBY_BIT,
    NUMBER_OF_AI_CONVERSATION_LINE_FLAGS
};

enum {
    AI_CONVERSATION_SELECTION_TYPE_FRIENDLY_ACTOR,
    AI_CONVERSATION_SELECTION_TYPE_DISEMBODIED,
    AI_CONVERSATION_SELECTION_TYPE_IN_PLAYER_VEHICLE,
    AI_CONVERSATION_SELECTION_TYPE_NOT_IN_VEHICLE,
    AI_CONVERSATION_SELECTION_TYPE_SARGEANT,
    AI_CONVERSATION_SELECTION_TYPE_ANY_ACTOR,
    AI_CONVERSATION_SELECTION_TYPE_RADIO,
    AI_CONVERSATION_SELECTION_TYPE_RADIO_SARGEANT,
    NUMBER_OF_AI_CONVERSATION_SELECTION_TYPES
};

enum {
    AI_CONVERSATION_ADDRESS_TYPE_NONE,
    AI_CONVERSATION_ADDRESS_TYPE_PLAYER,
    AI_CONVERSATION_ADDRESS_TYPE_PARTICIPANT,
    NUMBER_OF_AI_CONVERSATION_ADDRESS_TYPES
};

struct ai_conversation_participant {
    uint16_t pad;
    uint16_t flags;
    uint16_t selection_type;
    uint16_t actor_type;
    uint16_t preexisting_object_name_index;
    uint16_t new_attach_object_name_index;
    uint32_t unused[3];
    int16_t dialogue_variants[AI_CONVERSATION_DIALOGUE_VARIANT_COUNT];
    String32 ai_index_name;
    uint32_t runtime_ai_index;
    uint32_t unused2[3];
};
static_assert(sizeof(struct ai_conversation_participant) == 84);

struct ai_conversation_line {
    uint16_t flags;
    uint16_t participant_index;
    uint16_t address_type;
    uint16_t address_participant_index;
    uint32_t unused;
    float delay_time;
    uint32_t unused2[3];
    struct tag_reference dialogue[AI_CONVERSATION_DIALOGUE_VARIANT_COUNT];
};
static_assert(sizeof(struct ai_conversation_line) == 124);

struct ai_conversation {
    String32 name;
    uint16_t flags;
    uint16_t pad;
    float trigger_dist;
    float run_to_player_dist;
    uint32_t unused[9];
    struct tag_reflexive participants;
    struct tag_reflexive lines;
    uint32_t unused2[3];
};
static_assert(sizeof(struct ai_conversation) == 116);

#define scenario_get_ai_conversation(sceanrio, index, data) tag_reflexive_get_element(&(scenario)->ai_conversations, index, sizeof(struct ai_conversation), data)
#define scenario_get_ai_conversation_participant(conversation, index, data) tag_reflexive_get_element(&(conversation)->participants, index, sizeof(struct ai_conversation_participant), data)
#define scenario_get_ai_conversation_line(conversation, index, data) tag_reflexive_get_element(&(conversation)->lines, index, sizeof(struct ai_conversation_line), data)
