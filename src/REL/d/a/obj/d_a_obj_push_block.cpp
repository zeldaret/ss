#include <d/a/obj/d_a_obj_push_block.h>

SPECIAL_ACTOR_PROFILE(OBJ_PUSH_BLOCK, dAcOpushBlk_c, fProfile::OBJ_PUSH_BLOCK, 0X12D, 0, 2);

STATE_DEFINE(dAcOpushBlk_c, Wait);
STATE_DEFINE(dAcOpushBlk_c, Move);
STATE_DEFINE(dAcOpushBlk_c, RopeGrab);

void dAcOpushBlk_c::initializeState_Wait() {}
void dAcOpushBlk_c::executeState_Wait() {}
void dAcOpushBlk_c::finalizeState_Wait() {}
void dAcOpushBlk_c::initializeState_Move() {}
void dAcOpushBlk_c::executeState_Move() {}
void dAcOpushBlk_c::finalizeState_Move() {}
void dAcOpushBlk_c::initializeState_RopeGrab() {}
void dAcOpushBlk_c::executeState_RopeGrab() {}
void dAcOpushBlk_c::finalizeState_RopeGrab() {}
