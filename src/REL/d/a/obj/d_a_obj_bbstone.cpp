#include "d/a/obj/d_a_obj_bbstone.h"

SPECIAL_ACTOR_PROFILE(OBJ_BSTN, dAcOBBStone, fProfile::OBJ_BSTN, 0xA5, 0, 3);

STATE_DEFINE(dAcOBBStone, Wait);
STATE_DEFINE(dAcOBBStone, FirstWait);
STATE_DEFINE(dAcOBBStone, FirstDemo);

void dAcOBBStone::initializeState_Wait() {}
void dAcOBBStone::executeState_Wait() {}
void dAcOBBStone::finalizeState_Wait() {}
void dAcOBBStone::initializeState_FirstWait() {}
void dAcOBBStone::executeState_FirstWait() {}
void dAcOBBStone::finalizeState_FirstWait() {}
void dAcOBBStone::initializeState_FirstDemo() {}
void dAcOBBStone::executeState_FirstDemo() {}
void dAcOBBStone::finalizeState_FirstDemo() {}
