#include "d/a/obj/d_a_obj_fortune_ring.h"

SPECIAL_ACTOR_PROFILE(OBJ_FORTUNE_RING, dAcOfortuneRing_c, fProfile::OBJ_FORTUNE_RING, 0x1C6, 0, 0);

STATE_DEFINE(dAcOfortuneRing_c, Wait);
STATE_DEFINE(dAcOfortuneRing_c, Play);

dAcOfortuneRing_c::dAcOfortuneRing_c() : mStateMgr(*this) {}
dAcOfortuneRing_c::~dAcOfortuneRing_c() {}

void dAcOfortuneRing_c::initializeState_Wait() {}
void dAcOfortuneRing_c::executeState_Wait() {}
void dAcOfortuneRing_c::finalizeState_Wait() {}
void dAcOfortuneRing_c::initializeState_Play() {}
void dAcOfortuneRing_c::executeState_Play() {}
void dAcOfortuneRing_c::finalizeState_Play() {}
