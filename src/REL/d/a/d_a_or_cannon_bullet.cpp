#include "d/a/d_a_or_cannon_bullet.h"

SPECIAL_ACTOR_PROFILE(OR_CANN_BULLET, dAcOrCannBullet_c, fProfile::OR_CANN_BULLET, 0xF5, 0, 2);

STATE_DEFINE(dAcOrCannBullet_c, Wait);

void dAcOrCannBullet_c::initializeState_Wait() {}
void dAcOrCannBullet_c::executeState_Wait() {}
void dAcOrCannBullet_c::finalizeState_Wait() {}
