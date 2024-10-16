#include "d/a/obj/d_a_obj_girahim_foot.h"

SPECIAL_ACTOR_PROFILE(OBJ_GIRAHIM_FOOT, dAcOgirahimFoot_c, fProfile::OBJ_GIRAHIM_FOOT, 0x210, 0, 6);

STATE_DEFINE(dAcOgirahimFoot_c, Wait);
STATE_DEFINE(dAcOgirahimFoot_c, Appear);

void dAcOgirahimFoot_c::initializeState_Wait() {}
void dAcOgirahimFoot_c::executeState_Wait() {}
void dAcOgirahimFoot_c::finalizeState_Wait() {}
void dAcOgirahimFoot_c::initializeState_Appear() {}
void dAcOgirahimFoot_c::executeState_Appear() {}
void dAcOgirahimFoot_c::finalizeState_Appear() {}
