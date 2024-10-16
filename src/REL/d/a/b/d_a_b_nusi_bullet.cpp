#include "d/a/b/d_a_b_nusi_bullet.h"

SPECIAL_ACTOR_PROFILE(B_NUSI_BULLET, dAcNusi_Bullet_c, fProfile::B_NUSI_BULLET, 0xB5, 0, 0);

STATE_DEFINE(dAcNusi_Bullet_c, Move);
STATE_DEFINE(dAcNusi_Bullet_c, Wait);
STATE_DEFINE(dAcNusi_Bullet_c, Back);

void dAcNusi_Bullet_c::initializeState_Move() {}
void dAcNusi_Bullet_c::executeState_Move() {}
void dAcNusi_Bullet_c::finalizeState_Move() {}
void dAcNusi_Bullet_c::initializeState_Wait() {}
void dAcNusi_Bullet_c::executeState_Wait() {}
void dAcNusi_Bullet_c::finalizeState_Wait() {}
void dAcNusi_Bullet_c::initializeState_Back() {}
void dAcNusi_Bullet_c::executeState_Back() {}
void dAcNusi_Bullet_c::finalizeState_Back() {}
