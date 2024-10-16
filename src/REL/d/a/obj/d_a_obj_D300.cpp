#include "d/a/obj/d_a_obj_D300.h"

SPECIAL_ACTOR_PROFILE(OBJ_D300, dAcOD300_c, fProfile::OBJ_D300, 0x248, 0, 5);

STATE_DEFINE(dAcOD300_c, Wait);
STATE_DEFINE(dAcOD300_c, Appear);
STATE_DEFINE(dAcOD300_c, End);

void dAcOD300_c::initializeState_Wait() {}
void dAcOD300_c::executeState_Wait() {}
void dAcOD300_c::finalizeState_Wait() {}
void dAcOD300_c::initializeState_Appear() {}
void dAcOD300_c::executeState_Appear() {}
void dAcOD300_c::finalizeState_Appear() {}
void dAcOD300_c::initializeState_End() {}
void dAcOD300_c::executeState_End() {}
void dAcOD300_c::finalizeState_End() {}
