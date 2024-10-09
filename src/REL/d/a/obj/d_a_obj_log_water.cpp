#include <d/a/obj/d_a_obj_log_water.h>

SPECIAL_ACTOR_PROFILE(OBJ_LOG_WATER, dAcOlogWater_c, fProfile::OBJ_LOG_WATER, 0x12F, 0, 3);

STATE_DEFINE(dAcOlogWater_c, Wait);

void dAcOlogWater_c::initializeState_Wait() {}
void dAcOlogWater_c::executeState_Wait() {}
void dAcOlogWater_c::finalizeState_Wait() {}
