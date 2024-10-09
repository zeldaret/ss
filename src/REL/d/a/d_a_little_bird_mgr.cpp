#include <d/a/d_a_little_bird_mgr.h>

SPECIAL_ACTOR_PROFILE(LITTLE_BIRD_MGR, dAcLittleBirdMgr_c, fProfile::LITTLE_BIRD_MGR, 0x98, 0, 3);

STATE_DEFINE(dAcLittleBirdMgr_c, Set);
STATE_DEFINE(dAcLittleBirdMgr_c, Reset);
STATE_DEFINE(dAcLittleBirdMgr_c, Safety);
STATE_DEFINE(dAcLittleBirdMgr_c, Warning);
STATE_DEFINE(dAcLittleBirdMgr_c, Danger);
STATE_DEFINE(dAcLittleBirdMgr_c, Kill);

void dAcLittleBirdMgr_c::initializeState_Set() {}
void dAcLittleBirdMgr_c::executeState_Set() {}
void dAcLittleBirdMgr_c::finalizeState_Set() {}
void dAcLittleBirdMgr_c::initializeState_Reset() {}
void dAcLittleBirdMgr_c::executeState_Reset() {}
void dAcLittleBirdMgr_c::finalizeState_Reset() {}
void dAcLittleBirdMgr_c::initializeState_Safety() {}
void dAcLittleBirdMgr_c::executeState_Safety() {}
void dAcLittleBirdMgr_c::finalizeState_Safety() {}
void dAcLittleBirdMgr_c::initializeState_Warning() {}
void dAcLittleBirdMgr_c::executeState_Warning() {}
void dAcLittleBirdMgr_c::finalizeState_Warning() {}
void dAcLittleBirdMgr_c::initializeState_Danger() {}
void dAcLittleBirdMgr_c::executeState_Danger() {}
void dAcLittleBirdMgr_c::finalizeState_Danger() {}
void dAcLittleBirdMgr_c::initializeState_Kill() {}
void dAcLittleBirdMgr_c::executeState_Kill() {}
void dAcLittleBirdMgr_c::finalizeState_Kill() {}
