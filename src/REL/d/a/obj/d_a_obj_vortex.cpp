#include <d/a/obj/d_a_obj_vortex.h>

SPECIAL_ACTOR_PROFILE(OBJ_VORTEX, dAcOVortex_c, fProfile::OBJ_VORTEX, 0x246, 0, 0);

STATE_DEFINE(dAcOVortex_c, Wait);
STATE_DEFINE(dAcOVortex_c, Appear);

void dAcOVortex_c::initializeState_Wait() {}
void dAcOVortex_c::executeState_Wait() {}
void dAcOVortex_c::finalizeState_Wait() {}
void dAcOVortex_c::initializeState_Appear() {}
void dAcOVortex_c::executeState_Appear() {}
void dAcOVortex_c::finalizeState_Appear() {}
