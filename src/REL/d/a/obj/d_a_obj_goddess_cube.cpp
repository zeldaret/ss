#include <d/a/obj/d_a_obj_goddess_cube.h>

SPECIAL_ACTOR_PROFILE(OBJ_GODDESS_CUBE, dAcOGoddessCube_c, fProfile::OBJ_GODDESS_CUBE, 0X1A6, 0, 3);

STATE_DEFINE(dAcOGoddessCube_c, Wait);
STATE_DEFINE(dAcOGoddessCube_c, Shot);

void dAcOGoddessCube_c::initializeState_Wait() {}
void dAcOGoddessCube_c::executeState_Wait() {}
void dAcOGoddessCube_c::finalizeState_Wait() {}
void dAcOGoddessCube_c::initializeState_Shot() {}
void dAcOGoddessCube_c::executeState_Shot() {}
void dAcOGoddessCube_c::finalizeState_Shot() {}
