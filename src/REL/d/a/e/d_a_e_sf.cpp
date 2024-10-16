#include <d/a/e/d_a_e_sf.h>

SPECIAL_ACTOR_PROFILE(E_SF, dAcEsf_c, fProfile::E_SF, 0xC8, 0, 2);

STATE_DEFINE(dAcEsf_c, Standby);
STATE_DEFINE(dAcEsf_c, Home);
STATE_DEFINE(dAcEsf_c, Fight);
STATE_DEFINE(dAcEsf_c, Attack);
STATE_DEFINE(dAcEsf_c, AttackB);
STATE_DEFINE(dAcEsf_c, Attack2);
STATE_DEFINE(dAcEsf_c, Guard);
STATE_DEFINE(dAcEsf_c, Guard2);
STATE_DEFINE(dAcEsf_c, GuardJust);
STATE_DEFINE(dAcEsf_c, Damage);
STATE_DEFINE(dAcEsf_c, Fail);

void dAcEsf_c::initializeState_Standby() {}
void dAcEsf_c::executeState_Standby() {}
void dAcEsf_c::finalizeState_Standby() {}
void dAcEsf_c::initializeState_Home() {}
void dAcEsf_c::executeState_Home() {}
void dAcEsf_c::finalizeState_Home() {}
void dAcEsf_c::initializeState_Fight() {}
void dAcEsf_c::executeState_Fight() {}
void dAcEsf_c::finalizeState_Fight() {}
void dAcEsf_c::initializeState_Attack() {}
void dAcEsf_c::executeState_Attack() {}
void dAcEsf_c::finalizeState_Attack() {}
void dAcEsf_c::initializeState_AttackB() {}
void dAcEsf_c::executeState_AttackB() {}
void dAcEsf_c::finalizeState_AttackB() {}
void dAcEsf_c::initializeState_Attack2() {}
void dAcEsf_c::executeState_Attack2() {}
void dAcEsf_c::finalizeState_Attack2() {}
void dAcEsf_c::initializeState_Guard() {}
void dAcEsf_c::executeState_Guard() {}
void dAcEsf_c::finalizeState_Guard() {}
void dAcEsf_c::initializeState_Guard2() {}
void dAcEsf_c::executeState_Guard2() {}
void dAcEsf_c::finalizeState_Guard2() {}
void dAcEsf_c::initializeState_GuardJust() {}
void dAcEsf_c::executeState_GuardJust() {}
void dAcEsf_c::finalizeState_GuardJust() {}
void dAcEsf_c::initializeState_Damage() {}
void dAcEsf_c::executeState_Damage() {}
void dAcEsf_c::finalizeState_Damage() {}
void dAcEsf_c::initializeState_Fail() {}
void dAcEsf_c::executeState_Fail() {}
void dAcEsf_c::finalizeState_Fail() {}
