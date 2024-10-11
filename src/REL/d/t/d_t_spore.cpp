#include <d/t/d_t_spore.h>

SPECIAL_ACTOR_PROFILE(SPORE_TAG, dSporeTag_c, fProfile::SPORE_TAG, 0xD, 0, 388);

STATE_DEFINE(dSporeTag_c, Wait);

void dSporeTag_c::initializeState_Wait() {}
void dSporeTag_c::executeState_Wait() {}
void dSporeTag_c::finalizeState_Wait() {}
