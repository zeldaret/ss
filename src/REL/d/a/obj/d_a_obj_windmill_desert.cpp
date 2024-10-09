#include <d/a/obj/d_a_obj_windmill_desert.h>

SPECIAL_ACTOR_PROFILE(OBJ_WINDMILL_DESERT, dAcOwindmillDesert_c, fProfile::OBJ_WINDMILL_DESERT, 0X20B, 0, 6);

STATE_DEFINE(dAcOwindmillDesert_c, Wait);
STATE_DEFINE(dAcOwindmillDesert_c, Acceleration);
STATE_DEFINE(dAcOwindmillDesert_c, RollMaxSpeed);
STATE_DEFINE(dAcOwindmillDesert_c, Deceleration);

void dAcOwindmillDesert_c::initializeState_Wait() {}
void dAcOwindmillDesert_c::executeState_Wait() {}
void dAcOwindmillDesert_c::finalizeState_Wait() {}
void dAcOwindmillDesert_c::initializeState_Acceleration() {}
void dAcOwindmillDesert_c::executeState_Acceleration() {}
void dAcOwindmillDesert_c::finalizeState_Acceleration() {}
void dAcOwindmillDesert_c::initializeState_RollMaxSpeed() {}
void dAcOwindmillDesert_c::executeState_RollMaxSpeed() {}
void dAcOwindmillDesert_c::finalizeState_RollMaxSpeed() {}
void dAcOwindmillDesert_c::initializeState_Deceleration() {}
void dAcOwindmillDesert_c::executeState_Deceleration() {}
void dAcOwindmillDesert_c::finalizeState_Deceleration() {}
