#include <d/a/obj/d_a_obj_syako_shutter.h>

SPECIAL_ACTOR_PROFILE(OBJ_SYAKO_SHUTTER, dAcOSyakoShutter_c, fProfile::OBJ_SYAKO_SHUTTER, 0X178, 0, 3);

STATE_DEFINE(dAcOSyakoShutter_c, Wait);
STATE_DEFINE(dAcOSyakoShutter_c, OpenEvent);
STATE_DEFINE(dAcOSyakoShutter_c, End);

void dAcOSyakoShutter_c::initializeState_Wait() {}
void dAcOSyakoShutter_c::executeState_Wait() {}
void dAcOSyakoShutter_c::finalizeState_Wait() {}
void dAcOSyakoShutter_c::initializeState_OpenEvent() {}
void dAcOSyakoShutter_c::executeState_OpenEvent() {}
void dAcOSyakoShutter_c::finalizeState_OpenEvent() {}
void dAcOSyakoShutter_c::initializeState_End() {}
void dAcOSyakoShutter_c::executeState_End() {}
void dAcOSyakoShutter_c::finalizeState_End() {}
