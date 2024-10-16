#include "d/a/obj/d_a_obj_underground_switch.h"

SPECIAL_ACTOR_PROFILE(OBJ_UG_SWITCH, dAcOUgSwitch_c, fProfile::OBJ_UG_SWITCH, 0x15A, 0, 3);

STATE_DEFINE(dAcOUgSwitch_c, Off);
STATE_DEFINE(dAcOUgSwitch_c, Active);
STATE_DEFINE(dAcOUgSwitch_c, On);

void dAcOUgSwitch_c::initializeState_Off() {}
void dAcOUgSwitch_c::executeState_Off() {}
void dAcOUgSwitch_c::finalizeState_Off() {}
void dAcOUgSwitch_c::initializeState_Active() {}
void dAcOUgSwitch_c::executeState_Active() {}
void dAcOUgSwitch_c::finalizeState_Active() {}
void dAcOUgSwitch_c::initializeState_On() {}
void dAcOUgSwitch_c::executeState_On() {}
void dAcOUgSwitch_c::finalizeState_On() {}
