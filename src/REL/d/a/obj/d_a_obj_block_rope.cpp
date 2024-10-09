#include <d/a/obj/d_a_obj_block_rope.h>

SPECIAL_ACTOR_PROFILE(OBJ_BLOCK_ROPE, dAcOblockRope_c, fProfile::OBJ_BLOCK_ROPE, 0x264, 0, 2);

STATE_DEFINE(dAcOblockRope_c, Wait);
STATE_DEFINE(dAcOblockRope_c, Delete);

void dAcOblockRope_c::initializeState_Wait() {}
void dAcOblockRope_c::executeState_Wait() {}
void dAcOblockRope_c::finalizeState_Wait() {}
void dAcOblockRope_c::initializeState_Delete() {}
void dAcOblockRope_c::executeState_Delete() {}
void dAcOblockRope_c::finalizeState_Delete() {}
