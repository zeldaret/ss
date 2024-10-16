#include "d/a/obj/d_a_obj_stopping_rope.h"

SPECIAL_ACTOR_PROFILE(OBJ_STOPPING_ROPE, dAcOStoppingRope_c, fProfile::OBJ_STOPPING_ROPE, 0x23B, 0, 3);

STATE_DEFINE(dAcOStoppingRope_c, Wait);
STATE_DEFINE(dAcOStoppingRope_c, SwitchOn);

void dAcOStoppingRope_c::initializeState_Wait() {}
void dAcOStoppingRope_c::executeState_Wait() {}
void dAcOStoppingRope_c::finalizeState_Wait() {}
void dAcOStoppingRope_c::initializeState_SwitchOn() {}
void dAcOStoppingRope_c::executeState_SwitchOn() {}
void dAcOStoppingRope_c::finalizeState_SwitchOn() {}
