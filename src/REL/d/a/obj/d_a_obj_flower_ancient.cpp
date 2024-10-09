#include <d/a/obj/d_a_obj_flower_ancient.h>

SPECIAL_ACTOR_PROFILE(OBJ_FLOWER_ANCIENT, dAcOFlowerAncient_c, fProfile::OBJ_FLOWER_ANCIENT, 0x245, 0, 4098);

STATE_DEFINE(dAcOFlowerAncient_c, Wait);
STATE_DEFINE(dAcOFlowerAncient_c, TimeSlip);
STATE_DEFINE(dAcOFlowerAncient_c, Break);
STATE_DEFINE(dAcOFlowerAncient_c, WhipCarry);

void dAcOFlowerAncient_c::initializeState_Wait() {}
void dAcOFlowerAncient_c::executeState_Wait() {}
void dAcOFlowerAncient_c::finalizeState_Wait() {}
void dAcOFlowerAncient_c::initializeState_TimeSlip() {}
void dAcOFlowerAncient_c::executeState_TimeSlip() {}
void dAcOFlowerAncient_c::finalizeState_TimeSlip() {}
void dAcOFlowerAncient_c::initializeState_Break() {}
void dAcOFlowerAncient_c::executeState_Break() {}
void dAcOFlowerAncient_c::finalizeState_Break() {}
void dAcOFlowerAncient_c::initializeState_WhipCarry() {}
void dAcOFlowerAncient_c::executeState_WhipCarry() {}
void dAcOFlowerAncient_c::finalizeState_WhipCarry() {}
