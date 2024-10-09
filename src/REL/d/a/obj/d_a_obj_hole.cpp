#include <d/a/obj/d_a_obj_hole.h>

SPECIAL_ACTOR_PROFILE(OBJ_HOLE, dAcOhole_c, fProfile::OBJ_HOLE, 0xF7, 0, 258);

STATE_DEFINE(dAcOhole_c, Wait);

void dAcOhole_c::initializeState_Wait() {}
void dAcOhole_c::executeState_Wait() {}
void dAcOhole_c::finalizeState_Wait() {}
