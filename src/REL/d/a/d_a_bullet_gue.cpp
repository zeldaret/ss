#include <d/a/d_a_bullet_gue.h>

SPECIAL_ACTOR_PROFILE(GUE_BULLET, dAcBulletGue_c, fProfile::GUE_BULLET, 0X105, 0, 2);

STATE_DEFINE(dAcBulletGue_c, Wait);
STATE_DEFINE(dAcBulletGue_c, Hold);
STATE_DEFINE(dAcBulletGue_c, TakeOff);

void dAcBulletGue_c::initializeState_Wait() {}
void dAcBulletGue_c::executeState_Wait() {}
void dAcBulletGue_c::finalizeState_Wait() {}
void dAcBulletGue_c::initializeState_Hold() {}
void dAcBulletGue_c::executeState_Hold() {}
void dAcBulletGue_c::finalizeState_Hold() {}
void dAcBulletGue_c::initializeState_TakeOff() {}
void dAcBulletGue_c::executeState_TakeOff() {}
void dAcBulletGue_c::finalizeState_TakeOff() {}
