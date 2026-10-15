#include "d/a/npc/d_a_npc_knight_leader.h"

#include "d/d_sc_game.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(NPC_KNIGHT_LEADER, dAcNpcKnightLeader_c, fProfile::NPC_KNIGHT_LEADER, 0x8F, 0, 2);

static const char *sLordsLogComment = "LordsLogComment";

void dAcNpcKnightLeader_c::triggerLordsLogCommentEvent(dAcNpcKnightLeader_c *actor) {
    {
        Event event(sLordsLogComment, 0x190, 0x100001, nullptr, nullptr);
        EventManager::alsoSetAsCurrentEvent(actor, &event, nullptr);
    }
    actor->field_0x104C = dScGame_c::getUpdateFrameCount();
}
