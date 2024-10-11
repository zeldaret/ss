#include <d/a/e/d_a_e_ks.h>

SPECIAL_ACTOR_PROFILE(E_KS, dAcEKs_c, fProfile::E_KS, 0xFE, 0, 4099);

STATE_DEFINE(dAcEKs_c, Wait);
STATE_DEFINE(dAcEKs_c, WakeUp);
STATE_DEFINE(dAcEKs_c, ReturnToWait);
STATE_DEFINE(dAcEKs_c, WaitReady);
STATE_DEFINE(dAcEKs_c, Move);
STATE_DEFINE(dAcEKs_c, Chase);
STATE_DEFINE(dAcEKs_c, ChaseAttack);
STATE_DEFINE(dAcEKs_c, Fighting);
STATE_DEFINE(dAcEKs_c, AttackReady);
STATE_DEFINE(dAcEKs_c, Attack);
STATE_DEFINE(dAcEKs_c, Damage);
STATE_DEFINE(dAcEKs_c, Stun);
STATE_DEFINE(dAcEKs_c, WindBlow);
STATE_DEFINE(dAcEKs_c, PathMove);

void dAcEKs_c::initializeState_Wait() {}
void dAcEKs_c::executeState_Wait() {}
void dAcEKs_c::finalizeState_Wait() {}
void dAcEKs_c::initializeState_WakeUp() {}
void dAcEKs_c::executeState_WakeUp() {}
void dAcEKs_c::finalizeState_WakeUp() {}
void dAcEKs_c::initializeState_ReturnToWait() {}
void dAcEKs_c::executeState_ReturnToWait() {}
void dAcEKs_c::finalizeState_ReturnToWait() {}
void dAcEKs_c::initializeState_WaitReady() {}
void dAcEKs_c::executeState_WaitReady() {}
void dAcEKs_c::finalizeState_WaitReady() {}
void dAcEKs_c::initializeState_Move() {}
void dAcEKs_c::executeState_Move() {}
void dAcEKs_c::finalizeState_Move() {}
void dAcEKs_c::initializeState_Chase() {}
void dAcEKs_c::executeState_Chase() {}
void dAcEKs_c::finalizeState_Chase() {}
void dAcEKs_c::initializeState_ChaseAttack() {}
void dAcEKs_c::executeState_ChaseAttack() {}
void dAcEKs_c::finalizeState_ChaseAttack() {}
void dAcEKs_c::initializeState_Fighting() {}
void dAcEKs_c::executeState_Fighting() {}
void dAcEKs_c::finalizeState_Fighting() {}
void dAcEKs_c::initializeState_AttackReady() {}
void dAcEKs_c::executeState_AttackReady() {}
void dAcEKs_c::finalizeState_AttackReady() {}
void dAcEKs_c::initializeState_Attack() {}
void dAcEKs_c::executeState_Attack() {}
void dAcEKs_c::finalizeState_Attack() {}
void dAcEKs_c::initializeState_Damage() {}
void dAcEKs_c::executeState_Damage() {}
void dAcEKs_c::finalizeState_Damage() {}
void dAcEKs_c::initializeState_Stun() {}
void dAcEKs_c::executeState_Stun() {}
void dAcEKs_c::finalizeState_Stun() {}
void dAcEKs_c::initializeState_WindBlow() {}
void dAcEKs_c::executeState_WindBlow() {}
void dAcEKs_c::finalizeState_WindBlow() {}
void dAcEKs_c::initializeState_PathMove() {}
void dAcEKs_c::executeState_PathMove() {}
void dAcEKs_c::finalizeState_PathMove() {}
