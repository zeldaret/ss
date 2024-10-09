#include <d/a/obj/d_a_obj_time_door_before.h>

SPECIAL_ACTOR_PROFILE(OBJ_TIME_DOOR_BEFORE, dAcOTimeDoorBefore_c, fProfile::OBJ_TIME_DOOR_BEFORE, 0x251, 0, 3);

STATE_DEFINE(dAcOTimeDoorBefore_c, Wait);

void dAcOTimeDoorBefore_c::initializeState_Wait() {}
void dAcOTimeDoorBefore_c::executeState_Wait() {}
void dAcOTimeDoorBefore_c::finalizeState_Wait() {}
