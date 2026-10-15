#include "d/a/npc/d_a_npc_knight_leader.h"

#include "d/d_sc_game.h"
#include "toBeSorted/event_manager.h"

static int lbl_5_data_0 = 0x00000000;
static int lbl_5_data_4 = 0xFFFFF05C;
static int lbl_5_data_8 = 0x0000038E;
static int lbl_5_data_C = 0xFFFFFC72;
static int lbl_5_data_10 = 0x0000071C;
static int lbl_5_data_14 = 0xFFFFF6C2;
static int lbl_5_data_18 = 0x00000E38;
static int lbl_5_data_1C = 0xFFFFF1C8;
static int lbl_5_data_20 = 0x00001C70;
static int lbl_5_data_24 = 0xFFFFE390;
static int lbl_5_data_28 = 0x00002710;
static int lbl_5_data_2C = 0xFFFFD8F0;

extern "C" void fn_5_F0(void *self, s16 *out) {
    out[0] = lbl_5_data_0;
    out[1] = lbl_5_data_4;
}

extern "C" void fn_5_110(void *self, s16 *out) {
    out[0] = lbl_5_data_8;
    out[1] = lbl_5_data_C;
}

extern "C" void fn_5_130(void *self, s16 *out) {
    out[0] = lbl_5_data_10;
    out[1] = lbl_5_data_14;
}

extern "C" void fn_5_150(void *self, s16 *out) {
    out[0] = lbl_5_data_18;
    out[1] = lbl_5_data_1C;
}

extern "C" void fn_5_170(void *self, s16 *out) {
    out[0] = lbl_5_data_20;
    out[1] = lbl_5_data_24;
}

extern "C" void fn_5_190(void *self, s16 *out) {
    out[0] = lbl_5_data_28;
    out[1] = lbl_5_data_2C;
}

SPECIAL_ACTOR_PROFILE(NPC_KNIGHT_LEADER, dAcNpcKnightLeader_c, fProfile::NPC_KNIGHT_LEADER, 0x8F, 0, 2);

static const char *sEvents[] = {"LordsLogComment", nullptr};

void dAcNpcKnightLeader_c::triggerLordsLogCommentEvent(dAcNpcKnightLeader_c *actor) {
    {
        Event event = Event(sEvents[0], 0x190, 0x100001, nullptr, nullptr);
        EventManager::alsoSetAsCurrentEvent(actor, &event, nullptr);
    }
    actor->field_0x104C = dScGame_c::getUpdateFrameCount();
}
