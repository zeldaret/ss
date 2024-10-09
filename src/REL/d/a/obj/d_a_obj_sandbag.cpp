#include <d/a/obj/d_a_obj_sandbag.h>

SPECIAL_ACTOR_PROFILE(OBJ_SANDBAG, dAcOSandbag_c, fProfile::OBJ_SANDBAG, 0x162, 0, 3);

STATE_DEFINE(dAcOSandbag_c, Wait);

void dAcOSandbag_c::initializeState_Wait() {}
void dAcOSandbag_c::executeState_Wait() {}
void dAcOSandbag_c::finalizeState_Wait() {}
