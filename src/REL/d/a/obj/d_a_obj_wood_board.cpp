#include <d/a/obj/d_a_obj_wood_board.h>

SPECIAL_ACTOR_PROFILE(OBJ_WOOD_BOARD, dAcOwoodBoard_c, fProfile::OBJ_WOOD_BOARD, 0X1B7, 0, 2);

STATE_DEFINE(dAcOwoodBoard_c, Wait);
STATE_DEFINE(dAcOwoodBoard_c, Break);

void dAcOwoodBoard_c::initializeState_Wait() {}
void dAcOwoodBoard_c::executeState_Wait() {}
void dAcOwoodBoard_c::finalizeState_Wait() {}
void dAcOwoodBoard_c::initializeState_Break() {}
void dAcOwoodBoard_c::executeState_Break() {}
void dAcOwoodBoard_c::finalizeState_Break() {}
