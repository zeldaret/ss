#include "d/a/obj/d_a_obj_sw_sword_beam.h"

SPECIAL_ACTOR_PROFILE(OBJ_SW_SWORD_BEAM, dAcOSwSwordBeam_c, fProfile::OBJ_SW_SWORD_BEAM, 0x1A2, 0, 3);

STATE_DEFINE(dAcOSwSwordBeam_c, Wait);
STATE_DEFINE(dAcOSwSwordBeam_c, Rotate);
STATE_DEFINE(dAcOSwSwordBeam_c, RotateEnd);
STATE_DEFINE(dAcOSwSwordBeam_c, OnSwitch);
STATE_DEFINE(dAcOSwSwordBeam_c, End);

void dAcOSwSwordBeam_c::initializeState_Wait() {}
void dAcOSwSwordBeam_c::executeState_Wait() {}
void dAcOSwSwordBeam_c::finalizeState_Wait() {}
void dAcOSwSwordBeam_c::initializeState_Rotate() {}
void dAcOSwSwordBeam_c::executeState_Rotate() {}
void dAcOSwSwordBeam_c::finalizeState_Rotate() {}
void dAcOSwSwordBeam_c::initializeState_RotateEnd() {}
void dAcOSwSwordBeam_c::executeState_RotateEnd() {}
void dAcOSwSwordBeam_c::finalizeState_RotateEnd() {}
void dAcOSwSwordBeam_c::initializeState_OnSwitch() {}
void dAcOSwSwordBeam_c::executeState_OnSwitch() {}
void dAcOSwSwordBeam_c::finalizeState_OnSwitch() {}
void dAcOSwSwordBeam_c::initializeState_End() {}
void dAcOSwSwordBeam_c::executeState_End() {}
void dAcOSwSwordBeam_c::finalizeState_End() {}
