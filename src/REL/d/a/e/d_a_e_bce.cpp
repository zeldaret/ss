#include <d/a/e/d_a_e_bce.h>

SPECIAL_ACTOR_PROFILE(E_BCE, dAcEbce_c, fProfile::E_BCE, 0XD1, 0, 4099);

STATE_DEFINE(dAcEbce_c, Standby);
STATE_DEFINE(dAcEbce_c, Home);
STATE_DEFINE(dAcEbce_c, Normal);
STATE_DEFINE(dAcEbce_c, Path);
STATE_DEFINE(dAcEbce_c, FightStart);
STATE_DEFINE(dAcEbce_c, Fight);
STATE_DEFINE(dAcEbce_c, Attack);
STATE_DEFINE(dAcEbce_c, StepAttack);
STATE_DEFINE(dAcEbce_c, Damage);
STATE_DEFINE(dAcEbce_c, AirDamage);
STATE_DEFINE(dAcEbce_c, Fail);
STATE_DEFINE(dAcEbce_c, BombAction);
STATE_DEFINE(dAcEbce_c, StabGuard);
STATE_DEFINE(dAcEbce_c, Stun);
STATE_DEFINE(dAcEbce_c, Drop);

void dAcEbce_c::initializeState_Standby() {}
void dAcEbce_c::executeState_Standby() {}
void dAcEbce_c::finalizeState_Standby() {}
void dAcEbce_c::initializeState_Home() {}
void dAcEbce_c::executeState_Home() {}
void dAcEbce_c::finalizeState_Home() {}
void dAcEbce_c::initializeState_Normal() {}
void dAcEbce_c::executeState_Normal() {}
void dAcEbce_c::finalizeState_Normal() {}
void dAcEbce_c::initializeState_Path() {}
void dAcEbce_c::executeState_Path() {}
void dAcEbce_c::finalizeState_Path() {}
void dAcEbce_c::initializeState_FightStart() {}
void dAcEbce_c::executeState_FightStart() {}
void dAcEbce_c::finalizeState_FightStart() {}
void dAcEbce_c::initializeState_Fight() {}
void dAcEbce_c::executeState_Fight() {}
void dAcEbce_c::finalizeState_Fight() {}
void dAcEbce_c::initializeState_Attack() {}
void dAcEbce_c::executeState_Attack() {}
void dAcEbce_c::finalizeState_Attack() {}
void dAcEbce_c::initializeState_StepAttack() {}
void dAcEbce_c::executeState_StepAttack() {}
void dAcEbce_c::finalizeState_StepAttack() {}
void dAcEbce_c::initializeState_Damage() {}
void dAcEbce_c::executeState_Damage() {}
void dAcEbce_c::finalizeState_Damage() {}
void dAcEbce_c::initializeState_AirDamage() {}
void dAcEbce_c::executeState_AirDamage() {}
void dAcEbce_c::finalizeState_AirDamage() {}
void dAcEbce_c::initializeState_Fail() {}
void dAcEbce_c::executeState_Fail() {}
void dAcEbce_c::finalizeState_Fail() {}
void dAcEbce_c::initializeState_BombAction() {}
void dAcEbce_c::executeState_BombAction() {}
void dAcEbce_c::finalizeState_BombAction() {}
void dAcEbce_c::initializeState_StabGuard() {}
void dAcEbce_c::executeState_StabGuard() {}
void dAcEbce_c::finalizeState_StabGuard() {}
void dAcEbce_c::initializeState_Stun() {}
void dAcEbce_c::executeState_Stun() {}
void dAcEbce_c::finalizeState_Stun() {}
void dAcEbce_c::initializeState_Drop() {}
void dAcEbce_c::executeState_Drop() {}
void dAcEbce_c::finalizeState_Drop() {}
