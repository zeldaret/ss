#include <d/a/npc/d_a_npc_kobun2.h>

SPECIAL_ACTOR_PROFILE(NPC_KBN2, dAcNpcKbn2_c, fProfile::NPC_KBN2, 0x34, 0, 2);

STATE_DEFINE(dAcNpcKbn2_c, RaceInit);
STATE_DEFINE(dAcNpcKbn2_c, RaceRun);
STATE_DEFINE(dAcNpcKbn2_c, RaceDive);
STATE_DEFINE(dAcNpcKbn2_c, RaceRide);

void dAcNpcKbn2_c::initializeState_RaceInit() {}
void dAcNpcKbn2_c::executeState_RaceInit() {}
void dAcNpcKbn2_c::finalizeState_RaceInit() {}
void dAcNpcKbn2_c::initializeState_RaceRun() {}
void dAcNpcKbn2_c::executeState_RaceRun() {}
void dAcNpcKbn2_c::finalizeState_RaceRun() {}
void dAcNpcKbn2_c::initializeState_RaceDive() {}
void dAcNpcKbn2_c::executeState_RaceDive() {}
void dAcNpcKbn2_c::finalizeState_RaceDive() {}
void dAcNpcKbn2_c::initializeState_RaceRide() {}
void dAcNpcKbn2_c::executeState_RaceRide() {}
void dAcNpcKbn2_c::finalizeState_RaceRide() {}
