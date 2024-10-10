#include <d/a/obj/d_a_obj_time_stone.h>

SPECIAL_ACTOR_PROFILE(OBJ_TIME_STONE, dAcOTimeStone_c, fProfile::OBJ_TIME_STONE, 0x19A, 0, 6);

STATE_DEFINE(dAcOTimeStone_c, Wait);
STATE_DEFINE(dAcOTimeStone_c, TimeSlip);
STATE_DEFINE(dAcOTimeStone_c, TimeSlipEnd);

void dAcOTimeStone_c::initializeState_Wait() {}
void dAcOTimeStone_c::executeState_Wait() {}
void dAcOTimeStone_c::finalizeState_Wait() {}
void dAcOTimeStone_c::initializeState_TimeSlip() {}
void dAcOTimeStone_c::executeState_TimeSlip() {}
void dAcOTimeStone_c::finalizeState_TimeSlip() {}
void dAcOTimeStone_c::initializeState_TimeSlipEnd() {}
void dAcOTimeStone_c::executeState_TimeSlipEnd() {}
void dAcOTimeStone_c::finalizeState_TimeSlipEnd() {}
