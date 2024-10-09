#include <d/a/e/d_a_e_bcal.h>

SPECIAL_ACTOR_PROFILE(E_BCAL, dAcEbcaL_c, fProfile::E_BCAL, 0XD2, 0, 2);

STATE_DEFINE(dAcEbcaL_c, Standby);
STATE_DEFINE(dAcEbcaL_c, Attack);
STATE_DEFINE(dAcEbcaL_c, Damage);
STATE_DEFINE(dAcEbcaL_c, Hit);
STATE_DEFINE(dAcEbcaL_c, Lead);
STATE_DEFINE(dAcEbcaL_c, Stun);
STATE_DEFINE(dAcEbcaL_c, AirDamage);
STATE_DEFINE(dAcEbcaL_c, Fail);
STATE_DEFINE(dAcEbcaL_c, StabGuard);

void dAcEbcaL_c::initializeState_Standby() {}
void dAcEbcaL_c::executeState_Standby() {}
void dAcEbcaL_c::finalizeState_Standby() {}
void dAcEbcaL_c::initializeState_Attack() {}
void dAcEbcaL_c::executeState_Attack() {}
void dAcEbcaL_c::finalizeState_Attack() {}
void dAcEbcaL_c::initializeState_Damage() {}
void dAcEbcaL_c::executeState_Damage() {}
void dAcEbcaL_c::finalizeState_Damage() {}
void dAcEbcaL_c::initializeState_Hit() {}
void dAcEbcaL_c::executeState_Hit() {}
void dAcEbcaL_c::finalizeState_Hit() {}
void dAcEbcaL_c::initializeState_Lead() {}
void dAcEbcaL_c::executeState_Lead() {}
void dAcEbcaL_c::finalizeState_Lead() {}
void dAcEbcaL_c::initializeState_Stun() {}
void dAcEbcaL_c::executeState_Stun() {}
void dAcEbcaL_c::finalizeState_Stun() {}
void dAcEbcaL_c::initializeState_AirDamage() {}
void dAcEbcaL_c::executeState_AirDamage() {}
void dAcEbcaL_c::finalizeState_AirDamage() {}
void dAcEbcaL_c::initializeState_Fail() {}
void dAcEbcaL_c::executeState_Fail() {}
void dAcEbcaL_c::finalizeState_Fail() {}
void dAcEbcaL_c::initializeState_StabGuard() {}
void dAcEbcaL_c::executeState_StabGuard() {}
void dAcEbcaL_c::finalizeState_StabGuard() {}
