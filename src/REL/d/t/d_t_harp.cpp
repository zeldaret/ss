#include "d/t/d_t_harp.h"

SPECIAL_ACTOR_PROFILE(HARP_TAG, dTgHarp_c, fProfile::HARP_TAG, 0x1A4, 0, 0);

STATE_DEFINE(dTgHarp_c, Wait);

s16 dTgHarp_c::fn_536_420() {
    return 1;
}

unsigned short fn_536_690(unsigned short *param_1) {
    if (*param_1 != 0) {
        *param_1 = *param_1 + -1;
    }
    return *param_1;
}

short fn_536_530() {
    return 1;
}

short fn_536_540(dTgHarp_c *param_1) {
    param_1->mStateMgr.executeState();
    return 1;
}

void dTgHarp_c::initializeState_Wait() {}
void dTgHarp_c::executeState_Wait() {}
void dTgHarp_c::finalizeState_Wait() {}
