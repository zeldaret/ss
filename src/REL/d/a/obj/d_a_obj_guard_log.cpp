#include <d/a/obj/d_a_obj_guard_log.h>

SPECIAL_ACTOR_PROFILE(OBJ_GUARD_LOG, dAcOguardLog_c, fProfile::OBJ_GUARD_LOG, 0x1F4, 0, 2);

STATE_DEFINE(dAcOguardLog_c, Init);
STATE_DEFINE(dAcOguardLog_c, Wait);
STATE_DEFINE(dAcOguardLog_c, Shake);

void dAcOguardLog_c::initializeState_Init() {}
void dAcOguardLog_c::executeState_Init() {}
void dAcOguardLog_c::finalizeState_Init() {}
void dAcOguardLog_c::initializeState_Wait() {}
void dAcOguardLog_c::executeState_Wait() {}
void dAcOguardLog_c::finalizeState_Wait() {}
void dAcOguardLog_c::initializeState_Shake() {}
void dAcOguardLog_c::executeState_Shake() {}
void dAcOguardLog_c::finalizeState_Shake() {}
