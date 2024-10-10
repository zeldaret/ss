#include <d/a/d_a_hidory_fire.h>

SPECIAL_ACTOR_PROFILE(HIDORY_FIRE, dAcHidoryFire_c, fProfile::HIDORY_FIRE, 0x10B, 0, 0);

STATE_DEFINE(dAcHidoryFire_c, BulletMove);
STATE_DEFINE(dAcHidoryFire_c, Damage);

void dAcHidoryFire_c::initializeState_BulletMove() {}
void dAcHidoryFire_c::executeState_BulletMove() {}
void dAcHidoryFire_c::finalizeState_BulletMove() {}
void dAcHidoryFire_c::initializeState_Damage() {}
void dAcHidoryFire_c::executeState_Damage() {}
void dAcHidoryFire_c::finalizeState_Damage() {}
