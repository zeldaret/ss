#include <d/a/npc/d_a_npc_bird_zelda.h>

SPECIAL_ACTOR_PROFILE(NPC_BDZ, dAcNpcBdz_c, fProfile::NPC_BDZ, 0x5E, 0, 0);

STATE_DEFINE(dAcNpcBdz_c, Wait);
STATE_DEFINE(dAcNpcBdz_c, Demo);

void dAcNpcBdz_c::initializeState_Wait() {}
void dAcNpcBdz_c::executeState_Wait() {}
void dAcNpcBdz_c::finalizeState_Wait() {}
void dAcNpcBdz_c::initializeState_Demo() {}
void dAcNpcBdz_c::executeState_Demo() {}
void dAcNpcBdz_c::finalizeState_Demo() {}
