#include "d/a/obj/d_a_obj_farmland.h"

SPECIAL_ACTOR_PROFILE(OBJ_FARMLAND, dAcOFarmLand_c, fProfile::OBJ_FARMLAND, 0x155, 0, 3);

STATE_DEFINE(dAcOFarmLand_c, Wait);
STATE_DEFINE(dAcOFarmLand_c, DummyEvent);

void dAcOFarmLand_c::initializeState_Wait() {}
void dAcOFarmLand_c::executeState_Wait() {}
void dAcOFarmLand_c::finalizeState_Wait() {}
void dAcOFarmLand_c::initializeState_DummyEvent() {}
void dAcOFarmLand_c::executeState_DummyEvent() {}
void dAcOFarmLand_c::finalizeState_DummyEvent() {}
