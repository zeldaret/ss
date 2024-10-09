#include <d/a/obj/d_a_obj_chair.h>

SPECIAL_ACTOR_PROFILE(OBJ_CHAIR, dAcOChair_c, fProfile::OBJ_CHAIR, 0x196, 0, 7);

STATE_DEFINE(dAcOChair_c, Wait);

void dAcOChair_c::initializeState_Wait() {}
void dAcOChair_c::executeState_Wait() {}
void dAcOChair_c::finalizeState_Wait() {}
