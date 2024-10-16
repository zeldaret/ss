#include "d/a/obj/d_a_obj_fire.h"

SPECIAL_ACTOR_PROFILE(OBJ_FIRE, dAcOfire_c, fProfile::OBJ_FIRE, 0x26D, 0, 6);

STATE_DEFINE(dAcOfire_c, Wait);

void dAcOfire_c::initializeState_Wait() {}
void dAcOfire_c::executeState_Wait() {}
void dAcOfire_c::finalizeState_Wait() {}
