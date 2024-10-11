#include <d/a/obj/d_a_obj_carry_stone.h>

SPECIAL_ACTOR_PROFILE(OBJ_CARRY_STONE, dAcOcarryStone_c, fProfile::OBJ_CARRY_STONE, 0x12A, 0, 2);

STATE_DEFINE(dAcOcarryStone_c, Wait);
STATE_DEFINE(dAcOcarryStone_c, GrabCarry);
STATE_DEFINE(dAcOcarryStone_c, Water);
STATE_DEFINE(dAcOcarryStone_c, Slope);
STATE_DEFINE(dAcOcarryStone_c, SlopeWait);
STATE_DEFINE(dAcOcarryStone_c, MoveVacuum);
STATE_DEFINE(dAcOcarryStone_c, ShakeWind);
STATE_DEFINE(dAcOcarryStone_c, MoveMoleGlove);
STATE_DEFINE(dAcOcarryStone_c, EggEnterSwitch);

void dAcOcarryStone_c::initializeState_Wait() {}
void dAcOcarryStone_c::executeState_Wait() {}
void dAcOcarryStone_c::finalizeState_Wait() {}
void dAcOcarryStone_c::initializeState_GrabCarry() {}
void dAcOcarryStone_c::executeState_GrabCarry() {}
void dAcOcarryStone_c::finalizeState_GrabCarry() {}
void dAcOcarryStone_c::initializeState_Water() {}
void dAcOcarryStone_c::executeState_Water() {}
void dAcOcarryStone_c::finalizeState_Water() {}
void dAcOcarryStone_c::initializeState_Slope() {}
void dAcOcarryStone_c::executeState_Slope() {}
void dAcOcarryStone_c::finalizeState_Slope() {}
void dAcOcarryStone_c::initializeState_SlopeWait() {}
void dAcOcarryStone_c::executeState_SlopeWait() {}
void dAcOcarryStone_c::finalizeState_SlopeWait() {}
void dAcOcarryStone_c::initializeState_MoveVacuum() {}
void dAcOcarryStone_c::executeState_MoveVacuum() {}
void dAcOcarryStone_c::finalizeState_MoveVacuum() {}
void dAcOcarryStone_c::initializeState_ShakeWind() {}
void dAcOcarryStone_c::executeState_ShakeWind() {}
void dAcOcarryStone_c::finalizeState_ShakeWind() {}
void dAcOcarryStone_c::initializeState_MoveMoleGlove() {}
void dAcOcarryStone_c::executeState_MoveMoleGlove() {}
void dAcOcarryStone_c::finalizeState_MoveMoleGlove() {}
void dAcOcarryStone_c::initializeState_EggEnterSwitch() {}
void dAcOcarryStone_c::executeState_EggEnterSwitch() {}
void dAcOcarryStone_c::finalizeState_EggEnterSwitch() {}
