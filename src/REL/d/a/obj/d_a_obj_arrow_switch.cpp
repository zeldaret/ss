#include <d/a/obj/d_a_obj_arrow_switch.h>

SPECIAL_ACTOR_PROFILE(OBJ_ARROW_SWITCH, dAcOarrowSwitch_c, fProfile::OBJ_ARROW_SWITCH, 0X1B5, 0, 6);

STATE_DEFINE(dAcOarrowSwitch_c, Off);
STATE_DEFINE(dAcOarrowSwitch_c, On);
STATE_DEFINE(dAcOarrowSwitch_c, FixedOn);
STATE_DEFINE(dAcOarrowSwitch_c, OpenEye);

void dAcOarrowSwitch_c::initializeState_Off() {}
void dAcOarrowSwitch_c::executeState_Off() {}
void dAcOarrowSwitch_c::finalizeState_Off() {}
void dAcOarrowSwitch_c::initializeState_On() {}
void dAcOarrowSwitch_c::executeState_On() {}
void dAcOarrowSwitch_c::finalizeState_On() {}
void dAcOarrowSwitch_c::initializeState_FixedOn() {}
void dAcOarrowSwitch_c::executeState_FixedOn() {}
void dAcOarrowSwitch_c::finalizeState_FixedOn() {}
void dAcOarrowSwitch_c::initializeState_OpenEye() {}
void dAcOarrowSwitch_c::executeState_OpenEye() {}
void dAcOarrowSwitch_c::finalizeState_OpenEye() {}
