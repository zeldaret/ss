#include <d/a/e/d_a_e_bc.h>

SPECIAL_ACTOR_PROFILE(E_BC, dAcEbc_c, fProfile::E_BC, 0XD0, 0, 4099);

STATE_DEFINE(dAcEbc_c, Standby);
STATE_DEFINE(dAcEbc_c, Home);
STATE_DEFINE(dAcEbc_c, Normal);
STATE_DEFINE(dAcEbc_c, Lantern);
STATE_DEFINE(dAcEbc_c, Yagura);
STATE_DEFINE(dAcEbc_c, Path);
STATE_DEFINE(dAcEbc_c, LanternPath);
STATE_DEFINE(dAcEbc_c, RopeWalk);
STATE_DEFINE(dAcEbc_c, FightStart);
STATE_DEFINE(dAcEbc_c, FindCall);
STATE_DEFINE(dAcEbc_c, Fight);
STATE_DEFINE(dAcEbc_c, Attack);
STATE_DEFINE(dAcEbc_c, BigAttack);
STATE_DEFINE(dAcEbc_c, StepAttack);
STATE_DEFINE(dAcEbc_c, RockAttack);
STATE_DEFINE(dAcEbc_c, CarryDamage);
STATE_DEFINE(dAcEbc_c, Damage);
STATE_DEFINE(dAcEbc_c, AirDamage);
STATE_DEFINE(dAcEbc_c, Fire);
STATE_DEFINE(dAcEbc_c, Fail);
STATE_DEFINE(dAcEbc_c, Fall);
STATE_DEFINE(dAcEbc_c, BombAction);
STATE_DEFINE(dAcEbc_c, StabGuard);
STATE_DEFINE(dAcEbc_c, Stun);
STATE_DEFINE(dAcEbc_c, BeeDamage);
STATE_DEFINE(dAcEbc_c, WhipCapture);
STATE_DEFINE(dAcEbc_c, WakidasiWait);
STATE_DEFINE(dAcEbc_c, SlopeSlide);
STATE_DEFINE(dAcEbc_c, Drop);
STATE_DEFINE(dAcEbc_c, MastDrop);
STATE_DEFINE(dAcEbc_c, AStandby);
STATE_DEFINE(dAcEbc_c, AFindCall);
STATE_DEFINE(dAcEbc_c, APath);
STATE_DEFINE(dAcEbc_c, AAttack);
STATE_DEFINE(dAcEbc_c, ADamage);

