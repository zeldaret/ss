#include <d/a/obj/d_a_obj_bridge_step.h>

SPECIAL_ACTOR_PROFILE(OBJ_BRIDGE_STEP, dAcOBridgeStep_c, fProfile::OBJ_BRIDGE_STEP, 0x231, 0, 7);

STATE_DEFINE(dAcOBridgeStep_c, Wait);
STATE_DEFINE(dAcOBridgeStep_c, SwitchPushed);
STATE_DEFINE(dAcOBridgeStep_c, BridgeUp);
STATE_DEFINE(dAcOBridgeStep_c, BridgeUpAll);

void dAcOBridgeStep_c::initializeState_Wait() {}
void dAcOBridgeStep_c::executeState_Wait() {}
void dAcOBridgeStep_c::finalizeState_Wait() {}
void dAcOBridgeStep_c::initializeState_SwitchPushed() {}
void dAcOBridgeStep_c::executeState_SwitchPushed() {}
void dAcOBridgeStep_c::finalizeState_SwitchPushed() {}
void dAcOBridgeStep_c::initializeState_BridgeUp() {}
void dAcOBridgeStep_c::executeState_BridgeUp() {}
void dAcOBridgeStep_c::finalizeState_BridgeUp() {}
void dAcOBridgeStep_c::initializeState_BridgeUpAll() {}
void dAcOBridgeStep_c::executeState_BridgeUpAll() {}
void dAcOBridgeStep_c::finalizeState_BridgeUpAll() {}
