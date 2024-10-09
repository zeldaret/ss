#include <d/a/obj/d_a_obj_bridge_building.h>

SPECIAL_ACTOR_PROFILE(OBJ_BRIDGE_BUILDING, dAcObridgeBuilding_c, fProfile::OBJ_BRIDGE_BUILDING, 0X1C8, 0, 2);

STATE_DEFINE(dAcObridgeBuilding_c, Wait);
STATE_DEFINE(dAcObridgeBuilding_c, PreMove);
STATE_DEFINE(dAcObridgeBuilding_c, Move);
STATE_DEFINE(dAcObridgeBuilding_c, Shake);
STATE_DEFINE(dAcObridgeBuilding_c, Across);

void dAcObridgeBuilding_c::initializeState_Wait() {}
void dAcObridgeBuilding_c::executeState_Wait() {}
void dAcObridgeBuilding_c::finalizeState_Wait() {}
void dAcObridgeBuilding_c::initializeState_PreMove() {}
void dAcObridgeBuilding_c::executeState_PreMove() {}
void dAcObridgeBuilding_c::finalizeState_PreMove() {}
void dAcObridgeBuilding_c::initializeState_Move() {}
void dAcObridgeBuilding_c::executeState_Move() {}
void dAcObridgeBuilding_c::finalizeState_Move() {}
void dAcObridgeBuilding_c::initializeState_Shake() {}
void dAcObridgeBuilding_c::executeState_Shake() {}
void dAcObridgeBuilding_c::finalizeState_Shake() {}
void dAcObridgeBuilding_c::initializeState_Across() {}
void dAcObridgeBuilding_c::executeState_Across() {}
void dAcObridgeBuilding_c::finalizeState_Across() {}
