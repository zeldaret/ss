#include <d/a/e/d_a_e_gunho.h>

SPECIAL_ACTOR_PROFILE(E_GUNHO, dAcEgunho_c, fProfile::E_GUNHO, 0XE7, 0, 4099);

STATE_DEFINE(dAcEgunho_c, OpenClose);
STATE_DEFINE(dAcEgunho_c, Wait);
STATE_DEFINE(dAcEgunho_c, NormalMove);
STATE_DEFINE(dAcEgunho_c, Search);
STATE_DEFINE(dAcEgunho_c, AttackBullet);
STATE_DEFINE(dAcEgunho_c, AttackBomb);
STATE_DEFINE(dAcEgunho_c, Damage);
STATE_DEFINE(dAcEgunho_c, Dead);

void dAcEgunho_c::initializeState_OpenClose() {}
void dAcEgunho_c::executeState_OpenClose() {}
void dAcEgunho_c::finalizeState_OpenClose() {}
void dAcEgunho_c::initializeState_Wait() {}
void dAcEgunho_c::executeState_Wait() {}
void dAcEgunho_c::finalizeState_Wait() {}
void dAcEgunho_c::initializeState_NormalMove() {}
void dAcEgunho_c::executeState_NormalMove() {}
void dAcEgunho_c::finalizeState_NormalMove() {}
void dAcEgunho_c::initializeState_Search() {}
void dAcEgunho_c::executeState_Search() {}
void dAcEgunho_c::finalizeState_Search() {}
void dAcEgunho_c::initializeState_AttackBullet() {}
void dAcEgunho_c::executeState_AttackBullet() {}
void dAcEgunho_c::finalizeState_AttackBullet() {}
void dAcEgunho_c::initializeState_AttackBomb() {}
void dAcEgunho_c::executeState_AttackBomb() {}
void dAcEgunho_c::finalizeState_AttackBomb() {}
void dAcEgunho_c::initializeState_Damage() {}
void dAcEgunho_c::executeState_Damage() {}
void dAcEgunho_c::finalizeState_Damage() {}
void dAcEgunho_c::initializeState_Dead() {}
void dAcEgunho_c::executeState_Dead() {}
void dAcEgunho_c::finalizeState_Dead() {}
