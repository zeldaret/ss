#include <d/a/e/d_a_e_or.h>

SPECIAL_ACTOR_PROFILE(E_OR, dAcEor_c, fProfile::E_OR, 0xF3, 0, 3);

STATE_DEFINE(dAcEor_c, Wait);
STATE_DEFINE(dAcEor_c, Appear);
STATE_DEFINE(dAcEor_c, Attack);
STATE_DEFINE(dAcEor_c, AttackCarry);
STATE_DEFINE(dAcEor_c, Stun);
STATE_DEFINE(dAcEor_c, Damage);
STATE_DEFINE(dAcEor_c, Hide);
STATE_DEFINE(dAcEor_c, Kyoro);

void dAcEor_c::initializeState_Wait() {}
void dAcEor_c::executeState_Wait() {}
void dAcEor_c::finalizeState_Wait() {}
void dAcEor_c::initializeState_Appear() {}
void dAcEor_c::executeState_Appear() {}
void dAcEor_c::finalizeState_Appear() {}
void dAcEor_c::initializeState_Attack() {}
void dAcEor_c::executeState_Attack() {}
void dAcEor_c::finalizeState_Attack() {}
void dAcEor_c::initializeState_AttackCarry() {}
void dAcEor_c::executeState_AttackCarry() {}
void dAcEor_c::finalizeState_AttackCarry() {}
void dAcEor_c::initializeState_Stun() {}
void dAcEor_c::executeState_Stun() {}
void dAcEor_c::finalizeState_Stun() {}
void dAcEor_c::initializeState_Damage() {}
void dAcEor_c::executeState_Damage() {}
void dAcEor_c::finalizeState_Damage() {}
void dAcEor_c::initializeState_Hide() {}
void dAcEor_c::executeState_Hide() {}
void dAcEor_c::finalizeState_Hide() {}
void dAcEor_c::initializeState_Kyoro() {}
void dAcEor_c::executeState_Kyoro() {}
void dAcEor_c::finalizeState_Kyoro() {}
