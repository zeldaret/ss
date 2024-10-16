#include "d/a/obj/d_a_obj_shed.h"

SPECIAL_ACTOR_PROFILE(OBJ_SHED, dAcOShed_c, fProfile::OBJ_SHED, 0x256, 0, 6);

STATE_DEFINE(dAcOShed_c, Wait);
STATE_DEFINE(dAcOShed_c, Move);

void dAcOShed_c::initializeState_Wait() {}
void dAcOShed_c::executeState_Wait() {}
void dAcOShed_c::finalizeState_Wait() {}
void dAcOShed_c::initializeState_Move() {}
void dAcOShed_c::executeState_Move() {}
void dAcOShed_c::finalizeState_Move() {}
