#include <d/a/e/d_a_e_sf4.h>

SPECIAL_ACTOR_PROFILE(E_SF4, dAcEsf4_c, fProfile::E_SF4, 0xC9, 0, 2);

STATE_DEFINE(dAcEsf4_c, Standby);
STATE_DEFINE(dAcEsf4_c, Fight);
STATE_DEFINE(dAcEsf4_c, Attack);
STATE_DEFINE(dAcEsf4_c, Guard);
STATE_DEFINE(dAcEsf4_c, Guard2);
STATE_DEFINE(dAcEsf4_c, GuardJust);
STATE_DEFINE(dAcEsf4_c, Counter);
STATE_DEFINE(dAcEsf4_c, Change4Sword);
STATE_DEFINE(dAcEsf4_c, Damage);
STATE_DEFINE(dAcEsf4_c, End);

void dAcEsf4_c::initializeState_Standby() {}
void dAcEsf4_c::executeState_Standby() {}
void dAcEsf4_c::finalizeState_Standby() {}
void dAcEsf4_c::initializeState_Fight() {}
void dAcEsf4_c::executeState_Fight() {}
void dAcEsf4_c::finalizeState_Fight() {}
void dAcEsf4_c::initializeState_Attack() {}
void dAcEsf4_c::executeState_Attack() {}
void dAcEsf4_c::finalizeState_Attack() {}
void dAcEsf4_c::initializeState_Guard() {}
void dAcEsf4_c::executeState_Guard() {}
void dAcEsf4_c::finalizeState_Guard() {}
void dAcEsf4_c::initializeState_Guard2() {}
void dAcEsf4_c::executeState_Guard2() {}
void dAcEsf4_c::finalizeState_Guard2() {}
void dAcEsf4_c::initializeState_GuardJust() {}
void dAcEsf4_c::executeState_GuardJust() {}
void dAcEsf4_c::finalizeState_GuardJust() {}
void dAcEsf4_c::initializeState_Counter() {}
void dAcEsf4_c::executeState_Counter() {}
void dAcEsf4_c::finalizeState_Counter() {}
void dAcEsf4_c::initializeState_Change4Sword() {}
void dAcEsf4_c::executeState_Change4Sword() {}
void dAcEsf4_c::finalizeState_Change4Sword() {}
void dAcEsf4_c::initializeState_Damage() {}
void dAcEsf4_c::executeState_Damage() {}
void dAcEsf4_c::finalizeState_Damage() {}
void dAcEsf4_c::initializeState_End() {}
void dAcEsf4_c::executeState_End() {}
void dAcEsf4_c::finalizeState_End() {}
