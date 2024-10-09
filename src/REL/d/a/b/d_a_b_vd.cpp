#include <d/a/b/d_a_b_vd.h>

SPECIAL_ACTOR_PROFILE(B_VD, dAcBvd_c, fProfile::B_VD, 0XBC, 0, 32);

STATE_DEFINE(dAcBvd_c, Opening);
STATE_DEFINE(dAcBvd_c, Wait);
STATE_DEFINE(dAcBvd_c, WaitPlayer);
STATE_DEFINE(dAcBvd_c, Run);
STATE_DEFINE(dAcBvd_c, FireAttack);
STATE_DEFINE(dAcBvd_c, Eat);
STATE_DEFINE(dAcBvd_c, Damage);
STATE_DEFINE(dAcBvd_c, BombDamage);
STATE_DEFINE(dAcBvd_c, BodyReverse);
STATE_DEFINE(dAcBvd_c, InitMove);
STATE_DEFINE(dAcBvd_c, Dead);

void dAcBvd_c::initializeState_Opening() {}
void dAcBvd_c::executeState_Opening() {}
void dAcBvd_c::finalizeState_Opening() {}
void dAcBvd_c::initializeState_Wait() {}
void dAcBvd_c::executeState_Wait() {}
void dAcBvd_c::finalizeState_Wait() {}
void dAcBvd_c::initializeState_WaitPlayer() {}
void dAcBvd_c::executeState_WaitPlayer() {}
void dAcBvd_c::finalizeState_WaitPlayer() {}
void dAcBvd_c::initializeState_Run() {}
void dAcBvd_c::executeState_Run() {}
void dAcBvd_c::finalizeState_Run() {}
void dAcBvd_c::initializeState_FireAttack() {}
void dAcBvd_c::executeState_FireAttack() {}
void dAcBvd_c::finalizeState_FireAttack() {}
void dAcBvd_c::initializeState_Eat() {}
void dAcBvd_c::executeState_Eat() {}
void dAcBvd_c::finalizeState_Eat() {}
void dAcBvd_c::initializeState_Damage() {}
void dAcBvd_c::executeState_Damage() {}
void dAcBvd_c::finalizeState_Damage() {}
void dAcBvd_c::initializeState_BombDamage() {}
void dAcBvd_c::executeState_BombDamage() {}
void dAcBvd_c::finalizeState_BombDamage() {}
void dAcBvd_c::initializeState_BodyReverse() {}
void dAcBvd_c::executeState_BodyReverse() {}
void dAcBvd_c::finalizeState_BodyReverse() {}
void dAcBvd_c::initializeState_InitMove() {}
void dAcBvd_c::executeState_InitMove() {}
void dAcBvd_c::finalizeState_InitMove() {}
void dAcBvd_c::initializeState_Dead() {}
void dAcBvd_c::executeState_Dead() {}
void dAcBvd_c::finalizeState_Dead() {}
