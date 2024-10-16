#include "d/a/npc/d_a_npc_kobun.h"

SPECIAL_ACTOR_PROFILE(NPC_KBN, dAcNpcKbn_c, fProfile::NPC_KBN, 0x33, 0, 2);

STATE_DEFINE(dAcNpcKbn_c, RaceInit);
STATE_DEFINE(dAcNpcKbn_c, RaceRun);
STATE_DEFINE(dAcNpcKbn_c, RaceDive);
STATE_DEFINE(dAcNpcKbn_c, RaceRide);
STATE_DEFINE(dAcNpcKbn_c, LetterWait);
STATE_DEFINE(dAcNpcKbn_c, Sleep);
STATE_DEFINE(dAcNpcKbn_c, Sit);

void dAcNpcKbn_c::initializeState_RaceInit() {}
void dAcNpcKbn_c::executeState_RaceInit() {}
void dAcNpcKbn_c::finalizeState_RaceInit() {}
void dAcNpcKbn_c::initializeState_RaceRun() {}
void dAcNpcKbn_c::executeState_RaceRun() {}
void dAcNpcKbn_c::finalizeState_RaceRun() {}
void dAcNpcKbn_c::initializeState_RaceDive() {}
void dAcNpcKbn_c::executeState_RaceDive() {}
void dAcNpcKbn_c::finalizeState_RaceDive() {}
void dAcNpcKbn_c::initializeState_RaceRide() {}
void dAcNpcKbn_c::executeState_RaceRide() {}
void dAcNpcKbn_c::finalizeState_RaceRide() {}
void dAcNpcKbn_c::initializeState_LetterWait() {}
void dAcNpcKbn_c::executeState_LetterWait() {}
void dAcNpcKbn_c::finalizeState_LetterWait() {}
void dAcNpcKbn_c::initializeState_Sleep() {}
void dAcNpcKbn_c::executeState_Sleep() {}
void dAcNpcKbn_c::finalizeState_Sleep() {}
void dAcNpcKbn_c::initializeState_Sit() {}
void dAcNpcKbn_c::executeState_Sit() {}
void dAcNpcKbn_c::finalizeState_Sit() {}
