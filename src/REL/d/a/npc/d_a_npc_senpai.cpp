#include "d/a/npc/d_a_npc_senpai.h"

SPECIAL_ACTOR_PROFILE(NPC_SENPAI, dAcNpcSenpai_c, fProfile::NPC_SENPAI, 0x90, 0, 3);

STATE_DEFINE(dAcNpcSenpai_c, ReadyWatch1);
STATE_DEFINE(dAcNpcSenpai_c, ReadyWatch2);
STATE_DEFINE(dAcNpcSenpai_c, Watch);
STATE_DEFINE(dAcNpcSenpai_c, Think);

void dAcNpcSenpai_c::initializeState_ReadyWatch1() {}
void dAcNpcSenpai_c::executeState_ReadyWatch1() {}
void dAcNpcSenpai_c::finalizeState_ReadyWatch1() {}
void dAcNpcSenpai_c::initializeState_ReadyWatch2() {}
void dAcNpcSenpai_c::executeState_ReadyWatch2() {}
void dAcNpcSenpai_c::finalizeState_ReadyWatch2() {}
void dAcNpcSenpai_c::initializeState_Watch() {}
void dAcNpcSenpai_c::executeState_Watch() {}
void dAcNpcSenpai_c::finalizeState_Watch() {}
void dAcNpcSenpai_c::initializeState_Think() {}
void dAcNpcSenpai_c::executeState_Think() {}
void dAcNpcSenpai_c::finalizeState_Think() {}
