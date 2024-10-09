#include <d/a/obj/d_a_obj_ivy_rope.h>

SPECIAL_ACTOR_PROFILE(OBJ_IVY_ROPE, dAcOivyRope_c, fProfile::OBJ_IVY_ROPE, 0X262, 0, 2);

STATE_DEFINE(dAcOivyRope_c, RopeWait);
STATE_DEFINE(dAcOivyRope_c, PlayerGrip);
STATE_DEFINE(dAcOivyRope_c, RopeReturn);
STATE_DEFINE(dAcOivyRope_c, RopeCut);
STATE_DEFINE(dAcOivyRope_c, TerryRopeWait);
STATE_DEFINE(dAcOivyRope_c, TerryRopeUp);
STATE_DEFINE(dAcOivyRope_c, TerryRopeDown);
STATE_DEFINE(dAcOivyRope_c, TerryRopeInit);
STATE_DEFINE(dAcOivyRope_c, TerryRopePlayerCarry);

void dAcOivyRope_c::initializeState_RopeWait() {}
void dAcOivyRope_c::executeState_RopeWait() {}
void dAcOivyRope_c::finalizeState_RopeWait() {}
void dAcOivyRope_c::initializeState_PlayerGrip() {}
void dAcOivyRope_c::executeState_PlayerGrip() {}
void dAcOivyRope_c::finalizeState_PlayerGrip() {}
void dAcOivyRope_c::initializeState_RopeReturn() {}
void dAcOivyRope_c::executeState_RopeReturn() {}
void dAcOivyRope_c::finalizeState_RopeReturn() {}
void dAcOivyRope_c::initializeState_RopeCut() {}
void dAcOivyRope_c::executeState_RopeCut() {}
void dAcOivyRope_c::finalizeState_RopeCut() {}
void dAcOivyRope_c::initializeState_TerryRopeWait() {}
void dAcOivyRope_c::executeState_TerryRopeWait() {}
void dAcOivyRope_c::finalizeState_TerryRopeWait() {}
void dAcOivyRope_c::initializeState_TerryRopeUp() {}
void dAcOivyRope_c::executeState_TerryRopeUp() {}
void dAcOivyRope_c::finalizeState_TerryRopeUp() {}
void dAcOivyRope_c::initializeState_TerryRopeDown() {}
void dAcOivyRope_c::executeState_TerryRopeDown() {}
void dAcOivyRope_c::finalizeState_TerryRopeDown() {}
void dAcOivyRope_c::initializeState_TerryRopeInit() {}
void dAcOivyRope_c::executeState_TerryRopeInit() {}
void dAcOivyRope_c::finalizeState_TerryRopeInit() {}
void dAcOivyRope_c::initializeState_TerryRopePlayerCarry() {}
void dAcOivyRope_c::executeState_TerryRopePlayerCarry() {}
void dAcOivyRope_c::finalizeState_TerryRopePlayerCarry() {}
