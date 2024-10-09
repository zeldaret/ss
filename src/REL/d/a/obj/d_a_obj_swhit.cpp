#include <d/a/obj/d_a_obj_swhit.h>

SPECIAL_ACTOR_PROFILE(OBJ_SWHIT, dAcOswhit_c, fProfile::OBJ_SWHIT, 0X132, 0, 18);

STATE_DEFINE(dAcOswhit_c, OnWait);
STATE_DEFINE(dAcOswhit_c, On);
STATE_DEFINE(dAcOswhit_c, OffWait);
STATE_DEFINE(dAcOswhit_c, Off);

void dAcOswhit_c::initializeState_OnWait() {}
void dAcOswhit_c::executeState_OnWait() {}
void dAcOswhit_c::finalizeState_OnWait() {}
void dAcOswhit_c::initializeState_On() {}
void dAcOswhit_c::executeState_On() {}
void dAcOswhit_c::finalizeState_On() {}
void dAcOswhit_c::initializeState_OffWait() {}
void dAcOswhit_c::executeState_OffWait() {}
void dAcOswhit_c::finalizeState_OffWait() {}
void dAcOswhit_c::initializeState_Off() {}
void dAcOswhit_c::executeState_Off() {}
void dAcOswhit_c::finalizeState_Off() {}
