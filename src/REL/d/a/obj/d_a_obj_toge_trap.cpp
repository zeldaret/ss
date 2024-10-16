#include "d/a/obj/d_a_obj_toge_trap.h"

SPECIAL_ACTOR_PROFILE(OBJ_TOGE_TRAP, dAcOtogeTrap_c, fProfile::OBJ_TOGE_TRAP, 0x133, 0, 4098);

STATE_DEFINE(dAcOtogeTrap_c, Wait);
STATE_DEFINE(dAcOtogeTrap_c, Move);

void dAcOtogeTrap_c::initializeState_Wait() {}
void dAcOtogeTrap_c::executeState_Wait() {}
void dAcOtogeTrap_c::finalizeState_Wait() {}
void dAcOtogeTrap_c::initializeState_Move() {}
void dAcOtogeTrap_c::executeState_Move() {}
void dAcOtogeTrap_c::finalizeState_Move() {}
