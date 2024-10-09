#include <d/a/e/d_a_e_eye.h>

SPECIAL_ACTOR_PROFILE(E_EYE, dAcEEye_c, fProfile::E_EYE, 0XF6, 0, 3);

STATE_DEFINE(dAcEEye_c, Open);
STATE_DEFINE(dAcEEye_c, Wait);
STATE_DEFINE(dAcEEye_c, Close);
STATE_DEFINE(dAcEEye_c, Chance);
STATE_DEFINE(dAcEEye_c, Dead);

void dAcEEye_c::initializeState_Open() {}
void dAcEEye_c::executeState_Open() {}
void dAcEEye_c::finalizeState_Open() {}
void dAcEEye_c::initializeState_Wait() {}
void dAcEEye_c::executeState_Wait() {}
void dAcEEye_c::finalizeState_Wait() {}
void dAcEEye_c::initializeState_Close() {}
void dAcEEye_c::executeState_Close() {}
void dAcEEye_c::finalizeState_Close() {}
void dAcEEye_c::initializeState_Chance() {}
void dAcEEye_c::executeState_Chance() {}
void dAcEEye_c::finalizeState_Chance() {}
void dAcEEye_c::initializeState_Dead() {}
void dAcEEye_c::executeState_Dead() {}
void dAcEEye_c::finalizeState_Dead() {}
