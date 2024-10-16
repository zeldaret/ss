#include "d/a/obj/d_a_obj_truck.h"

SPECIAL_ACTOR_PROFILE(OBJ_TRUCK, dAcOtruck_c, fProfile::OBJ_TRUCK, 0x26A, 0, 2);

STATE_DEFINE(dAcOtruck_c, Wait);
STATE_DEFINE(dAcOtruck_c, Ready);
STATE_DEFINE(dAcOtruck_c, PathMove);
STATE_DEFINE(dAcOtruck_c, Sink);

void dAcOtruck_c::initializeState_Wait() {}
void dAcOtruck_c::executeState_Wait() {}
void dAcOtruck_c::finalizeState_Wait() {}
void dAcOtruck_c::initializeState_Ready() {}
void dAcOtruck_c::executeState_Ready() {}
void dAcOtruck_c::finalizeState_Ready() {}
void dAcOtruck_c::initializeState_PathMove() {}
void dAcOtruck_c::executeState_PathMove() {}
void dAcOtruck_c::finalizeState_PathMove() {}
void dAcOtruck_c::initializeState_Sink() {}
void dAcOtruck_c::executeState_Sink() {}
void dAcOtruck_c::finalizeState_Sink() {}
