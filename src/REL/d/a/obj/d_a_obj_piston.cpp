#include "d/a/obj/d_a_obj_piston.h"

SPECIAL_ACTOR_PROFILE(OBJ_PISTON, dAcOPiston_c, fProfile::OBJ_PISTON, 0x172, 0, 3);

STATE_DEFINE(dAcOPiston_c, WaitDelay);
STATE_DEFINE(dAcOPiston_c, OnAnim);
STATE_DEFINE(dAcOPiston_c, Wait);
STATE_DEFINE(dAcOPiston_c, Move);
STATE_DEFINE(dAcOPiston_c, Event);
STATE_DEFINE(dAcOPiston_c, End);

void dAcOPiston_c::initializeState_WaitDelay() {}
void dAcOPiston_c::executeState_WaitDelay() {}
void dAcOPiston_c::finalizeState_WaitDelay() {}
void dAcOPiston_c::initializeState_OnAnim() {}
void dAcOPiston_c::executeState_OnAnim() {}
void dAcOPiston_c::finalizeState_OnAnim() {}
void dAcOPiston_c::initializeState_Wait() {}
void dAcOPiston_c::executeState_Wait() {}
void dAcOPiston_c::finalizeState_Wait() {}
void dAcOPiston_c::initializeState_Move() {}
void dAcOPiston_c::executeState_Move() {}
void dAcOPiston_c::finalizeState_Move() {}
void dAcOPiston_c::initializeState_Event() {}
void dAcOPiston_c::executeState_Event() {}
void dAcOPiston_c::finalizeState_Event() {}
void dAcOPiston_c::initializeState_End() {}
void dAcOPiston_c::executeState_End() {}
void dAcOPiston_c::finalizeState_End() {}
