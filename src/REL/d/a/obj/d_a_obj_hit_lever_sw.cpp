#include <d/a/obj/d_a_obj_hit_lever_sw.h>

SPECIAL_ACTOR_PROFILE(OBJ_HIT_LEVER_SW, dAcOhitLeverSw_c, fProfile::OBJ_HIT_LEVER_SW, 0X1C0, 0, 6);

STATE_DEFINE(dAcOhitLeverSw_c, Right);
STATE_DEFINE(dAcOhitLeverSw_c, Left);
STATE_DEFINE(dAcOhitLeverSw_c, Move);

void dAcOhitLeverSw_c::initializeState_Right() {}
void dAcOhitLeverSw_c::executeState_Right() {}
void dAcOhitLeverSw_c::finalizeState_Right() {}
void dAcOhitLeverSw_c::initializeState_Left() {}
void dAcOhitLeverSw_c::executeState_Left() {}
void dAcOhitLeverSw_c::finalizeState_Left() {}
void dAcOhitLeverSw_c::initializeState_Move() {}
void dAcOhitLeverSw_c::executeState_Move() {}
void dAcOhitLeverSw_c::finalizeState_Move() {}
