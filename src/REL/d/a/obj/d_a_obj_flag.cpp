#include "d/a/obj/d_a_obj_flag.h"

SPECIAL_ACTOR_PROFILE(OBJ_FLAG, dAcOFlag_c, fProfile::OBJ_FLAG, 0x14E, 0, 3);

STATE_DEFINE(dAcOFlag_c, Wait);

void dAcOFlag_c::initializeState_Wait() {}
void dAcOFlag_c::executeState_Wait() {}
void dAcOFlag_c::finalizeState_Wait() {}
