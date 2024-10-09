#include <d/a/obj/d_a_obj_shutter_waterD101.h>

SPECIAL_ACTOR_PROFILE(OBJ_SHUTTER_WATER_D101, dAcOShutterWaterD101_c, fProfile::OBJ_SHUTTER_WATER_D101, 0X17F, 0, 7);

STATE_DEFINE(dAcOShutterWaterD101_c, Wait);
STATE_DEFINE(dAcOShutterWaterD101_c, End);

void dAcOShutterWaterD101_c::initializeState_Wait() {}
void dAcOShutterWaterD101_c::executeState_Wait() {}
void dAcOShutterWaterD101_c::finalizeState_Wait() {}
void dAcOShutterWaterD101_c::initializeState_End() {}
void dAcOShutterWaterD101_c::executeState_End() {}
void dAcOShutterWaterD101_c::finalizeState_End() {}
