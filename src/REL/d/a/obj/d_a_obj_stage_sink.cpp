#include <d/a/obj/d_a_obj_stage_sink.h>

SPECIAL_ACTOR_PROFILE(OBJ_STAGE_SINK, dAcOstageSink_c, fProfile::OBJ_STAGE_SINK, 0X1E5, 0, 0);

STATE_DEFINE(dAcOstageSink_c, Wait);
STATE_DEFINE(dAcOstageSink_c, Move);

void dAcOstageSink_c::initializeState_Wait() {}
void dAcOstageSink_c::executeState_Wait() {}
void dAcOstageSink_c::finalizeState_Wait() {}
void dAcOstageSink_c::initializeState_Move() {}
void dAcOstageSink_c::executeState_Move() {}
void dAcOstageSink_c::finalizeState_Move() {}
