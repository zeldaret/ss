#include <d/a/obj/d_a_obj_bridge_stretch.h>

SPECIAL_ACTOR_PROFILE(OBJ_BRIDGE_STRETCH, dAcObridgeStretch_c, fProfile::OBJ_BRIDGE_STRETCH, 0x1CC, 0, 2);

STATE_DEFINE(dAcObridgeStretch_c, Wait);
STATE_DEFINE(dAcObridgeStretch_c, Move);
STATE_DEFINE(dAcObridgeStretch_c, Shake);
STATE_DEFINE(dAcObridgeStretch_c, Stretch);

void dAcObridgeStretch_c::initializeState_Wait() {}
void dAcObridgeStretch_c::executeState_Wait() {}
void dAcObridgeStretch_c::finalizeState_Wait() {}
void dAcObridgeStretch_c::initializeState_Move() {}
void dAcObridgeStretch_c::executeState_Move() {}
void dAcObridgeStretch_c::finalizeState_Move() {}
void dAcObridgeStretch_c::initializeState_Shake() {}
void dAcObridgeStretch_c::executeState_Shake() {}
void dAcObridgeStretch_c::finalizeState_Shake() {}
void dAcObridgeStretch_c::initializeState_Stretch() {}
void dAcObridgeStretch_c::executeState_Stretch() {}
void dAcObridgeStretch_c::finalizeState_Stretch() {}
