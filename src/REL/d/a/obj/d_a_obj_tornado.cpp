#include "d/a/obj/d_a_obj_tornado.h"

SPECIAL_ACTOR_PROFILE(OBJ_TORNADO, dAcOTornado_c, fProfile::OBJ_TORNADO, 0xDF, 0, 2);

STATE_DEFINE(dAcOTornado_c, Wait);

void dAcOTornado_c::initializeState_Wait() {}
void dAcOTornado_c::executeState_Wait() {}
void dAcOTornado_c::finalizeState_Wait() {}
