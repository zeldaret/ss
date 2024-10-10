#include <d/a/obj/d_a_obj_D3_dummy.h>

SPECIAL_ACTOR_PROFILE(OBJ_D3_DUMMY, dAcOD3Dummy_c, fProfile::OBJ_D3_DUMMY, 0x15E, 0, 6);

STATE_DEFINE(dAcOD3Dummy_c, Wait);

void dAcOD3Dummy_c::initializeState_Wait() {}
void dAcOD3Dummy_c::executeState_Wait() {}
void dAcOD3Dummy_c::finalizeState_Wait() {}
