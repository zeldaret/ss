#include "d/t/d_t_sparks.h"

SPECIAL_ACTOR_PROFILE(SPARKS_TAG, dTgSparksTag, fProfile::SPARKS_TAG, 0x19, 0, 4);

STATE_DEFINE(dTgSparksTag, Wait);

void dTgSparksTag::initializeState_Wait() {}
void dTgSparksTag::executeState_Wait() {}
void dTgSparksTag::finalizeState_Wait() {}
