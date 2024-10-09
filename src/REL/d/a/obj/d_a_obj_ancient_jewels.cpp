#include <d/a/obj/d_a_obj_ancient_jewels.h>

SPECIAL_ACTOR_PROFILE(OBJ_ANCIENT_JEWELS, dAcOAncientJewels_c, fProfile::OBJ_ANCIENT_JEWELS, 0x14C, 0, 3);

STATE_DEFINE(dAcOAncientJewels_c, Wait);
STATE_DEFINE(dAcOAncientJewels_c, WaitDrop);
STATE_DEFINE(dAcOAncientJewels_c, Drop);
STATE_DEFINE(dAcOAncientJewels_c, Buried);
STATE_DEFINE(dAcOAncientJewels_c, WhipCarry);
STATE_DEFINE(dAcOAncientJewels_c, GetItem);
STATE_DEFINE(dAcOAncientJewels_c, UnderLog);

void dAcOAncientJewels_c::initializeState_Wait() {}
void dAcOAncientJewels_c::executeState_Wait() {}
void dAcOAncientJewels_c::finalizeState_Wait() {}
void dAcOAncientJewels_c::initializeState_WaitDrop() {}
void dAcOAncientJewels_c::executeState_WaitDrop() {}
void dAcOAncientJewels_c::finalizeState_WaitDrop() {}
void dAcOAncientJewels_c::initializeState_Drop() {}
void dAcOAncientJewels_c::executeState_Drop() {}
void dAcOAncientJewels_c::finalizeState_Drop() {}
void dAcOAncientJewels_c::initializeState_Buried() {}
void dAcOAncientJewels_c::executeState_Buried() {}
void dAcOAncientJewels_c::finalizeState_Buried() {}
void dAcOAncientJewels_c::initializeState_WhipCarry() {}
void dAcOAncientJewels_c::executeState_WhipCarry() {}
void dAcOAncientJewels_c::finalizeState_WhipCarry() {}
void dAcOAncientJewels_c::initializeState_GetItem() {}
void dAcOAncientJewels_c::executeState_GetItem() {}
void dAcOAncientJewels_c::finalizeState_GetItem() {}
void dAcOAncientJewels_c::initializeState_UnderLog() {}
void dAcOAncientJewels_c::executeState_UnderLog() {}
void dAcOAncientJewels_c::finalizeState_UnderLog() {}
