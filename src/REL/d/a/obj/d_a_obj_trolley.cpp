#include "d/a/obj/d_a_obj_trolley.h"

SPECIAL_ACTOR_PROFILE(OBJ_TROLLEY, dAcOTrolley_c, fProfile::OBJ_TROLLEY, 0x14A, 0, 4099);

STATE_DEFINE(dAcOTrolley_c, Wait);
STATE_DEFINE(dAcOTrolley_c, RideAction);
STATE_DEFINE(dAcOTrolley_c, StartAction);
STATE_DEFINE(dAcOTrolley_c, PathMove);
STATE_DEFINE(dAcOTrolley_c, StopAction);
STATE_DEFINE(dAcOTrolley_c, CrashAction);
STATE_DEFINE(dAcOTrolley_c, PushPull);
STATE_DEFINE(dAcOTrolley_c, Fall);
STATE_DEFINE(dAcOTrolley_c, End);
STATE_DEFINE(dAcOTrolley_c, TimeSlipStart);
STATE_DEFINE(dAcOTrolley_c, TimeSlipEnd);
STATE_DEFINE(dAcOTrolley_c, WaitOpenDoor);
STATE_DEFINE(dAcOTrolley_c, RestartWait);
STATE_DEFINE(dAcOTrolley_c, BuriedSand);
STATE_DEFINE(dAcOTrolley_c, BackMove);
STATE_DEFINE(dAcOTrolley_c, F302RevivalDemo);
STATE_DEFINE(dAcOTrolley_c, TimeSlipEndFromAnother);
STATE_DEFINE(dAcOTrolley_c, WaitOnSwitch);

void dAcOTrolley_c::initializeState_Wait() {}
void dAcOTrolley_c::executeState_Wait() {}
void dAcOTrolley_c::finalizeState_Wait() {}
void dAcOTrolley_c::initializeState_RideAction() {}
void dAcOTrolley_c::executeState_RideAction() {}
void dAcOTrolley_c::finalizeState_RideAction() {}
void dAcOTrolley_c::initializeState_StartAction() {}
void dAcOTrolley_c::executeState_StartAction() {}
void dAcOTrolley_c::finalizeState_StartAction() {}
void dAcOTrolley_c::initializeState_PathMove() {}
void dAcOTrolley_c::executeState_PathMove() {}
void dAcOTrolley_c::finalizeState_PathMove() {}
void dAcOTrolley_c::initializeState_StopAction() {}
void dAcOTrolley_c::executeState_StopAction() {}
void dAcOTrolley_c::finalizeState_StopAction() {}
void dAcOTrolley_c::initializeState_CrashAction() {}
void dAcOTrolley_c::executeState_CrashAction() {}
void dAcOTrolley_c::finalizeState_CrashAction() {}
void dAcOTrolley_c::initializeState_PushPull() {}
void dAcOTrolley_c::executeState_PushPull() {}
void dAcOTrolley_c::finalizeState_PushPull() {}
void dAcOTrolley_c::initializeState_Fall() {}
void dAcOTrolley_c::executeState_Fall() {}
void dAcOTrolley_c::finalizeState_Fall() {}
void dAcOTrolley_c::initializeState_End() {}
void dAcOTrolley_c::executeState_End() {}
void dAcOTrolley_c::finalizeState_End() {}
void dAcOTrolley_c::initializeState_TimeSlipStart() {}
void dAcOTrolley_c::executeState_TimeSlipStart() {}
void dAcOTrolley_c::finalizeState_TimeSlipStart() {}
void dAcOTrolley_c::initializeState_TimeSlipEnd() {}
void dAcOTrolley_c::executeState_TimeSlipEnd() {}
void dAcOTrolley_c::finalizeState_TimeSlipEnd() {}
void dAcOTrolley_c::initializeState_WaitOpenDoor() {}
void dAcOTrolley_c::executeState_WaitOpenDoor() {}
void dAcOTrolley_c::finalizeState_WaitOpenDoor() {}
void dAcOTrolley_c::initializeState_RestartWait() {}
void dAcOTrolley_c::executeState_RestartWait() {}
void dAcOTrolley_c::finalizeState_RestartWait() {}
void dAcOTrolley_c::initializeState_BuriedSand() {}
void dAcOTrolley_c::executeState_BuriedSand() {}
void dAcOTrolley_c::finalizeState_BuriedSand() {}
void dAcOTrolley_c::initializeState_BackMove() {}
void dAcOTrolley_c::executeState_BackMove() {}
void dAcOTrolley_c::finalizeState_BackMove() {}
void dAcOTrolley_c::initializeState_F302RevivalDemo() {}
void dAcOTrolley_c::executeState_F302RevivalDemo() {}
void dAcOTrolley_c::finalizeState_F302RevivalDemo() {}
void dAcOTrolley_c::initializeState_TimeSlipEndFromAnother() {}
void dAcOTrolley_c::executeState_TimeSlipEndFromAnother() {}
void dAcOTrolley_c::finalizeState_TimeSlipEndFromAnother() {}
void dAcOTrolley_c::initializeState_WaitOnSwitch() {}
void dAcOTrolley_c::executeState_WaitOnSwitch() {}
void dAcOTrolley_c::finalizeState_WaitOnSwitch() {}