void dAcEbc_c::initializeState_Standby() {}
void dAcEbc_c::executeState_Standby() {}
void dAcEbc_c::finalizeState_Standby() {}
void dAcEbc_c::initializeState_Home() {}
void dAcEbc_c::executeState_Home() {}
void dAcEbc_c::finalizeState_Home() {}
void dAcEbc_c::initializeState_Normal() {}
void dAcEbc_c::executeState_Normal() {}
void dAcEbc_c::finalizeState_Normal() {}
void dAcEbc_c::initializeState_Lantern() {}
void dAcEbc_c::executeState_Lantern() {}
void dAcEbc_c::finalizeState_Lantern() {}
void dAcEbc_c::initializeState_Yagura() {}
void dAcEbc_c::executeState_Yagura() {}
void dAcEbc_c::finalizeState_Yagura() {}
void dAcEbc_c::initializeState_Path() {}
void dAcEbc_c::executeState_Path() {}
void dAcEbc_c::finalizeState_Path() {}
void dAcEbc_c::initializeState_LanternPath() {}
void dAcEbc_c::executeState_LanternPath() {}
void dAcEbc_c::finalizeState_LanternPath() {}
void dAcEbc_c::initializeState_RopeWalk() {}
void dAcEbc_c::executeState_RopeWalk() {}
void dAcEbc_c::finalizeState_RopeWalk() {}
void dAcEbc_c::initializeState_FightStart() {}
void dAcEbc_c::executeState_FightStart() {}
void dAcEbc_c::finalizeState_FightStart() {}
void dAcEbc_c::initializeState_FindCall() {}
void dAcEbc_c::executeState_FindCall() {}
void dAcEbc_c::finalizeState_FindCall() {}
void dAcEbc_c::initializeState_Fight() {}
void dAcEbc_c::executeState_Fight() {}
void dAcEbc_c::finalizeState_Fight() {}
void dAcEbc_c::initializeState_Attack() {}
void dAcEbc_c::executeState_Attack() {}
void dAcEbc_c::finalizeState_Attack() {}
void dAcEbc_c::initializeState_BigAttack() {}
void dAcEbc_c::executeState_BigAttack() {}
void dAcEbc_c::finalizeState_BigAttack() {}
void dAcEbc_c::initializeState_StepAttack() {}
void dAcEbc_c::executeState_StepAttack() {}
void dAcEbc_c::finalizeState_StepAttack() {}
void dAcEbc_c::initializeState_RockAttack() {}
void dAcEbc_c::executeState_RockAttack() {}
void dAcEbc_c::finalizeState_RockAttack() {}
void dAcEbc_c::initializeState_CarryDamage() {}
void dAcEbc_c::executeState_CarryDamage() {}
void dAcEbc_c::finalizeState_CarryDamage() {}
void dAcEbc_c::initializeState_Damage() {}
void dAcEbc_c::executeState_Damage() {}
void dAcEbc_c::finalizeState_Damage() {}
void dAcEbc_c::initializeState_AirDamage() {}
void dAcEbc_c::executeState_AirDamage() {}
void dAcEbc_c::finalizeState_AirDamage() {}
void dAcEbc_c::initializeState_Fire() {}
void dAcEbc_c::executeState_Fire() {}
void dAcEbc_c::finalizeState_Fire() {}
void dAcEbc_c::initializeState_Fail() {}
void dAcEbc_c::executeState_Fail() {}
void dAcEbc_c::finalizeState_Fail() {}
void dAcEbc_c::initializeState_Fall() {}
void dAcEbc_c::executeState_Fall() {}
void dAcEbc_c::finalizeState_Fall() {}
void dAcEbc_c::initializeState_BombAction() {}
void dAcEbc_c::executeState_BombAction() {}
void dAcEbc_c::finalizeState_BombAction() {}
void dAcEbc_c::initializeState_StabGuard() {}
void dAcEbc_c::executeState_StabGuard() {}
void dAcEbc_c::finalizeState_StabGuard() {}
void dAcEbc_c::initializeState_Stun() {}
void dAcEbc_c::executeState_Stun() {}
void dAcEbc_c::finalizeState_Stun() {}
void dAcEbc_c::initializeState_BeeDamage() {}
void dAcEbc_c::executeState_BeeDamage() {}
void dAcEbc_c::finalizeState_BeeDamage() {}
void dAcEbc_c::initializeState_WhipCapture() {}
void dAcEbc_c::executeState_WhipCapture() {}
void dAcEbc_c::finalizeState_WhipCapture() {}
void dAcEbc_c::initializeState_WakidasiWait() {}
void dAcEbc_c::executeState_WakidasiWait() {}
void dAcEbc_c::finalizeState_WakidasiWait() {}
void dAcEbc_c::initializeState_SlopeSlide() {}
void dAcEbc_c::executeState_SlopeSlide() {}
void dAcEbc_c::finalizeState_SlopeSlide() {}
void dAcEbc_c::initializeState_Drop() {}
void dAcEbc_c::executeState_Drop() {}
void dAcEbc_c::finalizeState_Drop() {}
void dAcEbc_c::initializeState_MastDrop() {}
void dAcEbc_c::executeState_MastDrop() {}
void dAcEbc_c::finalizeState_MastDrop() {}
void dAcEbc_c::initializeState_AStandby() {}
void dAcEbc_c::executeState_AStandby() {}
void dAcEbc_c::finalizeState_AStandby() {}
void dAcEbc_c::initializeState_AFindCall() {}
void dAcEbc_c::executeState_AFindCall() {}
void dAcEbc_c::finalizeState_AFindCall() {}
void dAcEbc_c::initializeState_APath() {}
void dAcEbc_c::executeState_APath() {}
void dAcEbc_c::finalizeState_APath() {}
void dAcEbc_c::initializeState_AAttack() {}
void dAcEbc_c::executeState_AAttack() {}
void dAcEbc_c::finalizeState_AAttack() {}
void dAcEbc_c::initializeState_ADamage() {}
void dAcEbc_c::executeState_ADamage() {}
void dAcEbc_c::finalizeState_ADamage() {}
