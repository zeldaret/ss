#include <d/a/obj/d_a_obj_time_boat.h>

SPECIAL_ACTOR_PROFILE(OBJ_TIME_BOAT, dAcOTimeBoat_c, fProfile::OBJ_TIME_BOAT, 0x19B, 0, 2);

STATE_DEFINE(dAcOTimeBoat_c, Wait);
STATE_DEFINE(dAcOTimeBoat_c, StartTimeSlip);
STATE_DEFINE(dAcOTimeBoat_c, TSWait);
STATE_DEFINE(dAcOTimeBoat_c, TSRide);

void dAcOTimeBoat_c::initializeState_Wait() {}
void dAcOTimeBoat_c::executeState_Wait() {}
void dAcOTimeBoat_c::finalizeState_Wait() {}
void dAcOTimeBoat_c::initializeState_StartTimeSlip() {}
void dAcOTimeBoat_c::executeState_StartTimeSlip() {}
void dAcOTimeBoat_c::finalizeState_StartTimeSlip() {}
void dAcOTimeBoat_c::initializeState_TSWait() {}
void dAcOTimeBoat_c::executeState_TSWait() {}
void dAcOTimeBoat_c::finalizeState_TSWait() {}
void dAcOTimeBoat_c::initializeState_TSRide() {}
void dAcOTimeBoat_c::executeState_TSRide() {}
void dAcOTimeBoat_c::finalizeState_TSRide() {}
