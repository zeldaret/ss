#include "d/a/obj/d_a_obj_sail.h"

SPECIAL_ACTOR_PROFILE(OBJ_SAIL, dAcOSail_c, fProfile::OBJ_SAIL, 0x171, 0, 7);

STATE_DEFINE(dAcOSail_c, Wait);

void dAcOSail_c::initializeState_Wait() {}
void dAcOSail_c::executeState_Wait() {}
void dAcOSail_c::finalizeState_Wait() {}
