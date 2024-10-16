#include "d/a/obj/d_a_obj_move_elec.h"

SPECIAL_ACTOR_PROFILE(OBJ_MOVE_ELEC, dAcOMoveElec_c, fProfile::OBJ_MOVE_ELEC, 0x1A8, 0, 6);

STATE_DEFINE(dAcOMoveElec_c, WaitOff);
STATE_DEFINE(dAcOMoveElec_c, WaitOn);
STATE_DEFINE(dAcOMoveElec_c, Move);

void dAcOMoveElec_c::initializeState_WaitOff() {}
void dAcOMoveElec_c::executeState_WaitOff() {}
void dAcOMoveElec_c::finalizeState_WaitOff() {}
void dAcOMoveElec_c::initializeState_WaitOn() {}
void dAcOMoveElec_c::executeState_WaitOn() {}
void dAcOMoveElec_c::finalizeState_WaitOn() {}
void dAcOMoveElec_c::initializeState_Move() {}
void dAcOMoveElec_c::executeState_Move() {}
void dAcOMoveElec_c::finalizeState_Move() {}
