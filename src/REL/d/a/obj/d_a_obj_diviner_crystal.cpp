#include "d/a/obj/d_a_obj_diviner_crystal.h"

SPECIAL_ACTOR_PROFILE(OBJ_DIVINER_CRYSTAL, dAcODivinerCrystal_c, fProfile::OBJ_DIVINER_CRYSTAL, 0x157, 0, 3);

STATE_VIRTUAL_DEFINE(dAcODivinerCrystal_c, DemoThrow);
STATE_DEFINE(dAcODivinerCrystal_c, Stay);

void dAcODivinerCrystal_c::initializeState_DemoThrow() {}
void dAcODivinerCrystal_c::executeState_DemoThrow() {}
void dAcODivinerCrystal_c::finalizeState_DemoThrow() {}

void dAcODivinerCrystal_c::initializeState_Stay() {}
void dAcODivinerCrystal_c::executeState_Stay() {}
void dAcODivinerCrystal_c::finalizeState_Stay() {}
