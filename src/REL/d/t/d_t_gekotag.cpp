#include "d/t/d_t_gekotag.h"

SPECIAL_ACTOR_PROFILE(GEKO_TAG, dTgGekoTag, fProfile::GEKO_TAG, 0xEE, 0, 0);

STATE_DEFINE(dTgGekoTag, Wait);

void dTgGekoTag::initializeState_Wait() {}
void dTgGekoTag::executeState_Wait() {}
void dTgGekoTag::finalizeState_Wait() {}
