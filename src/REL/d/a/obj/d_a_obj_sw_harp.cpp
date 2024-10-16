#include "d/a/obj/d_a_obj_sw_harp.h"

SPECIAL_ACTOR_PROFILE(OBJ_SW_HARP, dAcOSwHarp_c, fProfile::OBJ_SW_HARP, 0x1A3, 0, 7);

STATE_DEFINE(dAcOSwHarp_c, Wait);
STATE_DEFINE(dAcOSwHarp_c, OnSwitch);
STATE_DEFINE(dAcOSwHarp_c, EndDemo);

void dAcOSwHarp_c::initializeState_Wait() {}
void dAcOSwHarp_c::executeState_Wait() {}
void dAcOSwHarp_c::finalizeState_Wait() {}
void dAcOSwHarp_c::initializeState_OnSwitch() {}
void dAcOSwHarp_c::executeState_OnSwitch() {}
void dAcOSwHarp_c::finalizeState_OnSwitch() {}
void dAcOSwHarp_c::initializeState_EndDemo() {}
void dAcOSwHarp_c::executeState_EndDemo() {}
void dAcOSwHarp_c::finalizeState_EndDemo() {}
