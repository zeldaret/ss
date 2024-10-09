#include <d/a/obj/d_a_obj_stage_water.h>

SPECIAL_ACTOR_PROFILE(OBJ_STAGE_WATER, dAcOstageWater_c, fProfile::OBJ_STAGE_WATER, 0X1E6, 0, 4);

STATE_DEFINE(dAcOstageWater_c, Wait);
STATE_DEFINE(dAcOstageWater_c, Move);

void dAcOstageWater_c::initializeState_Wait() {}
void dAcOstageWater_c::executeState_Wait() {}
void dAcOstageWater_c::finalizeState_Wait() {}
void dAcOstageWater_c::initializeState_Move() {}
void dAcOstageWater_c::executeState_Move() {}
void dAcOstageWater_c::finalizeState_Move() {}
