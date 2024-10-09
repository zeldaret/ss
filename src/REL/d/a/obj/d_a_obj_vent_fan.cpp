#include <d/a/obj/d_a_obj_vent_fan.h>

SPECIAL_ACTOR_PROFILE(OBJ_VENT_FAN, dAcOventFan_c, fProfile::OBJ_VENT_FAN, 0X1B6, 0, 2);

STATE_DEFINE(dAcOventFan_c, Normal);
STATE_DEFINE(dAcOventFan_c, Reverse);
STATE_DEFINE(dAcOventFan_c, Vacuum);

void dAcOventFan_c::initializeState_Normal() {}
void dAcOventFan_c::executeState_Normal() {}
void dAcOventFan_c::finalizeState_Normal() {}
void dAcOventFan_c::initializeState_Reverse() {}
void dAcOventFan_c::executeState_Reverse() {}
void dAcOventFan_c::finalizeState_Reverse() {}
void dAcOventFan_c::initializeState_Vacuum() {}
void dAcOventFan_c::executeState_Vacuum() {}
void dAcOventFan_c::finalizeState_Vacuum() {}
