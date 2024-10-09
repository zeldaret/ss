#include <d/a/obj/d_a_obj_lamp.h>

SPECIAL_ACTOR_PROFILE(OBJ_LAMP, dAcOLamp_c, fProfile::OBJ_LAMP, 0x14B, 0, 3);

STATE_DEFINE(dAcOLamp_c, Wait);

void dAcOLamp_c::initializeState_Wait() {}
void dAcOLamp_c::executeState_Wait() {}
void dAcOLamp_c::finalizeState_Wait() {}
