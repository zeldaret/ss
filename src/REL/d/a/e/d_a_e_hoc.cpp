#include "d/a/e/d_a_e_hoc.h"

SPECIAL_ACTOR_PROFILE(E_HOC, dAcEhoc_c, fProfile::E_HOC, 0xCC, 0, 3);

STATE_DEFINE(dAcEhoc_c, Fight);
STATE_DEFINE(dAcEhoc_c, Attack);
STATE_DEFINE(dAcEhoc_c, BigAttack);
STATE_DEFINE(dAcEhoc_c, AttackAfter);
STATE_DEFINE(dAcEhoc_c, Escape);
STATE_DEFINE(dAcEhoc_c, Damage);
STATE_DEFINE(dAcEhoc_c, Fire);
STATE_DEFINE(dAcEhoc_c, Fail);
STATE_DEFINE(dAcEhoc_c, StabGuard);
STATE_DEFINE(dAcEhoc_c, Stun);
STATE_DEFINE(dAcEhoc_c, DropOut);
STATE_DEFINE(dAcEhoc_c, Return);
STATE_DEFINE(dAcEhoc_c, DashReturn);
STATE_DEFINE(dAcEhoc_c, Wait);
STATE_DEFINE(dAcEhoc_c, Summon);
STATE_DEFINE(dAcEhoc_c, AirDamage);

void dAcEhoc_c::initializeState_Fight() {}
void dAcEhoc_c::executeState_Fight() {}
void dAcEhoc_c::finalizeState_Fight() {}
void dAcEhoc_c::initializeState_Attack() {}
void dAcEhoc_c::executeState_Attack() {}
void dAcEhoc_c::finalizeState_Attack() {}
void dAcEhoc_c::initializeState_BigAttack() {}
void dAcEhoc_c::executeState_BigAttack() {}
void dAcEhoc_c::finalizeState_BigAttack() {}
void dAcEhoc_c::initializeState_AttackAfter() {}
void dAcEhoc_c::executeState_AttackAfter() {}
void dAcEhoc_c::finalizeState_AttackAfter() {}
void dAcEhoc_c::initializeState_Escape() {}
void dAcEhoc_c::executeState_Escape() {}
void dAcEhoc_c::finalizeState_Escape() {}
void dAcEhoc_c::initializeState_Damage() {}
void dAcEhoc_c::executeState_Damage() {}
void dAcEhoc_c::finalizeState_Damage() {}
void dAcEhoc_c::initializeState_Fire() {}
void dAcEhoc_c::executeState_Fire() {}
void dAcEhoc_c::finalizeState_Fire() {}
void dAcEhoc_c::initializeState_Fail() {}
void dAcEhoc_c::executeState_Fail() {}
void dAcEhoc_c::finalizeState_Fail() {}
void dAcEhoc_c::initializeState_StabGuard() {}
void dAcEhoc_c::executeState_StabGuard() {}
void dAcEhoc_c::finalizeState_StabGuard() {}
void dAcEhoc_c::initializeState_Stun() {}
void dAcEhoc_c::executeState_Stun() {}
void dAcEhoc_c::finalizeState_Stun() {}
void dAcEhoc_c::initializeState_DropOut() {}
void dAcEhoc_c::executeState_DropOut() {}
void dAcEhoc_c::finalizeState_DropOut() {}
void dAcEhoc_c::initializeState_Return() {}
void dAcEhoc_c::executeState_Return() {}
void dAcEhoc_c::finalizeState_Return() {}
void dAcEhoc_c::initializeState_DashReturn() {}
void dAcEhoc_c::executeState_DashReturn() {}
void dAcEhoc_c::finalizeState_DashReturn() {}
void dAcEhoc_c::initializeState_Wait() {}
void dAcEhoc_c::executeState_Wait() {}
void dAcEhoc_c::finalizeState_Wait() {}
void dAcEhoc_c::initializeState_Summon() {}
void dAcEhoc_c::executeState_Summon() {}
void dAcEhoc_c::finalizeState_Summon() {}
void dAcEhoc_c::initializeState_AirDamage() {}
void dAcEhoc_c::executeState_AirDamage() {}
void dAcEhoc_c::finalizeState_AirDamage() {}
