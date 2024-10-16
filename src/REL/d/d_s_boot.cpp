#include "d/d_s_boot.h"

SPECIAL_ACTOR_PROFILE(BOOT, dScBoot_c, fProfile::BOOT, 0x2A7, 0, 1174405120);

STATE_DEFINE(dScBoot_c, Init);
STATE_DEFINE(dScBoot_c, Strap);
STATE_DEFINE(dScBoot_c, Connect);
STATE_DEFINE(dScBoot_c, Save);

dScBoot_c::dScBoot_c() : mStateMgr(*this, sStateID::null) {}
dScBoot_c::~dScBoot_c() {}

void dScBoot_c::initializeState_Init() {}
void dScBoot_c::executeState_Init() {}
void dScBoot_c::finalizeState_Init() {}
void dScBoot_c::initializeState_Strap() {}
void dScBoot_c::executeState_Strap() {}
void dScBoot_c::finalizeState_Strap() {}
void dScBoot_c::initializeState_Connect() {}
void dScBoot_c::executeState_Connect() {}
void dScBoot_c::finalizeState_Connect() {}
void dScBoot_c::initializeState_Save() {}
void dScBoot_c::executeState_Save() {}
void dScBoot_c::finalizeState_Save() {}
