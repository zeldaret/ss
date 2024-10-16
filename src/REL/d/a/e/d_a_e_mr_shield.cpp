#include "d/a/e/d_a_e_mr_shield.h"

SPECIAL_ACTOR_PROFILE(E_MR_SHIELD, dAcE_Mr_Shield_c, fProfile::E_MR_SHIELD, 0x121, 0, 3);

STATE_DEFINE(dAcE_Mr_Shield_c, Move);
STATE_DEFINE(dAcE_Mr_Shield_c, Wait);

void dAcE_Mr_Shield_c::initializeState_Move() {}
void dAcE_Mr_Shield_c::executeState_Move() {}
void dAcE_Mr_Shield_c::finalizeState_Move() {}
void dAcE_Mr_Shield_c::initializeState_Wait() {}
void dAcE_Mr_Shield_c::executeState_Wait() {}
void dAcE_Mr_Shield_c::finalizeState_Wait() {}
