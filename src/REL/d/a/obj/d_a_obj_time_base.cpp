#include <d/a/obj/d_a_obj_time_base.h>

SPECIAL_ACTOR_PROFILE(OBJ_TIME_BASE, dAcOTimeBase_c, fProfile::OBJ_TIME_BASE, 0X19E, 0, 3);

STATE_DEFINE(dAcOTimeBase_c, Wait);
STATE_DEFINE(dAcOTimeBase_c, Event);

void dAcOTimeBase_c::initializeState_Wait() {}
void dAcOTimeBase_c::executeState_Wait() {}
void dAcOTimeBase_c::finalizeState_Wait() {}
void dAcOTimeBase_c::initializeState_Event() {}
void dAcOTimeBase_c::executeState_Event() {}
void dAcOTimeBase_c::finalizeState_Event() {}
