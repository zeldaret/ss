#include <d/a/d_a_pumpkin.h>

SPECIAL_ACTOR_PROFILE(PUMPKIN, dAcPumpkin_c, fProfile::PUMPKIN, 0x134, 0, 3);

STATE_DEFINE(dAcPumpkin_c, Stick);
STATE_DEFINE(dAcPumpkin_c, Wait);
STATE_DEFINE(dAcPumpkin_c, CarryGrab);
STATE_DEFINE(dAcPumpkin_c, StickSword);
STATE_DEFINE(dAcPumpkin_c, Throw);
STATE_DEFINE(dAcPumpkin_c, Roll);
STATE_DEFINE(dAcPumpkin_c, CarryBeetle);
STATE_DEFINE(dAcPumpkin_c, CarryWhip);

void dAcPumpkin_c::initializeState_Stick() {}
void dAcPumpkin_c::executeState_Stick() {}
void dAcPumpkin_c::finalizeState_Stick() {}
void dAcPumpkin_c::initializeState_Wait() {}
void dAcPumpkin_c::executeState_Wait() {}
void dAcPumpkin_c::finalizeState_Wait() {}
void dAcPumpkin_c::initializeState_CarryGrab() {}
void dAcPumpkin_c::executeState_CarryGrab() {}
void dAcPumpkin_c::finalizeState_CarryGrab() {}
void dAcPumpkin_c::initializeState_StickSword() {}
void dAcPumpkin_c::executeState_StickSword() {}
void dAcPumpkin_c::finalizeState_StickSword() {}
void dAcPumpkin_c::initializeState_Throw() {}
void dAcPumpkin_c::executeState_Throw() {}
void dAcPumpkin_c::finalizeState_Throw() {}
void dAcPumpkin_c::initializeState_Roll() {}
void dAcPumpkin_c::executeState_Roll() {}
void dAcPumpkin_c::finalizeState_Roll() {}
void dAcPumpkin_c::initializeState_CarryBeetle() {}
void dAcPumpkin_c::executeState_CarryBeetle() {}
void dAcPumpkin_c::finalizeState_CarryBeetle() {}
void dAcPumpkin_c::initializeState_CarryWhip() {}
void dAcPumpkin_c::executeState_CarryWhip() {}
void dAcPumpkin_c::finalizeState_CarryWhip() {}
