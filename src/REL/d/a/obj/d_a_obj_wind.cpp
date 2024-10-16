#include "d/a/obj/d_a_obj_wind.h"

SPECIAL_ACTOR_PROFILE(OBJ_WIND, dAcOwind_c, fProfile::OBJ_WIND, 0xDB, 0, 261);

STATE_DEFINE(dAcOwind_c, Wait);

void dAcOwind_c::initializeState_Wait() {}
void dAcOwind_c::executeState_Wait() {}
void dAcOwind_c::finalizeState_Wait() {}
