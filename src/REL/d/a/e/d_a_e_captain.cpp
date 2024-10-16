#include "d/a/e/d_a_e_captain.h"

SPECIAL_ACTOR_PROFILE(E_CAPTAIN, dAcEcaptain_c, fProfile::E_CAPTAIN, 0xBE, 0, 32);

STATE_DEFINE(dAcEcaptain_c, Opening);
STATE_DEFINE(dAcEcaptain_c, Wait);
STATE_DEFINE(dAcEcaptain_c, BalanceWait);
STATE_DEFINE(dAcEcaptain_c, Walk);
STATE_DEFINE(dAcEcaptain_c, Run);
STATE_DEFINE(dAcEcaptain_c, AttackSW);
STATE_DEFINE(dAcEcaptain_c, AttackF);
STATE_DEFINE(dAcEcaptain_c, AttackSP);
STATE_DEFINE(dAcEcaptain_c, Guard);
STATE_DEFINE(dAcEcaptain_c, GuardJust);
STATE_DEFINE(dAcEcaptain_c, Damage);
STATE_DEFINE(dAcEcaptain_c, Dead);
STATE_DEFINE(dAcEcaptain_c, Demo1);
STATE_DEFINE(dAcEcaptain_c, Demo2);
STATE_DEFINE(dAcEcaptain_c, OpeningType2);
STATE_DEFINE(dAcEcaptain_c, DeadType2);
STATE_DEFINE(dAcEcaptain_c, Demo1Type2);
STATE_DEFINE(dAcEcaptain_c, Demo2Type2);

void dAcEcaptain_c::initializeState_Opening() {}
void dAcEcaptain_c::executeState_Opening() {}
void dAcEcaptain_c::finalizeState_Opening() {}
void dAcEcaptain_c::initializeState_Wait() {}
void dAcEcaptain_c::executeState_Wait() {}
void dAcEcaptain_c::finalizeState_Wait() {}
void dAcEcaptain_c::initializeState_BalanceWait() {}
void dAcEcaptain_c::executeState_BalanceWait() {}
void dAcEcaptain_c::finalizeState_BalanceWait() {}
void dAcEcaptain_c::initializeState_Walk() {}
void dAcEcaptain_c::executeState_Walk() {}
void dAcEcaptain_c::finalizeState_Walk() {}
void dAcEcaptain_c::initializeState_Run() {}
void dAcEcaptain_c::executeState_Run() {}
void dAcEcaptain_c::finalizeState_Run() {}
void dAcEcaptain_c::initializeState_AttackSW() {}
void dAcEcaptain_c::executeState_AttackSW() {}
void dAcEcaptain_c::finalizeState_AttackSW() {}
void dAcEcaptain_c::initializeState_AttackF() {}
void dAcEcaptain_c::executeState_AttackF() {}
void dAcEcaptain_c::finalizeState_AttackF() {}
void dAcEcaptain_c::initializeState_AttackSP() {}
void dAcEcaptain_c::executeState_AttackSP() {}
void dAcEcaptain_c::finalizeState_AttackSP() {}
void dAcEcaptain_c::initializeState_Guard() {}
void dAcEcaptain_c::executeState_Guard() {}
void dAcEcaptain_c::finalizeState_Guard() {}
void dAcEcaptain_c::initializeState_GuardJust() {}
void dAcEcaptain_c::executeState_GuardJust() {}
void dAcEcaptain_c::finalizeState_GuardJust() {}
void dAcEcaptain_c::initializeState_Damage() {}
void dAcEcaptain_c::executeState_Damage() {}
void dAcEcaptain_c::finalizeState_Damage() {}
void dAcEcaptain_c::initializeState_Dead() {}
void dAcEcaptain_c::executeState_Dead() {}
void dAcEcaptain_c::finalizeState_Dead() {}
void dAcEcaptain_c::initializeState_Demo1() {}
void dAcEcaptain_c::executeState_Demo1() {}
void dAcEcaptain_c::finalizeState_Demo1() {}
void dAcEcaptain_c::initializeState_Demo2() {}
void dAcEcaptain_c::executeState_Demo2() {}
void dAcEcaptain_c::finalizeState_Demo2() {}
void dAcEcaptain_c::initializeState_OpeningType2() {}
void dAcEcaptain_c::executeState_OpeningType2() {}
void dAcEcaptain_c::finalizeState_OpeningType2() {}
void dAcEcaptain_c::initializeState_DeadType2() {}
void dAcEcaptain_c::executeState_DeadType2() {}
void dAcEcaptain_c::finalizeState_DeadType2() {}
void dAcEcaptain_c::initializeState_Demo1Type2() {}
void dAcEcaptain_c::executeState_Demo1Type2() {}
void dAcEcaptain_c::finalizeState_Demo1Type2() {}
void dAcEcaptain_c::initializeState_Demo2Type2() {}
void dAcEcaptain_c::executeState_Demo2Type2() {}
void dAcEcaptain_c::finalizeState_Demo2Type2() {}
