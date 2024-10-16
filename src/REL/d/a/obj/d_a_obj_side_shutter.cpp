#include "d/a/obj/d_a_obj_side_shutter.h"

SPECIAL_ACTOR_PROFILE(OBJ_SIDE_SHUTTER, dAcOsideShutter_c, fProfile::OBJ_SIDE_SHUTTER, 0x1BD, 0, 6);

STATE_DEFINE(dAcOsideShutter_c, Off);
STATE_DEFINE(dAcOsideShutter_c, On);
STATE_DEFINE(dAcOsideShutter_c, OffToOnWait);
STATE_DEFINE(dAcOsideShutter_c, OffToOn);
STATE_DEFINE(dAcOsideShutter_c, OnToOffWait);
STATE_DEFINE(dAcOsideShutter_c, OnToOff);

void dAcOsideShutter_c::initializeState_Off() {}
void dAcOsideShutter_c::executeState_Off() {}
void dAcOsideShutter_c::finalizeState_Off() {}
void dAcOsideShutter_c::initializeState_On() {}
void dAcOsideShutter_c::executeState_On() {}
void dAcOsideShutter_c::finalizeState_On() {}
void dAcOsideShutter_c::initializeState_OffToOnWait() {}
void dAcOsideShutter_c::executeState_OffToOnWait() {}
void dAcOsideShutter_c::finalizeState_OffToOnWait() {}
void dAcOsideShutter_c::initializeState_OffToOn() {}
void dAcOsideShutter_c::executeState_OffToOn() {}
void dAcOsideShutter_c::finalizeState_OffToOn() {}
void dAcOsideShutter_c::initializeState_OnToOffWait() {}
void dAcOsideShutter_c::executeState_OnToOffWait() {}
void dAcOsideShutter_c::finalizeState_OnToOffWait() {}
void dAcOsideShutter_c::initializeState_OnToOff() {}
void dAcOsideShutter_c::executeState_OnToOff() {}
void dAcOsideShutter_c::finalizeState_OnToOff() {}
