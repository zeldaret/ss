#include "d/a/obj/d_a_obj_bridge_fall.h"

SPECIAL_ACTOR_PROFILE(OBJ_BRIDGE_FALL, dAcOBridgeFall_c, fProfile::OBJ_BRIDGE_FALL, 0x230, 0, 6);

STATE_DEFINE(dAcOBridgeFall_c, Wait);
STATE_DEFINE(dAcOBridgeFall_c, Fall);
STATE_DEFINE(dAcOBridgeFall_c, Shake);

void dAcOBridgeFall_c::initializeState_Wait() {}
void dAcOBridgeFall_c::executeState_Wait() {}
void dAcOBridgeFall_c::finalizeState_Wait() {}
void dAcOBridgeFall_c::initializeState_Fall() {}
void dAcOBridgeFall_c::executeState_Fall() {}
void dAcOBridgeFall_c::finalizeState_Fall() {}
void dAcOBridgeFall_c::initializeState_Shake() {}
void dAcOBridgeFall_c::executeState_Shake() {}
void dAcOBridgeFall_c::finalizeState_Shake() {}
