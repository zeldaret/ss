#include <d/a/obj/d_a_obj_pinwheel.h>

SPECIAL_ACTOR_PROFILE(OBJ_PINWHEEL, dAcOpinwheel_c, fProfile::OBJ_PINWHEEL, 0X1DD, 0, 2);

STATE_DEFINE(dAcOpinwheel_c, Wait);
STATE_DEFINE(dAcOpinwheel_c, Acceleration);
STATE_DEFINE(dAcOpinwheel_c, RollMaxSpeed);
STATE_DEFINE(dAcOpinwheel_c, Deceleration);

void dAcOpinwheel_c::initializeState_Wait() {}
void dAcOpinwheel_c::executeState_Wait() {}
void dAcOpinwheel_c::finalizeState_Wait() {}
void dAcOpinwheel_c::initializeState_Acceleration() {}
void dAcOpinwheel_c::executeState_Acceleration() {}
void dAcOpinwheel_c::finalizeState_Acceleration() {}
void dAcOpinwheel_c::initializeState_RollMaxSpeed() {}
void dAcOpinwheel_c::executeState_RollMaxSpeed() {}
void dAcOpinwheel_c::finalizeState_RollMaxSpeed() {}
void dAcOpinwheel_c::initializeState_Deceleration() {}
void dAcOpinwheel_c::executeState_Deceleration() {}
void dAcOpinwheel_c::finalizeState_Deceleration() {}
