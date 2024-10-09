#include <d/a/obj/d_a_obj_bridge_time.h>

SPECIAL_ACTOR_PROFILE(OBJ_BRIDGE_TIME, dAcOBridgeTime_c, fProfile::OBJ_BRIDGE_TIME, 0X232, 0, 4102);

STATE_DEFINE(dAcOBridgeTime_c, Wait);

void dAcOBridgeTime_c::initializeState_Wait() {}
void dAcOBridgeTime_c::executeState_Wait() {}
void dAcOBridgeTime_c::finalizeState_Wait() {}
