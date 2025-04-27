#include "d/lyt/d_lyt_pause_disp_01.h"

STATE_DEFINE(dLytPauseDisp01_c, None);
STATE_DEFINE(dLytPauseDisp01_c, In);
STATE_DEFINE(dLytPauseDisp01_c, Wait);
STATE_DEFINE(dLytPauseDisp01_c, Select);
STATE_DEFINE(dLytPauseDisp01_c, GetDemo);
STATE_DEFINE(dLytPauseDisp01_c, Out);

dLytPauseDisp01_c::dLytPauseDisp01_c() : mStateMgr(*this, sStateID::null) {}

void dLytPauseDisp01_c::initializeState_None() {}
void dLytPauseDisp01_c::executeState_None() {}
void dLytPauseDisp01_c::finalizeState_None() {}

void dLytPauseDisp01_c::initializeState_In() {}
void dLytPauseDisp01_c::executeState_In() {}
void dLytPauseDisp01_c::finalizeState_In() {}

void dLytPauseDisp01_c::initializeState_Wait() {}
void dLytPauseDisp01_c::executeState_Wait() {}
void dLytPauseDisp01_c::finalizeState_Wait() {}

void dLytPauseDisp01_c::initializeState_Select() {}
void dLytPauseDisp01_c::executeState_Select() {}
void dLytPauseDisp01_c::finalizeState_Select() {}

void dLytPauseDisp01_c::initializeState_GetDemo() {}
void dLytPauseDisp01_c::executeState_GetDemo() {}
void dLytPauseDisp01_c::finalizeState_GetDemo() {}

void dLytPauseDisp01_c::initializeState_Out() {}
void dLytPauseDisp01_c::executeState_Out() {}
void dLytPauseDisp01_c::finalizeState_Out() {}
