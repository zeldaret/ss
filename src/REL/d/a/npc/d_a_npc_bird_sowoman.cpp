#include "d/a/npc/d_a_npc_bird_sowoman.h"

SPECIAL_ACTOR_PROFILE(NPC_BDSW, dAcNpcBdsw_c, fProfile::NPC_BDSW, 0x5F, 0, 3);

STATE_DEFINE(dAcNpcBdsw_c, Wait);
STATE_DEFINE(dAcNpcBdsw_c, Demo);

void dAcNpcBdsw_c::initializeState_Wait() {}
void dAcNpcBdsw_c::executeState_Wait() {}
void dAcNpcBdsw_c::finalizeState_Wait() {}
void dAcNpcBdsw_c::initializeState_Demo() {}
void dAcNpcBdsw_c::executeState_Demo() {}
void dAcNpcBdsw_c::finalizeState_Demo() {}
