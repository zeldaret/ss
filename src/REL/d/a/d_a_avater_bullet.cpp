#include <d/a/d_a_avater_bullet.h>

SPECIAL_ACTOR_PROFILE(AVATER_BULLET, dAcAvater_Bullet_c, fProfile::AVATER_BULLET, 0XB0, 0, 0);

STATE_DEFINE(dAcAvater_Bullet_c, Move);
STATE_DEFINE(dAcAvater_Bullet_c, Wait);

void dAcAvater_Bullet_c::initializeState_Move() {}
void dAcAvater_Bullet_c::executeState_Move() {}
void dAcAvater_Bullet_c::finalizeState_Move() {}
void dAcAvater_Bullet_c::initializeState_Wait() {}
void dAcAvater_Bullet_c::executeState_Wait() {}
void dAcAvater_Bullet_c::finalizeState_Wait() {}
