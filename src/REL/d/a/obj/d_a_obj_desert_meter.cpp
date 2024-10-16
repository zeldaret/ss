#include "d/a/obj/d_a_obj_desert_meter.h"

SPECIAL_ACTOR_PROFILE(OBJ_DESERT_METER, dAcODesertMeter_c, fProfile::OBJ_DESERT_METER, 0x1B0, 0, 7);

STATE_DEFINE(dAcODesertMeter_c, Wait);
STATE_DEFINE(dAcODesertMeter_c, OnSwitch);

void dAcODesertMeter_c::initializeState_Wait() {}
void dAcODesertMeter_c::executeState_Wait() {}
void dAcODesertMeter_c::finalizeState_Wait() {}
void dAcODesertMeter_c::initializeState_OnSwitch() {}
void dAcODesertMeter_c::executeState_OnSwitch() {}
void dAcODesertMeter_c::finalizeState_OnSwitch() {}
