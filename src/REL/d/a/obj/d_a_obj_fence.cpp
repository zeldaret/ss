#include "d/a/obj/d_a_obj_fence.h"

SPECIAL_ACTOR_PROFILE(OBJ_FENCE, dAcOFence_c, fProfile::OBJ_FENCE, 0x18B, 0, 4);

STATE_DEFINE(dAcOFence_c, WaitOpen);
STATE_DEFINE(dAcOFence_c, Open);
STATE_DEFINE(dAcOFence_c, OpenPocoAPoco);
STATE_DEFINE(dAcOFence_c, SuccessPocoAPoco);
STATE_DEFINE(dAcOFence_c, StopOpenPocoAPoco);
STATE_DEFINE(dAcOFence_c, StopOpenAfterglowPocoAPoco);
STATE_DEFINE(dAcOFence_c, WaitClose);
STATE_DEFINE(dAcOFence_c, Close);
STATE_DEFINE(dAcOFence_c, RequestConfineEvent);

dAcOFence_c::dAcOFence_c() : mStateMgr(*this, sStateID::null) {}
dAcOFence_c::~dAcOFence_c() {}

void dAcOFence_c::initializeState_WaitOpen() {}
void dAcOFence_c::executeState_WaitOpen() {}
void dAcOFence_c::finalizeState_WaitOpen() {}
void dAcOFence_c::initializeState_Open() {}
void dAcOFence_c::executeState_Open() {}
void dAcOFence_c::finalizeState_Open() {}
void dAcOFence_c::initializeState_OpenPocoAPoco() {}
void dAcOFence_c::executeState_OpenPocoAPoco() {}
void dAcOFence_c::finalizeState_OpenPocoAPoco() {}
void dAcOFence_c::initializeState_SuccessPocoAPoco() {}
void dAcOFence_c::executeState_SuccessPocoAPoco() {}
void dAcOFence_c::finalizeState_SuccessPocoAPoco() {}
void dAcOFence_c::initializeState_StopOpenPocoAPoco() {}
void dAcOFence_c::executeState_StopOpenPocoAPoco() {}
void dAcOFence_c::finalizeState_StopOpenPocoAPoco() {}
void dAcOFence_c::initializeState_StopOpenAfterglowPocoAPoco() {}
void dAcOFence_c::executeState_StopOpenAfterglowPocoAPoco() {}
void dAcOFence_c::finalizeState_StopOpenAfterglowPocoAPoco() {}
void dAcOFence_c::initializeState_WaitClose() {}
void dAcOFence_c::executeState_WaitClose() {}
void dAcOFence_c::finalizeState_WaitClose() {}
void dAcOFence_c::initializeState_Close() {}
void dAcOFence_c::executeState_Close() {}
void dAcOFence_c::finalizeState_Close() {}
void dAcOFence_c::initializeState_RequestConfineEvent() {}
void dAcOFence_c::executeState_RequestConfineEvent() {}
void dAcOFence_c::finalizeState_RequestConfineEvent() {}
