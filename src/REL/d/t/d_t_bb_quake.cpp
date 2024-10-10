#include <d/t/d_t_bb_quake.h>

SPECIAL_ACTOR_PROFILE(T_QUAKE, dTgBBQuake_c, fProfile::T_QUAKE, 0xC2, 0, 0);

STATE_DEFINE(dTgBBQuake_c, Wait);
STATE_DEFINE(dTgBBQuake_c, Wake);

void dTgBBQuake_c::initializeState_Wait() {}
void dTgBBQuake_c::executeState_Wait() {}
void dTgBBQuake_c::finalizeState_Wait() {}
void dTgBBQuake_c::initializeState_Wake() {}
void dTgBBQuake_c::executeState_Wake() {}
void dTgBBQuake_c::finalizeState_Wake() {}
