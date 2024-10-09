#include <d/a/obj/d_a_obj_sink_floor_f.h>

SPECIAL_ACTOR_PROFILE(OBJ_SINK_FLOOR_F, dAcOSinkFloorF_c, fProfile::OBJ_SINK_FLOOR_F, 0x22E, 0, 6);

STATE_DEFINE(dAcOSinkFloorF_c, Wait);
STATE_DEFINE(dAcOSinkFloorF_c, Ride);
STATE_DEFINE(dAcOSinkFloorF_c, Sink);

void dAcOSinkFloorF_c::initializeState_Wait() {}
void dAcOSinkFloorF_c::executeState_Wait() {}
void dAcOSinkFloorF_c::finalizeState_Wait() {}
void dAcOSinkFloorF_c::initializeState_Ride() {}
void dAcOSinkFloorF_c::executeState_Ride() {}
void dAcOSinkFloorF_c::finalizeState_Ride() {}
void dAcOSinkFloorF_c::initializeState_Sink() {}
void dAcOSinkFloorF_c::executeState_Sink() {}
void dAcOSinkFloorF_c::finalizeState_Sink() {}
