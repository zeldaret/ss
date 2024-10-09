#include <d/a/obj/d_a_obj_slice_log.h>

SPECIAL_ACTOR_PROFILE(OBJ_SLICE_LOG, dAcOsliceLog_c, fProfile::OBJ_SLICE_LOG, 0X1F5, 0, 6);

STATE_DEFINE(dAcOsliceLog_c, Fall);
STATE_DEFINE(dAcOsliceLog_c, Bound);
STATE_DEFINE(dAcOsliceLog_c, Init);
STATE_DEFINE(dAcOsliceLog_c, Wait);
STATE_DEFINE(dAcOsliceLog_c, Slice);

void dAcOsliceLog_c::initializeState_Fall() {}
void dAcOsliceLog_c::executeState_Fall() {}
void dAcOsliceLog_c::finalizeState_Fall() {}
void dAcOsliceLog_c::initializeState_Bound() {}
void dAcOsliceLog_c::executeState_Bound() {}
void dAcOsliceLog_c::finalizeState_Bound() {}
void dAcOsliceLog_c::initializeState_Init() {}
void dAcOsliceLog_c::executeState_Init() {}
void dAcOsliceLog_c::finalizeState_Init() {}
void dAcOsliceLog_c::initializeState_Wait() {}
void dAcOsliceLog_c::executeState_Wait() {}
void dAcOsliceLog_c::finalizeState_Wait() {}
void dAcOsliceLog_c::initializeState_Slice() {}
void dAcOsliceLog_c::executeState_Slice() {}
void dAcOsliceLog_c::finalizeState_Slice() {}
