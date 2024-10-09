#include <d/a/npc/d_a_npc_rival.h>

SPECIAL_ACTOR_PROFILE(NPC_RVL, dAcNpcRvl_c, fProfile::NPC_RVL, 0x31, 0, 2);

STATE_DEFINE(dAcNpcRvl_c, RaceInit);
STATE_DEFINE(dAcNpcRvl_c, RaceRun);
STATE_DEFINE(dAcNpcRvl_c, RaceDive);
STATE_DEFINE(dAcNpcRvl_c, RaceRide);

void dAcNpcRvl_c::initializeState_RaceInit() {}
void dAcNpcRvl_c::executeState_RaceInit() {}
void dAcNpcRvl_c::finalizeState_RaceInit() {}
void dAcNpcRvl_c::initializeState_RaceRun() {}
void dAcNpcRvl_c::executeState_RaceRun() {}
void dAcNpcRvl_c::finalizeState_RaceRun() {}
void dAcNpcRvl_c::initializeState_RaceDive() {}
void dAcNpcRvl_c::executeState_RaceDive() {}
void dAcNpcRvl_c::finalizeState_RaceDive() {}
void dAcNpcRvl_c::initializeState_RaceRide() {}
void dAcNpcRvl_c::executeState_RaceRide() {}
void dAcNpcRvl_c::finalizeState_RaceRide() {}
