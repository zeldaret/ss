#include "d/a/obj/d_a_obj_bb_bridge.h"

SPECIAL_ACTOR_PROFILE(OBJ_BB_BRIDGE, dAcObbBridge_c, fProfile::OBJ_BB_BRIDGE, 0x1A1, 0, 7);

STATE_DEFINE(dAcObbBridge_c, Wait);
STATE_DEFINE(dAcObbBridge_c, BrokenDemo);
STATE_DEFINE(dAcObbBridge_c, Broken);

void dAcObbBridge_c::initializeState_Wait() {}
void dAcObbBridge_c::executeState_Wait() {}
void dAcObbBridge_c::finalizeState_Wait() {}
void dAcObbBridge_c::initializeState_BrokenDemo() {}
void dAcObbBridge_c::executeState_BrokenDemo() {}
void dAcObbBridge_c::finalizeState_BrokenDemo() {}
void dAcObbBridge_c::initializeState_Broken() {}
void dAcObbBridge_c::executeState_Broken() {}
void dAcObbBridge_c::finalizeState_Broken() {}
