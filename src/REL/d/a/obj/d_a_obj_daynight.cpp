#include "d/a/obj/d_a_obj_daynight.h"

SPECIAL_ACTOR_PROFILE(OBJ_DAYNIGHT, dAcODayNight_c, fProfile::OBJ_DAYNIGHT, 0x259, 0, 0);

STATE_DEFINE(dAcODayNight_c, Wait);

void dAcODayNight_c::initializeState_Wait() {}
void dAcODayNight_c::executeState_Wait() {}
void dAcODayNight_c::finalizeState_Wait() {}
