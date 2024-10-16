#include "d/a/b/d_a_b_lastboss.h"

SPECIAL_ACTOR_PROFILE(B_LASTBOSS, dAcBlastboss_c, fProfile::B_LASTBOSS, 0xBA, 0, 0);

STATE_DEFINE(dAcBlastboss_c, Fight);
STATE_DEFINE(dAcBlastboss_c, Guard);
STATE_DEFINE(dAcBlastboss_c, GuardBreak);
STATE_DEFINE(dAcBlastboss_c, Attack);
STATE_DEFINE(dAcBlastboss_c, CounterAttack);
STATE_DEFINE(dAcBlastboss_c, PunchAttack);
STATE_DEFINE(dAcBlastboss_c, DashAttack);
STATE_DEFINE(dAcBlastboss_c, SmallAttack);
STATE_DEFINE(dAcBlastboss_c, ThunderAttack);
STATE_DEFINE(dAcBlastboss_c, Damage);
STATE_DEFINE(dAcBlastboss_c, SitDamage);
STATE_DEFINE(dAcBlastboss_c, Down);
STATE_DEFINE(dAcBlastboss_c, Stun);
STATE_DEFINE(dAcBlastboss_c, ThunderWait);

void dAcBlastboss_c::initializeState_Fight() {}
void dAcBlastboss_c::executeState_Fight() {}
void dAcBlastboss_c::finalizeState_Fight() {}
void dAcBlastboss_c::initializeState_Guard() {}
void dAcBlastboss_c::executeState_Guard() {}
void dAcBlastboss_c::finalizeState_Guard() {}
void dAcBlastboss_c::initializeState_GuardBreak() {}
void dAcBlastboss_c::executeState_GuardBreak() {}
void dAcBlastboss_c::finalizeState_GuardBreak() {}
void dAcBlastboss_c::initializeState_Attack() {}
void dAcBlastboss_c::executeState_Attack() {}
void dAcBlastboss_c::finalizeState_Attack() {}
void dAcBlastboss_c::initializeState_CounterAttack() {}
void dAcBlastboss_c::executeState_CounterAttack() {}
void dAcBlastboss_c::finalizeState_CounterAttack() {}
void dAcBlastboss_c::initializeState_PunchAttack() {}
void dAcBlastboss_c::executeState_PunchAttack() {}
void dAcBlastboss_c::finalizeState_PunchAttack() {}
void dAcBlastboss_c::initializeState_DashAttack() {}
void dAcBlastboss_c::executeState_DashAttack() {}
void dAcBlastboss_c::finalizeState_DashAttack() {}
void dAcBlastboss_c::initializeState_SmallAttack() {}
void dAcBlastboss_c::executeState_SmallAttack() {}
void dAcBlastboss_c::finalizeState_SmallAttack() {}
void dAcBlastboss_c::initializeState_ThunderAttack() {}
void dAcBlastboss_c::executeState_ThunderAttack() {}
void dAcBlastboss_c::finalizeState_ThunderAttack() {}
void dAcBlastboss_c::initializeState_Damage() {}
void dAcBlastboss_c::executeState_Damage() {}
void dAcBlastboss_c::finalizeState_Damage() {}
void dAcBlastboss_c::initializeState_SitDamage() {}
void dAcBlastboss_c::executeState_SitDamage() {}
void dAcBlastboss_c::finalizeState_SitDamage() {}
void dAcBlastboss_c::initializeState_Down() {}
void dAcBlastboss_c::executeState_Down() {}
void dAcBlastboss_c::finalizeState_Down() {}
void dAcBlastboss_c::initializeState_Stun() {}
void dAcBlastboss_c::executeState_Stun() {}
void dAcBlastboss_c::finalizeState_Stun() {}
void dAcBlastboss_c::initializeState_ThunderWait() {}
void dAcBlastboss_c::executeState_ThunderWait() {}
void dAcBlastboss_c::finalizeState_ThunderWait() {}
