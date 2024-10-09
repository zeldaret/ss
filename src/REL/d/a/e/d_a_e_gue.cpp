#include <d/a/e/d_a_e_gue.h>

SPECIAL_ACTOR_PROFILE(E_GUE, dAcEgue_c, fProfile::E_GUE, 0x104, 0, 3);

STATE_DEFINE(dAcEgue_c, Attack);
STATE_DEFINE(dAcEgue_c, Blow);
STATE_DEFINE(dAcEgue_c, Damage);
STATE_DEFINE(dAcEgue_c, FlyHome);
STATE_DEFINE(dAcEgue_c, FlyPlayer);
STATE_DEFINE(dAcEgue_c, Return);
STATE_DEFINE(dAcEgue_c, Stay);
STATE_DEFINE(dAcEgue_c, Stun);

void dAcEgue_c::initializeState_Attack() {}
void dAcEgue_c::executeState_Attack() {}
void dAcEgue_c::finalizeState_Attack() {}
void dAcEgue_c::initializeState_Blow() {}
void dAcEgue_c::executeState_Blow() {}
void dAcEgue_c::finalizeState_Blow() {}
void dAcEgue_c::initializeState_Damage() {}
void dAcEgue_c::executeState_Damage() {}
void dAcEgue_c::finalizeState_Damage() {}
void dAcEgue_c::initializeState_FlyHome() {}
void dAcEgue_c::executeState_FlyHome() {}
void dAcEgue_c::finalizeState_FlyHome() {}
void dAcEgue_c::initializeState_FlyPlayer() {}
void dAcEgue_c::executeState_FlyPlayer() {}
void dAcEgue_c::finalizeState_FlyPlayer() {}
void dAcEgue_c::initializeState_Return() {}
void dAcEgue_c::executeState_Return() {}
void dAcEgue_c::finalizeState_Return() {}
void dAcEgue_c::initializeState_Stay() {}
void dAcEgue_c::executeState_Stay() {}
void dAcEgue_c::finalizeState_Stay() {}
void dAcEgue_c::initializeState_Stun() {}
void dAcEgue_c::executeState_Stun() {}
void dAcEgue_c::finalizeState_Stun() {}
