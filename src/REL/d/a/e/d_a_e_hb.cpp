#include <d/a/e/d_a_e_hb.h>

SPECIAL_ACTOR_PROFILE(E_HB, dAcEhb_c, fProfile::E_HB, 0xFF, 0, 4096);

STATE_DEFINE(dAcEhb_c, Under);
STATE_DEFINE(dAcEhb_c, Appear);
STATE_DEFINE(dAcEhb_c, Wait);
STATE_DEFINE(dAcEhb_c, Attack);
STATE_DEFINE(dAcEhb_c, Damage);
STATE_DEFINE(dAcEhb_c, Blow);
STATE_DEFINE(dAcEhb_c, Stun);
STATE_DEFINE(dAcEhb_c, DeadMouth);
STATE_DEFINE(dAcEhb_c, Avoid);

void dAcEhb_c::initializeState_Under() {}
void dAcEhb_c::executeState_Under() {}
void dAcEhb_c::finalizeState_Under() {}
void dAcEhb_c::initializeState_Appear() {}
void dAcEhb_c::executeState_Appear() {}
void dAcEhb_c::finalizeState_Appear() {}
void dAcEhb_c::initializeState_Wait() {}
void dAcEhb_c::executeState_Wait() {}
void dAcEhb_c::finalizeState_Wait() {}
void dAcEhb_c::initializeState_Attack() {}
void dAcEhb_c::executeState_Attack() {}
void dAcEhb_c::finalizeState_Attack() {}
void dAcEhb_c::initializeState_Damage() {}
void dAcEhb_c::executeState_Damage() {}
void dAcEhb_c::finalizeState_Damage() {}
void dAcEhb_c::initializeState_Blow() {}
void dAcEhb_c::executeState_Blow() {}
void dAcEhb_c::finalizeState_Blow() {}
void dAcEhb_c::initializeState_Stun() {}
void dAcEhb_c::executeState_Stun() {}
void dAcEhb_c::finalizeState_Stun() {}
void dAcEhb_c::initializeState_DeadMouth() {}
void dAcEhb_c::executeState_DeadMouth() {}
void dAcEhb_c::finalizeState_DeadMouth() {}
void dAcEhb_c::initializeState_Avoid() {}
void dAcEhb_c::executeState_Avoid() {}
void dAcEhb_c::finalizeState_Avoid() {}
