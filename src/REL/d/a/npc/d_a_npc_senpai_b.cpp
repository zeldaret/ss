#include "d/a/npc/d_a_npc_senpai_b.h"

SPECIAL_ACTOR_PROFILE(NPC_SENPAI_B, dAcNpcSenpaiB_c, fProfile::NPC_SENPAI_B, 0x91, 0, 3);

STATE_DEFINE(dAcNpcSenpaiB_c, Think);
STATE_DEFINE(dAcNpcSenpaiB_c, ThinkWait);
STATE_DEFINE(dAcNpcSenpaiB_c, ThinkTurn);
STATE_DEFINE(dAcNpcSenpaiB_c, ThinkRandom);

void dAcNpcSenpaiB_c::initializeState_Think() {}
void dAcNpcSenpaiB_c::executeState_Think() {}
void dAcNpcSenpaiB_c::finalizeState_Think() {}
void dAcNpcSenpaiB_c::initializeState_ThinkWait() {}
void dAcNpcSenpaiB_c::executeState_ThinkWait() {}
void dAcNpcSenpaiB_c::finalizeState_ThinkWait() {}
void dAcNpcSenpaiB_c::initializeState_ThinkTurn() {}
void dAcNpcSenpaiB_c::executeState_ThinkTurn() {}
void dAcNpcSenpaiB_c::finalizeState_ThinkTurn() {}
void dAcNpcSenpaiB_c::initializeState_ThinkRandom() {}
void dAcNpcSenpaiB_c::executeState_ThinkRandom() {}
void dAcNpcSenpaiB_c::finalizeState_ThinkRandom() {}
