#include <d/a/npc/d_a_npc_bird.h>

SPECIAL_ACTOR_PROFILE(NPC_BIRD, dNpc_Bird_c, fProfile::NPC_BIRD, 0X120, 0, 2);

STATE_DEFINE(dNpc_Bird_c, Wait);

void dNpc_Bird_c::initializeState_Wait() {}
void dNpc_Bird_c::executeState_Wait() {}
void dNpc_Bird_c::finalizeState_Wait() {}
