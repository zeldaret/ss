#include "d/a/obj/d_a_obj_lock.h"

SPECIAL_ACTOR_PROFILE(OBJ_SHUTTER_LOCK, dAcOLock_c, fProfile::OBJ_SHUTTER_LOCK, 0x149, 0, 7);

STATE_DEFINE(dAcOLock_c, Wait);

void dAcOLock_c::initializeState_Wait() {}
void dAcOLock_c::executeState_Wait() {}
void dAcOLock_c::finalizeState_Wait() {}
