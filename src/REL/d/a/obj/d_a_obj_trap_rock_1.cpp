#include "d/a/obj/d_a_obj_trap_rock_1.h"

SPECIAL_ACTOR_PROFILE(OBJ_TRAP_ROCK_1, dAcOtrapRock1_c, fProfile::OBJ_TRAP_ROCK_1, 0x26B, 0, 2);

STATE_DEFINE(dAcOtrapRock1_c, TrapWait);
STATE_DEFINE(dAcOtrapRock1_c, TrapAction);
STATE_DEFINE(dAcOtrapRock1_c, TrapReturn);

void dAcOtrapRock1_c::initializeState_TrapWait() {}
void dAcOtrapRock1_c::executeState_TrapWait() {}
void dAcOtrapRock1_c::finalizeState_TrapWait() {}
void dAcOtrapRock1_c::initializeState_TrapAction() {}
void dAcOtrapRock1_c::executeState_TrapAction() {}
void dAcOtrapRock1_c::finalizeState_TrapAction() {}
void dAcOtrapRock1_c::initializeState_TrapReturn() {}
void dAcOtrapRock1_c::executeState_TrapReturn() {}
void dAcOtrapRock1_c::finalizeState_TrapReturn() {}
