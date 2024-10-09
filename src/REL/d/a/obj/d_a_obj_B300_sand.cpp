#include <d/a/obj/d_a_obj_B300_sand.h>

SPECIAL_ACTOR_PROFILE(OBJ_B300_SAND, dAcOB300Sand_c, fProfile::OBJ_B300_SAND, 0X1AF, 0, 0);

STATE_DEFINE(dAcOB300Sand_c, Wait);
STATE_DEFINE(dAcOB300Sand_c, Clear);

void dAcOB300Sand_c::initializeState_Wait() {}
void dAcOB300Sand_c::executeState_Wait() {}
void dAcOB300Sand_c::finalizeState_Wait() {}
void dAcOB300Sand_c::initializeState_Clear() {}
void dAcOB300Sand_c::executeState_Clear() {}
void dAcOB300Sand_c::finalizeState_Clear() {}
