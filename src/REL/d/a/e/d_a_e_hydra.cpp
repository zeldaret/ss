#include <d/a/e/d_a_e_hydra.h>

SPECIAL_ACTOR_PROFILE(E_HYDRA, dAcEhydra_c, fProfile::E_HYDRA, 0XE6, 0, 4131);

STATE_DEFINE(dAcEhydra_c, Wait);
STATE_DEFINE(dAcEhydra_c, Walk);
STATE_DEFINE(dAcEhydra_c, Run);
STATE_DEFINE(dAcEhydra_c, Ret);
STATE_DEFINE(dAcEhydra_c, Attack);
STATE_DEFINE(dAcEhydra_c, AttackWait);
STATE_DEFINE(dAcEhydra_c, Recover);
STATE_DEFINE(dAcEhydra_c, Stun);
STATE_DEFINE(dAcEhydra_c, Damage);
STATE_DEFINE(dAcEhydra_c, EtcDamage);
STATE_DEFINE(dAcEhydra_c, Dead);

void dAcEhydra_c::initializeState_Wait() {}
void dAcEhydra_c::executeState_Wait() {}
void dAcEhydra_c::finalizeState_Wait() {}
void dAcEhydra_c::initializeState_Walk() {}
void dAcEhydra_c::executeState_Walk() {}
void dAcEhydra_c::finalizeState_Walk() {}
void dAcEhydra_c::initializeState_Run() {}
void dAcEhydra_c::executeState_Run() {}
void dAcEhydra_c::finalizeState_Run() {}
void dAcEhydra_c::initializeState_Ret() {}
void dAcEhydra_c::executeState_Ret() {}
void dAcEhydra_c::finalizeState_Ret() {}
void dAcEhydra_c::initializeState_Attack() {}
void dAcEhydra_c::executeState_Attack() {}
void dAcEhydra_c::finalizeState_Attack() {}
void dAcEhydra_c::initializeState_AttackWait() {}
void dAcEhydra_c::executeState_AttackWait() {}
void dAcEhydra_c::finalizeState_AttackWait() {}
void dAcEhydra_c::initializeState_Recover() {}
void dAcEhydra_c::executeState_Recover() {}
void dAcEhydra_c::finalizeState_Recover() {}
void dAcEhydra_c::initializeState_Stun() {}
void dAcEhydra_c::executeState_Stun() {}
void dAcEhydra_c::finalizeState_Stun() {}
void dAcEhydra_c::initializeState_Damage() {}
void dAcEhydra_c::executeState_Damage() {}
void dAcEhydra_c::finalizeState_Damage() {}
void dAcEhydra_c::initializeState_EtcDamage() {}
void dAcEhydra_c::executeState_EtcDamage() {}
void dAcEhydra_c::finalizeState_EtcDamage() {}
void dAcEhydra_c::initializeState_Dead() {}
void dAcEhydra_c::executeState_Dead() {}
void dAcEhydra_c::finalizeState_Dead() {}
