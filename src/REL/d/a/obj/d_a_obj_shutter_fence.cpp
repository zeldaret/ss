#include <d/a/obj/d_a_obj_shutter_fence.h>

SPECIAL_ACTOR_PROFILE(OBJ_SHUTTER_FENCE, dAcOshutterfence_c, fProfile::OBJ_SHUTTER_FENCE, 0x19F, 0, 6);

STATE_DEFINE(dAcOshutterfence_c, Wait);
STATE_DEFINE(dAcOshutterfence_c, Open);

void dAcOshutterfence_c::initializeState_Wait() {}
void dAcOshutterfence_c::executeState_Wait() {}
void dAcOshutterfence_c::finalizeState_Wait() {}
void dAcOshutterfence_c::initializeState_Open() {}
void dAcOshutterfence_c::executeState_Open() {}
void dAcOshutterfence_c::finalizeState_Open() {}
