#include <d/a/npc/d_a_npc_regret_rival.h>

SPECIAL_ACTOR_PROFILE(NPC_REGRET_RIVAL, dAcNpcRegretRvl_c, fProfile::NPC_REGRET_RIVAL, 0X92, 0, 3);

STATE_DEFINE(dAcNpcRegretRvl_c, Ready);
STATE_DEFINE(dAcNpcRegretRvl_c, Punch);

void dAcNpcRegretRvl_c::initializeState_Ready() {}
void dAcNpcRegretRvl_c::executeState_Ready() {}
void dAcNpcRegretRvl_c::finalizeState_Ready() {}
void dAcNpcRegretRvl_c::initializeState_Punch() {}
void dAcNpcRegretRvl_c::executeState_Punch() {}
void dAcNpcRegretRvl_c::finalizeState_Punch() {}
