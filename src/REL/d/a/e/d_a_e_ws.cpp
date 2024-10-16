#include "d/a/e/d_a_e_ws.h"

SPECIAL_ACTOR_PROFILE(E_WS, dAcEws_c, fProfile::E_WS, 0x10C, 0, 3);

STATE_DEFINE(dAcEws_c, Wait);
STATE_DEFINE(dAcEws_c, Move);
STATE_DEFINE(dAcEws_c, PyMove);
STATE_DEFINE(dAcEws_c, JumpReady);
STATE_DEFINE(dAcEws_c, JumpReady2);
STATE_DEFINE(dAcEws_c, Jump);
STATE_DEFINE(dAcEws_c, Hug);
STATE_DEFINE(dAcEws_c, ReturnJump);
STATE_DEFINE(dAcEws_c, Return);
STATE_DEFINE(dAcEws_c, Escape);
STATE_DEFINE(dAcEws_c, Dead);

void dAcEws_c::initializeState_Wait() {}
void dAcEws_c::executeState_Wait() {}
void dAcEws_c::finalizeState_Wait() {}
void dAcEws_c::initializeState_Move() {}
void dAcEws_c::executeState_Move() {}
void dAcEws_c::finalizeState_Move() {}
void dAcEws_c::initializeState_PyMove() {}
void dAcEws_c::executeState_PyMove() {}
void dAcEws_c::finalizeState_PyMove() {}
void dAcEws_c::initializeState_JumpReady() {}
void dAcEws_c::executeState_JumpReady() {}
void dAcEws_c::finalizeState_JumpReady() {}
void dAcEws_c::initializeState_JumpReady2() {}
void dAcEws_c::executeState_JumpReady2() {}
void dAcEws_c::finalizeState_JumpReady2() {}
void dAcEws_c::initializeState_Jump() {}
void dAcEws_c::executeState_Jump() {}
void dAcEws_c::finalizeState_Jump() {}
void dAcEws_c::initializeState_Hug() {}
void dAcEws_c::executeState_Hug() {}
void dAcEws_c::finalizeState_Hug() {}
void dAcEws_c::initializeState_ReturnJump() {}
void dAcEws_c::executeState_ReturnJump() {}
void dAcEws_c::finalizeState_ReturnJump() {}
void dAcEws_c::initializeState_Return() {}
void dAcEws_c::executeState_Return() {}
void dAcEws_c::finalizeState_Return() {}
void dAcEws_c::initializeState_Escape() {}
void dAcEws_c::executeState_Escape() {}
void dAcEws_c::finalizeState_Escape() {}
void dAcEws_c::initializeState_Dead() {}
void dAcEws_c::executeState_Dead() {}
void dAcEws_c::finalizeState_Dead() {}
