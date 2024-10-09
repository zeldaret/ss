#include <d/a/obj/d_a_obj_sand_floor.h>

SPECIAL_ACTOR_PROFILE(OBJ_SAND_FLOOR, dAcOSandFloor_c, fProfile::OBJ_SAND_FLOOR, 0X176, 0, 7);

STATE_DEFINE(dAcOSandFloor_c, Wait);

void dAcOSandFloor_c::initializeState_Wait() {}
void dAcOSandFloor_c::executeState_Wait() {}
void dAcOSandFloor_c::finalizeState_Wait() {}
