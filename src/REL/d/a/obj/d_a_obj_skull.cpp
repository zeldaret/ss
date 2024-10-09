#include <d/a/obj/d_a_obj_skull.h>

SPECIAL_ACTOR_PROFILE(OBJ_SKULL, dAcOSkull_c, fProfile::OBJ_SKULL, 0X165, 0, 3);

STATE_DEFINE(dAcOSkull_c, NormalWait);
STATE_DEFINE(dAcOSkull_c, GrabCarry);
STATE_DEFINE(dAcOSkull_c, BoomerangCarry);
STATE_DEFINE(dAcOSkull_c, WhipCarry);
STATE_DEFINE(dAcOSkull_c, Dead);

void dAcOSkull_c::initializeState_NormalWait() {}
void dAcOSkull_c::executeState_NormalWait() {}
void dAcOSkull_c::finalizeState_NormalWait() {}
void dAcOSkull_c::initializeState_GrabCarry() {}
void dAcOSkull_c::executeState_GrabCarry() {}
void dAcOSkull_c::finalizeState_GrabCarry() {}
void dAcOSkull_c::initializeState_BoomerangCarry() {}
void dAcOSkull_c::executeState_BoomerangCarry() {}
void dAcOSkull_c::finalizeState_BoomerangCarry() {}
void dAcOSkull_c::initializeState_WhipCarry() {}
void dAcOSkull_c::executeState_WhipCarry() {}
void dAcOSkull_c::finalizeState_WhipCarry() {}
void dAcOSkull_c::initializeState_Dead() {}
void dAcOSkull_c::executeState_Dead() {}
void dAcOSkull_c::finalizeState_Dead() {}
