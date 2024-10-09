#include <d/a/obj/d_a_obj_time_door.h>

SPECIAL_ACTOR_PROFILE(OBJ_TIME_DOOR, dAcOTimeDoor_c, fProfile::OBJ_TIME_DOOR, 0x250, 0, 6);

STATE_DEFINE(dAcOTimeDoor_c, Wait);

void dAcOTimeDoor_c::initializeState_Wait() {}
void dAcOTimeDoor_c::executeState_Wait() {}
void dAcOTimeDoor_c::finalizeState_Wait() {}
