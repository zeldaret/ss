#include "d/a/obj/d_a_obj_windmill.h"

SPECIAL_ACTOR_PROFILE(OBJ_WINDMILL, dAcOwindmill_c, fProfile::OBJ_WINDMILL, 0x1DC, 0, 6);

STATE_DEFINE(dAcOwindmill_c, Wait);
STATE_DEFINE(dAcOwindmill_c, CameraMove);
STATE_DEFINE(dAcOwindmill_c, Move);
STATE_DEFINE(dAcOwindmill_c, Collect);
STATE_DEFINE(dAcOwindmill_c, CollectStick);

void dAcOwindmill_c::initializeState_Wait() {}
void dAcOwindmill_c::executeState_Wait() {}
void dAcOwindmill_c::finalizeState_Wait() {}
void dAcOwindmill_c::initializeState_CameraMove() {}
void dAcOwindmill_c::executeState_CameraMove() {}
void dAcOwindmill_c::finalizeState_CameraMove() {}
void dAcOwindmill_c::initializeState_Move() {}
void dAcOwindmill_c::executeState_Move() {}
void dAcOwindmill_c::finalizeState_Move() {}
void dAcOwindmill_c::initializeState_Collect() {}
void dAcOwindmill_c::executeState_Collect() {}
void dAcOwindmill_c::finalizeState_Collect() {}
void dAcOwindmill_c::initializeState_CollectStick() {}
void dAcOwindmill_c::executeState_CollectStick() {}
void dAcOwindmill_c::finalizeState_CollectStick() {}
