#include <d/a/obj/d_a_obj_impa_door.h>

SPECIAL_ACTOR_PROFILE(OBJ_IMPA_DOOR, dAcOimpaDoor_c, fProfile::OBJ_IMPA_DOOR, 0x20C, 0, 3);

STATE_DEFINE(dAcOimpaDoor_c, Wait);

void dAcOimpaDoor_c::initializeState_Wait() {}
void dAcOimpaDoor_c::executeState_Wait() {}
void dAcOimpaDoor_c::finalizeState_Wait() {}
