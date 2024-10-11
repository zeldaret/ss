#include <d/a/npc/d_a_npc_shinkan.h>

SPECIAL_ACTOR_PROFILE(NPC_SKN, dAcNpcSkn_c, fProfile::NPC_SKN, 0x36, 0, 0);

STATE_DEFINE(dAcNpcSkn_c, RaceWatch);
STATE_DEFINE(dAcNpcSkn_c, RaceEvt);

void dAcNpcSkn_c::initializeState_RaceWatch() {}
void dAcNpcSkn_c::executeState_RaceWatch() {}
void dAcNpcSkn_c::finalizeState_RaceWatch() {}
void dAcNpcSkn_c::initializeState_RaceEvt() {}
void dAcNpcSkn_c::executeState_RaceEvt() {}
void dAcNpcSkn_c::finalizeState_RaceEvt() {}
