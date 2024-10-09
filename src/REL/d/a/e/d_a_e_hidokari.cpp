#include <d/a/e/d_a_e_hidokari.h>

SPECIAL_ACTOR_PROFILE(E_HIDOKARI, dAcEhidokari_c, fProfile::E_HIDOKARI, 0XE5, 0, 2);

STATE_DEFINE(dAcEhidokari_c, Wait);
STATE_DEFINE(dAcEhidokari_c, Walk);
STATE_DEFINE(dAcEhidokari_c, Kyoro);
STATE_DEFINE(dAcEhidokari_c, Find);
STATE_DEFINE(dAcEhidokari_c, Run);
STATE_DEFINE(dAcEhidokari_c, Inside);
STATE_DEFINE(dAcEhidokari_c, Stun);
STATE_DEFINE(dAcEhidokari_c, WaitShell);
STATE_DEFINE(dAcEhidokari_c, RunShell);
STATE_DEFINE(dAcEhidokari_c, WalkShell);
STATE_DEFINE(dAcEhidokari_c, FireShell);
STATE_DEFINE(dAcEhidokari_c, RetShell);
STATE_DEFINE(dAcEhidokari_c, NoShell);
STATE_DEFINE(dAcEhidokari_c, Damage);
STATE_DEFINE(dAcEhidokari_c, GuradShell);
STATE_DEFINE(dAcEhidokari_c, Panic);
STATE_DEFINE(dAcEhidokari_c, Dead);

void dAcEhidokari_c::initializeState_Wait() {}
void dAcEhidokari_c::executeState_Wait() {}
void dAcEhidokari_c::finalizeState_Wait() {}
void dAcEhidokari_c::initializeState_Walk() {}
void dAcEhidokari_c::executeState_Walk() {}
void dAcEhidokari_c::finalizeState_Walk() {}
void dAcEhidokari_c::initializeState_Kyoro() {}
void dAcEhidokari_c::executeState_Kyoro() {}
void dAcEhidokari_c::finalizeState_Kyoro() {}
void dAcEhidokari_c::initializeState_Find() {}
void dAcEhidokari_c::executeState_Find() {}
void dAcEhidokari_c::finalizeState_Find() {}
void dAcEhidokari_c::initializeState_Run() {}
void dAcEhidokari_c::executeState_Run() {}
void dAcEhidokari_c::finalizeState_Run() {}
void dAcEhidokari_c::initializeState_Inside() {}
void dAcEhidokari_c::executeState_Inside() {}
void dAcEhidokari_c::finalizeState_Inside() {}
void dAcEhidokari_c::initializeState_Stun() {}
void dAcEhidokari_c::executeState_Stun() {}
void dAcEhidokari_c::finalizeState_Stun() {}
void dAcEhidokari_c::initializeState_WaitShell() {}
void dAcEhidokari_c::executeState_WaitShell() {}
void dAcEhidokari_c::finalizeState_WaitShell() {}
void dAcEhidokari_c::initializeState_RunShell() {}
void dAcEhidokari_c::executeState_RunShell() {}
void dAcEhidokari_c::finalizeState_RunShell() {}
void dAcEhidokari_c::initializeState_WalkShell() {}
void dAcEhidokari_c::executeState_WalkShell() {}
void dAcEhidokari_c::finalizeState_WalkShell() {}
void dAcEhidokari_c::initializeState_FireShell() {}
void dAcEhidokari_c::executeState_FireShell() {}
void dAcEhidokari_c::finalizeState_FireShell() {}
void dAcEhidokari_c::initializeState_RetShell() {}
void dAcEhidokari_c::executeState_RetShell() {}
void dAcEhidokari_c::finalizeState_RetShell() {}
void dAcEhidokari_c::initializeState_NoShell() {}
void dAcEhidokari_c::executeState_NoShell() {}
void dAcEhidokari_c::finalizeState_NoShell() {}
void dAcEhidokari_c::initializeState_Damage() {}
void dAcEhidokari_c::executeState_Damage() {}
void dAcEhidokari_c::finalizeState_Damage() {}
void dAcEhidokari_c::initializeState_GuradShell() {}
void dAcEhidokari_c::executeState_GuradShell() {}
void dAcEhidokari_c::finalizeState_GuradShell() {}
void dAcEhidokari_c::initializeState_Panic() {}
void dAcEhidokari_c::executeState_Panic() {}
void dAcEhidokari_c::finalizeState_Panic() {}
void dAcEhidokari_c::initializeState_Dead() {}
void dAcEhidokari_c::executeState_Dead() {}
void dAcEhidokari_c::finalizeState_Dead() {}
