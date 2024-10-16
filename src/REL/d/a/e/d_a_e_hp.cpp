#include "d/a/e/d_a_e_hp.h"

SPECIAL_ACTOR_PROFILE(E_HP, dAcEHp_c, fProfile::E_HP, 0x102, 0, 18);

STATE_DEFINE(dAcEHp_c, BobUpAndDown);
STATE_DEFINE(dAcEHp_c, Chase);
STATE_DEFINE(dAcEHp_c, Attack);
STATE_DEFINE(dAcEHp_c, AtkWait);
STATE_DEFINE(dAcEHp_c, Extract);
STATE_DEFINE(dAcEHp_c, Petrify);
STATE_DEFINE(dAcEHp_c, Fluster);
STATE_DEFINE(dAcEHp_c, Escape);
STATE_DEFINE(dAcEHp_c, Damage);
STATE_DEFINE(dAcEHp_c, VcmDmg);
STATE_DEFINE(dAcEHp_c, Stun);
STATE_DEFINE(dAcEHp_c, Die);
STATE_DEFINE(dAcEHp_c, Dummy);

void dAcEHp_c::initializeState_BobUpAndDown() {}
void dAcEHp_c::executeState_BobUpAndDown() {}
void dAcEHp_c::finalizeState_BobUpAndDown() {}
void dAcEHp_c::initializeState_Chase() {}
void dAcEHp_c::executeState_Chase() {}
void dAcEHp_c::finalizeState_Chase() {}
void dAcEHp_c::initializeState_Attack() {}
void dAcEHp_c::executeState_Attack() {}
void dAcEHp_c::finalizeState_Attack() {}
void dAcEHp_c::initializeState_AtkWait() {}
void dAcEHp_c::executeState_AtkWait() {}
void dAcEHp_c::finalizeState_AtkWait() {}
void dAcEHp_c::initializeState_Extract() {}
void dAcEHp_c::executeState_Extract() {}
void dAcEHp_c::finalizeState_Extract() {}
void dAcEHp_c::initializeState_Petrify() {}
void dAcEHp_c::executeState_Petrify() {}
void dAcEHp_c::finalizeState_Petrify() {}
void dAcEHp_c::initializeState_Fluster() {}
void dAcEHp_c::executeState_Fluster() {}
void dAcEHp_c::finalizeState_Fluster() {}
void dAcEHp_c::initializeState_Escape() {}
void dAcEHp_c::executeState_Escape() {}
void dAcEHp_c::finalizeState_Escape() {}
void dAcEHp_c::initializeState_Damage() {}
void dAcEHp_c::executeState_Damage() {}
void dAcEHp_c::finalizeState_Damage() {}
void dAcEHp_c::initializeState_VcmDmg() {}
void dAcEHp_c::executeState_VcmDmg() {}
void dAcEHp_c::finalizeState_VcmDmg() {}
void dAcEHp_c::initializeState_Stun() {}
void dAcEHp_c::executeState_Stun() {}
void dAcEHp_c::finalizeState_Stun() {}
void dAcEHp_c::initializeState_Die() {}
void dAcEHp_c::executeState_Die() {}
void dAcEHp_c::finalizeState_Die() {}
void dAcEHp_c::initializeState_Dummy() {}
void dAcEHp_c::executeState_Dummy() {}
void dAcEHp_c::finalizeState_Dummy() {}
