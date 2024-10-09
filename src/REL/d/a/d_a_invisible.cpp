#include <d/a/d_a_invisible.h>

SPECIAL_ACTOR_PROFILE(INVISIBLE, dAcInvisible, fProfile::INVISIBLE, 0X11D, 0, 0);

STATE_DEFINE(dAcInvisible, Wait);

void dAcInvisible::initializeState_Wait() {}
void dAcInvisible::executeState_Wait() {}
void dAcInvisible::finalizeState_Wait() {}
