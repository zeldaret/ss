#include <d/t/d_t_barrel.h>

SPECIAL_ACTOR_PROFILE(TAG_BARREL, dTgBarrel_c, fProfile::TAG_BARREL, 0x221, 0, 0);

STATE_DEFINE(dTgBarrel_c, Wait);
STATE_DEFINE(dTgBarrel_c, Stop);

void dTgBarrel_c::initializeState_Wait() {}
void dTgBarrel_c::executeState_Wait() {}
void dTgBarrel_c::finalizeState_Wait() {}
void dTgBarrel_c::initializeState_Stop() {}
void dTgBarrel_c::executeState_Stop() {}
void dTgBarrel_c::finalizeState_Stop() {}
