#include "d/a/obj/d_a_obj_shutter.h"

SPECIAL_ACTOR_PROFILE(SHUTTER, dAcOShutter_c, fProfile::SHUTTER, 0x183, 0, 6);

STATE_DEFINE(dAcOShutter_c, Wait);
STATE_DEFINE(dAcOShutter_c, Open);
STATE_DEFINE(dAcOShutter_c, Close);

void dAcOShutter_c::initializeState_Wait() {}
void dAcOShutter_c::executeState_Wait() {}
void dAcOShutter_c::finalizeState_Wait() {}
void dAcOShutter_c::initializeState_Open() {}
void dAcOShutter_c::executeState_Open() {}
void dAcOShutter_c::finalizeState_Open() {}
void dAcOShutter_c::initializeState_Close() {}
void dAcOShutter_c::executeState_Close() {}
void dAcOShutter_c::finalizeState_Close() {}
