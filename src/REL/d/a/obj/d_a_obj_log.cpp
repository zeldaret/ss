#include <d/a/obj/d_a_obj_log.h>

SPECIAL_ACTOR_PROFILE(OBJ_LOG, dAcOlog_c, fProfile::OBJ_LOG, 0x12E, 0, 2);

STATE_DEFINE(dAcOlog_c, Wait);
STATE_DEFINE(dAcOlog_c, Move);
STATE_DEFINE(dAcOlog_c, MoveEnd);
STATE_DEFINE(dAcOlog_c, Fall);
STATE_DEFINE(dAcOlog_c, Shake);

void dAcOlog_c::initializeState_Wait() {}
void dAcOlog_c::executeState_Wait() {}
void dAcOlog_c::finalizeState_Wait() {}
void dAcOlog_c::initializeState_Move() {}
void dAcOlog_c::executeState_Move() {}
void dAcOlog_c::finalizeState_Move() {}
void dAcOlog_c::initializeState_MoveEnd() {}
void dAcOlog_c::executeState_MoveEnd() {}
void dAcOlog_c::finalizeState_MoveEnd() {}
void dAcOlog_c::initializeState_Fall() {}
void dAcOlog_c::executeState_Fall() {}
void dAcOlog_c::finalizeState_Fall() {}
void dAcOlog_c::initializeState_Shake() {}
void dAcOlog_c::executeState_Shake() {}
void dAcOlog_c::finalizeState_Shake() {}
