#include <d/a/e/d_a_e_bs.h>

SPECIAL_ACTOR_PROFILE(E_BS, dAcEbs_c, fProfile::E_BS, 0XC7, 0, 2);

STATE_DEFINE(dAcEbs_c, Standby);
STATE_DEFINE(dAcEbs_c, StandbyB);
STATE_DEFINE(dAcEbs_c, Fight);
STATE_DEFINE(dAcEbs_c, FightB);
STATE_DEFINE(dAcEbs_c, Attack);
STATE_DEFINE(dAcEbs_c, Damage);
STATE_DEFINE(dAcEbs_c, Fire);
STATE_DEFINE(dAcEbs_c, Disappear);

void dAcEbs_c::initializeState_Standby() {}
void dAcEbs_c::executeState_Standby() {}
void dAcEbs_c::finalizeState_Standby() {}
void dAcEbs_c::initializeState_StandbyB() {}
void dAcEbs_c::executeState_StandbyB() {}
void dAcEbs_c::finalizeState_StandbyB() {}
void dAcEbs_c::initializeState_Fight() {}
void dAcEbs_c::executeState_Fight() {}
void dAcEbs_c::finalizeState_Fight() {}
void dAcEbs_c::initializeState_FightB() {}
void dAcEbs_c::executeState_FightB() {}
void dAcEbs_c::finalizeState_FightB() {}
void dAcEbs_c::initializeState_Attack() {}
void dAcEbs_c::executeState_Attack() {}
void dAcEbs_c::finalizeState_Attack() {}
void dAcEbs_c::initializeState_Damage() {}
void dAcEbs_c::executeState_Damage() {}
void dAcEbs_c::finalizeState_Damage() {}
void dAcEbs_c::initializeState_Fire() {}
void dAcEbs_c::executeState_Fire() {}
void dAcEbs_c::finalizeState_Fire() {}
void dAcEbs_c::initializeState_Disappear() {}
void dAcEbs_c::executeState_Disappear() {}
void dAcEbs_c::finalizeState_Disappear() {}
