#include "d/t/d_t_harp.h"

SPECIAL_ACTOR_PROFILE(HARP_TAG, dTgHarp_c, fProfile::HARP_TAG, 0x1A4, 0, 0);

STATE_DEFINE(dTgHarp_c, Wait);

s16 dTgHarp_c::fn_536_420() {
    return 1;
}

s16 dTgHarp_c::fn_536_530() {
    return 1;
}

s16 dTgHarp_c::fn_536_540() {
    mStateMgr.executeState();
    return 1;
}

void dTgHarp_c::initializeState_Wait() {}
void dTgHarp_c::executeState_Wait() {}
void dTgHarp_c::finalizeState_Wait() {}
