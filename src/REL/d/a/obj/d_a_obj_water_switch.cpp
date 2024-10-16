#include "d/a/obj/d_a_obj_water_switch.h"

SPECIAL_ACTOR_PROFILE(OBJ_WATER_SWITCH, dAcOwaterSwitch_c, fProfile::OBJ_WATER_SWITCH, 0x1B9, 0, 6);

STATE_DEFINE(dAcOwaterSwitch_c, Off);
STATE_DEFINE(dAcOwaterSwitch_c, Wait1);
STATE_DEFINE(dAcOwaterSwitch_c, Wait2);
STATE_DEFINE(dAcOwaterSwitch_c, Wait3);
STATE_DEFINE(dAcOwaterSwitch_c, On);

void dAcOwaterSwitch_c::initializeState_Off() {}
void dAcOwaterSwitch_c::executeState_Off() {}
void dAcOwaterSwitch_c::finalizeState_Off() {}
void dAcOwaterSwitch_c::initializeState_Wait1() {}
void dAcOwaterSwitch_c::executeState_Wait1() {}
void dAcOwaterSwitch_c::finalizeState_Wait1() {}
void dAcOwaterSwitch_c::initializeState_Wait2() {}
void dAcOwaterSwitch_c::executeState_Wait2() {}
void dAcOwaterSwitch_c::finalizeState_Wait2() {}
void dAcOwaterSwitch_c::initializeState_Wait3() {}
void dAcOwaterSwitch_c::executeState_Wait3() {}
void dAcOwaterSwitch_c::finalizeState_Wait3() {}
void dAcOwaterSwitch_c::initializeState_On() {}
void dAcOwaterSwitch_c::executeState_On() {}
void dAcOwaterSwitch_c::finalizeState_On() {}
