#include "d/a/npc/d_a_npc_rescue.h"

SPECIAL_ACTOR_PROFILE(NPC_RESCUE, dAcNpcRescue_c, fProfile::NPC_RESCUE, 0x93, 0, 0);

STATE_DEFINE(dAcNpcRescue_c, Wait);

void dAcNpcRescue_c::initializeState_Wait() {}
void dAcNpcRescue_c::executeState_Wait() {}
void dAcNpcRescue_c::finalizeState_Wait() {}
