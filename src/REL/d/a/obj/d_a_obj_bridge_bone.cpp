#include <d/a/obj/d_a_obj_bridge_bone.h>

SPECIAL_ACTOR_PROFILE(OBJ_BRIDGE_BONE, dAcOBridgeBone_c, fProfile::OBJ_BRIDGE_BONE, 0X1A0, 0, 3);

STATE_DEFINE(dAcOBridgeBone_c, Wait);

void dAcOBridgeBone_c::initializeState_Wait() {}
void dAcOBridgeBone_c::executeState_Wait() {}
void dAcOBridgeBone_c::finalizeState_Wait() {}
