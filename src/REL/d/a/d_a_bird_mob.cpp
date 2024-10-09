#include <d/a/d_a_bird_mob.h>

SPECIAL_ACTOR_PROFILE(BIRD_MOB, dAcBirdMob_c, fProfile::BIRD_MOB, 0X24F, 0, 7);

STATE_DEFINE(dAcBirdMob_c, Wait);

void dAcBirdMob_c::initializeState_Wait() {}
void dAcBirdMob_c::executeState_Wait() {}
void dAcBirdMob_c::finalizeState_Wait() {}
