#include <d/a/e/d_a_e_ph.h>

SPECIAL_ACTOR_PROFILE(E_PH, dAcEPh_c, fProfile::E_PH, 0xF1, 0, 3);

STATE_DEFINE(dAcEPh_c, Stop);
STATE_DEFINE(dAcEPh_c, PathMove);

void dAcEPh_c::initializeState_Stop() {}
void dAcEPh_c::executeState_Stop() {}
void dAcEPh_c::finalizeState_Stop() {}
void dAcEPh_c::initializeState_PathMove() {}
void dAcEPh_c::executeState_PathMove() {}
void dAcEPh_c::finalizeState_PathMove() {}
