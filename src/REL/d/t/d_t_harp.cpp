#include <d/t/d_t_harp.h>

SPECIAL_ACTOR_PROFILE(HARP_TAG, dTgHarp_c, fProfile::HARP_TAG, 0X1A4, 0, 0);

STATE_DEFINE(dTgHarp_c, Wait);

void dTgHarp_c::initializeState_Wait() {}
void dTgHarp_c::executeState_Wait() {}
void dTgHarp_c::finalizeState_Wait() {}
