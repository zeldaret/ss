#include "d/a/obj/d_a_obj_door_boss.h"

SPECIAL_ACTOR_PROFILE(OBJ_DOOR_BOSS, dAcOdoorBoss_c, fProfile::OBJ_DOOR_BOSS, 0x189, 0, 2);

STATE_DEFINE(dAcOdoorBoss_c, UnlockWait);
STATE_DEFINE(dAcOdoorBoss_c, CorrectKey);
STATE_DEFINE(dAcOdoorBoss_c, KeyInsertInitialInterval);
STATE_DEFINE(dAcOdoorBoss_c, PutKeyIntoLock);
STATE_DEFINE(dAcOdoorBoss_c, KeyInsert);
STATE_DEFINE(dAcOdoorBoss_c, KeyInsertAfterglow);
STATE_DEFINE(dAcOdoorBoss_c, Wait);
STATE_DEFINE(dAcOdoorBoss_c, Shine);
STATE_DEFINE(dAcOdoorBoss_c, Unlock);
STATE_DEFINE(dAcOdoorBoss_c, OpenWait);
STATE_DEFINE(dAcOdoorBoss_c, Open);
STATE_DEFINE(dAcOdoorBoss_c, WaitClose);
STATE_DEFINE(dAcOdoorBoss_c, Close);
STATE_DEFINE(dAcOdoorBoss_c, Idle);
STATE_DEFINE(dAcOdoorBoss_c, BeforeTimeShift);

void dAcOdoorBoss_c::initializeState_UnlockWait() {}
void dAcOdoorBoss_c::executeState_UnlockWait() {}
void dAcOdoorBoss_c::finalizeState_UnlockWait() {}
void dAcOdoorBoss_c::initializeState_CorrectKey() {}
void dAcOdoorBoss_c::executeState_CorrectKey() {}
void dAcOdoorBoss_c::finalizeState_CorrectKey() {}
void dAcOdoorBoss_c::initializeState_KeyInsertInitialInterval() {}
void dAcOdoorBoss_c::executeState_KeyInsertInitialInterval() {}
void dAcOdoorBoss_c::finalizeState_KeyInsertInitialInterval() {}
void dAcOdoorBoss_c::initializeState_PutKeyIntoLock() {}
void dAcOdoorBoss_c::executeState_PutKeyIntoLock() {}
void dAcOdoorBoss_c::finalizeState_PutKeyIntoLock() {}
void dAcOdoorBoss_c::initializeState_KeyInsert() {}
void dAcOdoorBoss_c::executeState_KeyInsert() {}
void dAcOdoorBoss_c::finalizeState_KeyInsert() {}
void dAcOdoorBoss_c::initializeState_KeyInsertAfterglow() {}
void dAcOdoorBoss_c::executeState_KeyInsertAfterglow() {}
void dAcOdoorBoss_c::finalizeState_KeyInsertAfterglow() {}
void dAcOdoorBoss_c::initializeState_Wait() {}
void dAcOdoorBoss_c::executeState_Wait() {}
void dAcOdoorBoss_c::finalizeState_Wait() {}
void dAcOdoorBoss_c::initializeState_Shine() {}
void dAcOdoorBoss_c::executeState_Shine() {}
void dAcOdoorBoss_c::finalizeState_Shine() {}
void dAcOdoorBoss_c::initializeState_Unlock() {}
void dAcOdoorBoss_c::executeState_Unlock() {}
void dAcOdoorBoss_c::finalizeState_Unlock() {}
void dAcOdoorBoss_c::initializeState_OpenWait() {}
void dAcOdoorBoss_c::executeState_OpenWait() {}
void dAcOdoorBoss_c::finalizeState_OpenWait() {}
void dAcOdoorBoss_c::initializeState_Open() {}
void dAcOdoorBoss_c::executeState_Open() {}
void dAcOdoorBoss_c::finalizeState_Open() {}
void dAcOdoorBoss_c::initializeState_WaitClose() {}
void dAcOdoorBoss_c::executeState_WaitClose() {}
void dAcOdoorBoss_c::finalizeState_WaitClose() {}
void dAcOdoorBoss_c::initializeState_Close() {}
void dAcOdoorBoss_c::executeState_Close() {}
void dAcOdoorBoss_c::finalizeState_Close() {}
void dAcOdoorBoss_c::initializeState_Idle() {}
void dAcOdoorBoss_c::executeState_Idle() {}
void dAcOdoorBoss_c::finalizeState_Idle() {}
void dAcOdoorBoss_c::initializeState_BeforeTimeShift() {}
void dAcOdoorBoss_c::executeState_BeforeTimeShift() {}
void dAcOdoorBoss_c::finalizeState_BeforeTimeShift() {}
