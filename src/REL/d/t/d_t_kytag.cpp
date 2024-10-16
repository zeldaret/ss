#include "d/t/d_t_kytag.h"

SPECIAL_ACTOR_PROFILE(KYTAG_TAG, dKytagTag_c, fProfile::KYTAG_TAG, 0x1B, 0, 388);

STATE_DEFINE(dKytagTag_c, Wait);

void dKytagTag_c::initializeState_Wait() {}
void dKytagTag_c::executeState_Wait() {}
void dKytagTag_c::finalizeState_Wait() {}
