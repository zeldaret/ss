#include "d/a/obj/d_a_obj_roulette.h"

SPECIAL_ACTOR_PROFILE(OBJ_ROULETTE, dAcObjRoulette_c, fProfile::OBJ_ROULETTE, 0x97, 0, 384);

STATE_VIRTUAL_DEFINE(dAcObjRoulette_c, DemoThrow);
STATE_VIRTUAL_DEFINE(dAcObjRoulette_c, Wait);

void dAcObjRoulette_c::initializeState_DemoThrow() {}
void dAcObjRoulette_c::executeState_DemoThrow() {}
void dAcObjRoulette_c::finalizeState_DemoThrow() {}

void dAcObjRoulette_c::initializeState_Wait() {}
void dAcObjRoulette_c::executeState_Wait() {}
void dAcObjRoulette_c::finalizeState_Wait() {}
