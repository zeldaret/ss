#include "d/a/obj/d_a_obj_ballista_handle.h"

SPECIAL_ACTOR_PROFILE(OBJ_BALLISTA_HANDLE, dAcOBallistaHandle_c, fProfile::OBJ_BALLISTA_HANDLE, 0x23D, 0, 6);

STATE_DEFINE(dAcOBallistaHandle_c, Wait);

void dAcOBallistaHandle_c::initializeState_Wait() {}
void dAcOBallistaHandle_c::executeState_Wait() {}
void dAcOBallistaHandle_c::finalizeState_Wait() {}
