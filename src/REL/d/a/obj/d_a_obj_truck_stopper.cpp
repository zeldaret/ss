#include <d/a/obj/d_a_obj_truck_stopper.h>

SPECIAL_ACTOR_PROFILE(OBJ_TRUCK_STOPPER, dAcOtruckStopper_c, fProfile::OBJ_TRUCK_STOPPER, 0x200, 0, 3);

STATE_DEFINE(dAcOtruckStopper_c, Wait);
STATE_DEFINE(dAcOtruckStopper_c, Break);

void dAcOtruckStopper_c::initializeState_Wait() {}
void dAcOtruckStopper_c::executeState_Wait() {}
void dAcOtruckStopper_c::finalizeState_Wait() {}
void dAcOtruckStopper_c::initializeState_Break() {}
void dAcOtruckStopper_c::executeState_Break() {}
void dAcOtruckStopper_c::finalizeState_Break() {}
