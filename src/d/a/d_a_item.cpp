#include "d/a/d_a_item.h"

SPECIAL_ACTOR_PROFILE(ITEM, dAcItem_c, fProfile::ITEM, 0x2B, 0, 2);

STATE_DEFINE(dAcItem_c, Wait);
STATE_DEFINE(dAcItem_c, Carry);
STATE_DEFINE(dAcItem_c, GetBeetle);
STATE_DEFINE(dAcItem_c, WaitGet);
STATE_DEFINE(dAcItem_c, Get);
STATE_DEFINE(dAcItem_c, WaitGetDemo);
STATE_DEFINE(dAcItem_c, WaitForcedGetDemo);
STATE_DEFINE(dAcItem_c, GetDemo);
STATE_DEFINE(dAcItem_c, WaitTBoxGetDemo);
STATE_DEFINE(dAcItem_c, ResurgeWait);
STATE_DEFINE(dAcItem_c, WaitTurnOff);
STATE_DEFINE(dAcItem_c, WaitSacredDewGetEffect);

dAcItem_c::dAcItem_c() : mStateMgr(*this, sStateID::null) {}
dAcItem_c::~dAcItem_c() {}

void dAcItem_c::initializeState_Wait() {}
void dAcItem_c::executeState_Wait() {}
void dAcItem_c::finalizeState_Wait() {}
void dAcItem_c::initializeState_Carry() {}
void dAcItem_c::executeState_Carry() {}
void dAcItem_c::finalizeState_Carry() {}
void dAcItem_c::initializeState_GetBeetle() {}
void dAcItem_c::executeState_GetBeetle() {}
void dAcItem_c::finalizeState_GetBeetle() {}
void dAcItem_c::initializeState_WaitGet() {}
void dAcItem_c::executeState_WaitGet() {}
void dAcItem_c::finalizeState_WaitGet() {}
void dAcItem_c::initializeState_Get() {}
void dAcItem_c::executeState_Get() {}
void dAcItem_c::finalizeState_Get() {}
void dAcItem_c::initializeState_WaitGetDemo() {}
void dAcItem_c::executeState_WaitGetDemo() {}
void dAcItem_c::finalizeState_WaitGetDemo() {}
void dAcItem_c::initializeState_WaitForcedGetDemo() {}
void dAcItem_c::executeState_WaitForcedGetDemo() {}
void dAcItem_c::finalizeState_WaitForcedGetDemo() {}
void dAcItem_c::initializeState_GetDemo() {}
void dAcItem_c::executeState_GetDemo() {}
void dAcItem_c::finalizeState_GetDemo() {}
void dAcItem_c::initializeState_WaitTBoxGetDemo() {}
void dAcItem_c::executeState_WaitTBoxGetDemo() {}
void dAcItem_c::finalizeState_WaitTBoxGetDemo() {}
void dAcItem_c::initializeState_ResurgeWait() {}
void dAcItem_c::executeState_ResurgeWait() {}
void dAcItem_c::finalizeState_ResurgeWait() {}
void dAcItem_c::initializeState_WaitTurnOff() {}
void dAcItem_c::executeState_WaitTurnOff() {}
void dAcItem_c::finalizeState_WaitTurnOff() {}
void dAcItem_c::initializeState_WaitSacredDewGetEffect() {}
void dAcItem_c::executeState_WaitSacredDewGetEffect() {}
void dAcItem_c::finalizeState_WaitSacredDewGetEffect() {}
