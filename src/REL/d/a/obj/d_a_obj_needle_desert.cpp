#include <d/a/obj/d_a_obj_needle_desert.h>

SPECIAL_ACTOR_PROFILE(OBJ_NEEDLE_DESERT, dAcONeedleDesert_c, fProfile::OBJ_NEEDLE_DESERT, 0x1AA, 0, 3);

STATE_DEFINE(dAcONeedleDesert_c, On);
STATE_DEFINE(dAcONeedleDesert_c, Off);

void dAcONeedleDesert_c::initializeState_On() {}
void dAcONeedleDesert_c::executeState_On() {}
void dAcONeedleDesert_c::finalizeState_On() {}
void dAcONeedleDesert_c::initializeState_Off() {}
void dAcONeedleDesert_c::executeState_Off() {}
void dAcONeedleDesert_c::finalizeState_Off() {}
