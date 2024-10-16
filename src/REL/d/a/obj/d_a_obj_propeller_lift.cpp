#include "d/a/obj/d_a_obj_propeller_lift.h"

SPECIAL_ACTOR_PROFILE(OBJ_PROPELLER_LIFT, dAcOPropellerLift_c, fProfile::OBJ_PROPELLER_LIFT, 0x156, 0, 4099);

STATE_DEFINE(dAcOPropellerLift_c, Normal);
STATE_DEFINE(dAcOPropellerLift_c, BackMove);
STATE_DEFINE(dAcOPropellerLift_c, BackMoveMain);
STATE_DEFINE(dAcOPropellerLift_c, CrashAction);

void dAcOPropellerLift_c::initializeState_Normal() {}
void dAcOPropellerLift_c::executeState_Normal() {}
void dAcOPropellerLift_c::finalizeState_Normal() {}
void dAcOPropellerLift_c::initializeState_BackMove() {}
void dAcOPropellerLift_c::executeState_BackMove() {}
void dAcOPropellerLift_c::finalizeState_BackMove() {}
void dAcOPropellerLift_c::initializeState_BackMoveMain() {}
void dAcOPropellerLift_c::executeState_BackMoveMain() {}
void dAcOPropellerLift_c::finalizeState_BackMoveMain() {}
void dAcOPropellerLift_c::initializeState_CrashAction() {}
void dAcOPropellerLift_c::executeState_CrashAction() {}
void dAcOPropellerLift_c::finalizeState_CrashAction() {}
