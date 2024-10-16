#include "d/a/obj/d_a_obj_switch_shutter.h"

SPECIAL_ACTOR_PROFILE(OBJ_SWITCH_SHUTTER, dAcOSwShutter_c, fProfile::OBJ_SWITCH_SHUTTER, 0x13A, 0, 3);

STATE_DEFINE(dAcOSwShutter_c, Wait);
STATE_DEFINE(dAcOSwShutter_c, Move);
STATE_DEFINE(dAcOSwShutter_c, WaitEventEnd);

void dAcOSwShutter_c::initializeState_Wait() {}
void dAcOSwShutter_c::executeState_Wait() {}
void dAcOSwShutter_c::finalizeState_Wait() {}
void dAcOSwShutter_c::initializeState_Move() {}
void dAcOSwShutter_c::executeState_Move() {}
void dAcOSwShutter_c::finalizeState_Move() {}
void dAcOSwShutter_c::initializeState_WaitEventEnd() {}
void dAcOSwShutter_c::executeState_WaitEventEnd() {}
void dAcOSwShutter_c::finalizeState_WaitEventEnd() {}
