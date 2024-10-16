#include "d/a/obj/d_a_obj_time_block.h"

SPECIAL_ACTOR_PROFILE(OBJ_TIME_BLOCK, dAcOTimeBlock_c, fProfile::OBJ_TIME_BLOCK, 0x1A7, 0, 4103);

STATE_DEFINE(dAcOTimeBlock_c, Wait);
STATE_DEFINE(dAcOTimeBlock_c, TimeSlip);

void dAcOTimeBlock_c::initializeState_Wait() {}
void dAcOTimeBlock_c::executeState_Wait() {}
void dAcOTimeBlock_c::finalizeState_Wait() {}
void dAcOTimeBlock_c::initializeState_TimeSlip() {}
void dAcOTimeBlock_c::executeState_TimeSlip() {}
void dAcOTimeBlock_c::finalizeState_TimeSlip() {}
