#include "d/a/obj/d_a_obj_pot_sal.h"

SPECIAL_ACTOR_PROFILE(OBJ_POT_SAL, dAcOpotSal_c, fProfile::OBJ_POT_SAL, 0x86, 0, 2);

STATE_VIRTUAL_DEFINE(dAcOpotSal_c, DemoThrow);
STATE_VIRTUAL_DEFINE(dAcOpotSal_c, After);

void dAcOpotSal_c::initializeState_DemoThrow() {}
void dAcOpotSal_c::executeState_DemoThrow() {}
void dAcOpotSal_c::finalizeState_DemoThrow() {}

void dAcOpotSal_c::initializeState_After() {}
void dAcOpotSal_c::executeState_After() {}
void dAcOpotSal_c::finalizeState_After() {}
