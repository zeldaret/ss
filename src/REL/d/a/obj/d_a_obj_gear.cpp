#include <d/a/obj/d_a_obj_gear.h>

SPECIAL_ACTOR_PROFILE(OBJ_GEAR, dAcOGear_c, fProfile::OBJ_GEAR, 0X233, 0, 7);

STATE_DEFINE(dAcOGear_c, Wait);
STATE_DEFINE(dAcOGear_c, Rotation);
STATE_DEFINE(dAcOGear_c, Deboost);
STATE_DEFINE(dAcOGear_c, Stop);

void dAcOGear_c::initializeState_Wait() {}
void dAcOGear_c::executeState_Wait() {}
void dAcOGear_c::finalizeState_Wait() {}
void dAcOGear_c::initializeState_Rotation() {}
void dAcOGear_c::executeState_Rotation() {}
void dAcOGear_c::finalizeState_Rotation() {}
void dAcOGear_c::initializeState_Deboost() {}
void dAcOGear_c::executeState_Deboost() {}
void dAcOGear_c::finalizeState_Deboost() {}
void dAcOGear_c::initializeState_Stop() {}
void dAcOGear_c::executeState_Stop() {}
void dAcOGear_c::finalizeState_Stop() {}
