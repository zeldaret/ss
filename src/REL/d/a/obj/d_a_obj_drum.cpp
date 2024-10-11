#include <d/a/obj/d_a_obj_drum.h>

SPECIAL_ACTOR_PROFILE(OBJ_DRUM, dAcOdrum_c, fProfile::OBJ_DRUM, 0x26F, 0, 2);

STATE_DEFINE(dAcOdrum_c, Wait);
STATE_DEFINE(dAcOdrum_c, Move);
STATE_DEFINE(dAcOdrum_c, MoveEnd);
STATE_DEFINE(dAcOdrum_c, Fall);
STATE_DEFINE(dAcOdrum_c, Init);
STATE_DEFINE(dAcOdrum_c, BeltMove);
STATE_DEFINE(dAcOdrum_c, TeniN);

void dAcOdrum_c::initializeState_Wait() {}
void dAcOdrum_c::executeState_Wait() {}
void dAcOdrum_c::finalizeState_Wait() {}
void dAcOdrum_c::initializeState_Move() {}
void dAcOdrum_c::executeState_Move() {}
void dAcOdrum_c::finalizeState_Move() {}
void dAcOdrum_c::initializeState_MoveEnd() {}
void dAcOdrum_c::executeState_MoveEnd() {}
void dAcOdrum_c::finalizeState_MoveEnd() {}
void dAcOdrum_c::initializeState_Fall() {}
void dAcOdrum_c::executeState_Fall() {}
void dAcOdrum_c::finalizeState_Fall() {}
void dAcOdrum_c::initializeState_Init() {}
void dAcOdrum_c::executeState_Init() {}
void dAcOdrum_c::finalizeState_Init() {}
void dAcOdrum_c::initializeState_BeltMove() {}
void dAcOdrum_c::executeState_BeltMove() {}
void dAcOdrum_c::finalizeState_BeltMove() {}
void dAcOdrum_c::initializeState_TeniN() {}
void dAcOdrum_c::executeState_TeniN() {}
void dAcOdrum_c::finalizeState_TeniN() {}
