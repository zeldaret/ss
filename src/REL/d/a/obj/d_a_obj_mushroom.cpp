#include <d/a/obj/d_a_obj_mushroom.h>

SPECIAL_ACTOR_PROFILE(OBJ_MUSHROOM, dAcOMushRoom_c, fProfile::OBJ_MUSHROOM, 0X141, 0, 7);

STATE_DEFINE(dAcOMushRoom_c, Wait);
STATE_DEFINE(dAcOMushRoom_c, Init);

void dAcOMushRoom_c::initializeState_Wait() {}
void dAcOMushRoom_c::executeState_Wait() {}
void dAcOMushRoom_c::finalizeState_Wait() {}
void dAcOMushRoom_c::initializeState_Init() {}
void dAcOMushRoom_c::executeState_Init() {}
void dAcOMushRoom_c::finalizeState_Init() {}
