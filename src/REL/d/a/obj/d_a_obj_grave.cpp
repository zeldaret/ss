#include <d/a/obj/d_a_obj_grave.h>

SPECIAL_ACTOR_PROFILE(OBJ_GRAVE, dAcOGrave_c, fProfile::OBJ_GRAVE, 0x255, 0, 2);

STATE_DEFINE(dAcOGrave_c, Wait);
STATE_DEFINE(dAcOGrave_c, Move);

void dAcOGrave_c::initializeState_Wait() {}
void dAcOGrave_c::executeState_Wait() {}
void dAcOGrave_c::finalizeState_Wait() {}
void dAcOGrave_c::initializeState_Move() {}
void dAcOGrave_c::executeState_Move() {}
void dAcOGrave_c::finalizeState_Move() {}
