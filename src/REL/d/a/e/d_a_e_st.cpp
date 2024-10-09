#include <d/a/e/d_a_e_st.h>

SPECIAL_ACTOR_PROFILE(E_ST, dAcEst_c, fProfile::E_ST, 0XCA, 0, 2);

STATE_DEFINE(dAcEst_c, Home);
STATE_DEFINE(dAcEst_c, HomeLine);
STATE_DEFINE(dAcEst_c, HangHide);
STATE_DEFINE(dAcEst_c, HangWait);
STATE_DEFINE(dAcEst_c, HangAttack);
STATE_DEFINE(dAcEst_c, HangStun);
STATE_DEFINE(dAcEst_c, Drop);
STATE_DEFINE(dAcEst_c, NormalWalk);
STATE_DEFINE(dAcEst_c, NormalWait);
STATE_DEFINE(dAcEst_c, Fight);
STATE_DEFINE(dAcEst_c, JumpAttack);
STATE_DEFINE(dAcEst_c, WireAttack);
STATE_DEFINE(dAcEst_c, Hug);
STATE_DEFINE(dAcEst_c, HugWalk);
STATE_DEFINE(dAcEst_c, CancelHug);
STATE_DEFINE(dAcEst_c, Stun);
STATE_DEFINE(dAcEst_c, StandEnd);
STATE_DEFINE(dAcEst_c, StandStun);
STATE_DEFINE(dAcEst_c, StandHit);
STATE_DEFINE(dAcEst_c, Defense);
STATE_DEFINE(dAcEst_c, Dead);
STATE_DEFINE(dAcEst_c, WaterDead);
STATE_DEFINE(dAcEst_c, StandDead);
STATE_DEFINE(dAcEst_c, Reverse);
STATE_DEFINE(dAcEst_c, SpiderWait);
STATE_DEFINE(dAcEst_c, SpiderMove);
STATE_DEFINE(dAcEst_c, SpiderAttack);
STATE_DEFINE(dAcEst_c, SpiderHome);
STATE_DEFINE(dAcEst_c, SpiderFang);
STATE_DEFINE(dAcEst_c, WindGuard);

void dAcEst_c::initializeState_Home() {}
void dAcEst_c::executeState_Home() {}
void dAcEst_c::finalizeState_Home() {}
void dAcEst_c::initializeState_HomeLine() {}
void dAcEst_c::executeState_HomeLine() {}
void dAcEst_c::finalizeState_HomeLine() {}
void dAcEst_c::initializeState_HangHide() {}
void dAcEst_c::executeState_HangHide() {}
void dAcEst_c::finalizeState_HangHide() {}
void dAcEst_c::initializeState_HangWait() {}
void dAcEst_c::executeState_HangWait() {}
void dAcEst_c::finalizeState_HangWait() {}
void dAcEst_c::initializeState_HangAttack() {}
void dAcEst_c::executeState_HangAttack() {}
void dAcEst_c::finalizeState_HangAttack() {}
void dAcEst_c::initializeState_HangStun() {}
void dAcEst_c::executeState_HangStun() {}
void dAcEst_c::finalizeState_HangStun() {}
void dAcEst_c::initializeState_Drop() {}
void dAcEst_c::executeState_Drop() {}
void dAcEst_c::finalizeState_Drop() {}
void dAcEst_c::initializeState_NormalWalk() {}
void dAcEst_c::executeState_NormalWalk() {}
void dAcEst_c::finalizeState_NormalWalk() {}
void dAcEst_c::initializeState_NormalWait() {}
void dAcEst_c::executeState_NormalWait() {}
void dAcEst_c::finalizeState_NormalWait() {}
void dAcEst_c::initializeState_Fight() {}
void dAcEst_c::executeState_Fight() {}
void dAcEst_c::finalizeState_Fight() {}
void dAcEst_c::initializeState_JumpAttack() {}
void dAcEst_c::executeState_JumpAttack() {}
void dAcEst_c::finalizeState_JumpAttack() {}
void dAcEst_c::initializeState_WireAttack() {}
void dAcEst_c::executeState_WireAttack() {}
void dAcEst_c::finalizeState_WireAttack() {}
void dAcEst_c::initializeState_Hug() {}
void dAcEst_c::executeState_Hug() {}
void dAcEst_c::finalizeState_Hug() {}
void dAcEst_c::initializeState_HugWalk() {}
void dAcEst_c::executeState_HugWalk() {}
void dAcEst_c::finalizeState_HugWalk() {}
void dAcEst_c::initializeState_CancelHug() {}
void dAcEst_c::executeState_CancelHug() {}
void dAcEst_c::finalizeState_CancelHug() {}
void dAcEst_c::initializeState_Stun() {}
void dAcEst_c::executeState_Stun() {}
void dAcEst_c::finalizeState_Stun() {}
void dAcEst_c::initializeState_StandEnd() {}
void dAcEst_c::executeState_StandEnd() {}
void dAcEst_c::finalizeState_StandEnd() {}
void dAcEst_c::initializeState_StandStun() {}
void dAcEst_c::executeState_StandStun() {}
void dAcEst_c::finalizeState_StandStun() {}
void dAcEst_c::initializeState_StandHit() {}
void dAcEst_c::executeState_StandHit() {}
void dAcEst_c::finalizeState_StandHit() {}
void dAcEst_c::initializeState_Defense() {}
void dAcEst_c::executeState_Defense() {}
void dAcEst_c::finalizeState_Defense() {}
void dAcEst_c::initializeState_Dead() {}
void dAcEst_c::executeState_Dead() {}
void dAcEst_c::finalizeState_Dead() {}
void dAcEst_c::initializeState_WaterDead() {}
void dAcEst_c::executeState_WaterDead() {}
void dAcEst_c::finalizeState_WaterDead() {}
void dAcEst_c::initializeState_StandDead() {}
void dAcEst_c::executeState_StandDead() {}
void dAcEst_c::finalizeState_StandDead() {}
void dAcEst_c::initializeState_Reverse() {}
void dAcEst_c::executeState_Reverse() {}
void dAcEst_c::finalizeState_Reverse() {}
void dAcEst_c::initializeState_SpiderWait() {}
void dAcEst_c::executeState_SpiderWait() {}
void dAcEst_c::finalizeState_SpiderWait() {}
void dAcEst_c::initializeState_SpiderMove() {}
void dAcEst_c::executeState_SpiderMove() {}
void dAcEst_c::finalizeState_SpiderMove() {}
void dAcEst_c::initializeState_SpiderAttack() {}
void dAcEst_c::executeState_SpiderAttack() {}
void dAcEst_c::finalizeState_SpiderAttack() {}
void dAcEst_c::initializeState_SpiderHome() {}
void dAcEst_c::executeState_SpiderHome() {}
void dAcEst_c::finalizeState_SpiderHome() {}
void dAcEst_c::initializeState_SpiderFang() {}
void dAcEst_c::executeState_SpiderFang() {}
void dAcEst_c::finalizeState_SpiderFang() {}
void dAcEst_c::initializeState_WindGuard() {}
void dAcEst_c::executeState_WindGuard() {}
void dAcEst_c::finalizeState_WindGuard() {}
