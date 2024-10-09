#include <d/t/d_t_mist.h>

SPECIAL_ACTOR_PROFILE(MIST_TAG, dMistTag_c, fProfile::MIST_TAG, 0XE, 0, 388);

STATE_DEFINE(dMistTag_c, Wait);

void dMistTag_c::initializeState_Wait() {}
void dMistTag_c::executeState_Wait() {}
void dMistTag_c::finalizeState_Wait() {}
