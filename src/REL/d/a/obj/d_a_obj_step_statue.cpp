#include <d/a/obj/d_a_obj_step_statue.h>

SPECIAL_ACTOR_PROFILE(OBJ_STEP_STATUE, dAcOStepStatue_c, fProfile::OBJ_STEP_STATUE, 0X17C, 0, 3);

STATE_DEFINE(dAcOStepStatue_c, Wait);
STATE_DEFINE(dAcOStepStatue_c, CarryBomb);
STATE_DEFINE(dAcOStepStatue_c, BreakDown);
STATE_DEFINE(dAcOStepStatue_c, SandSink);
STATE_DEFINE(dAcOStepStatue_c, End);

void dAcOStepStatue_c::initializeState_Wait() {}
void dAcOStepStatue_c::executeState_Wait() {}
void dAcOStepStatue_c::finalizeState_Wait() {}
void dAcOStepStatue_c::initializeState_CarryBomb() {}
void dAcOStepStatue_c::executeState_CarryBomb() {}
void dAcOStepStatue_c::finalizeState_CarryBomb() {}
void dAcOStepStatue_c::initializeState_BreakDown() {}
void dAcOStepStatue_c::executeState_BreakDown() {}
void dAcOStepStatue_c::finalizeState_BreakDown() {}
void dAcOStepStatue_c::initializeState_SandSink() {}
void dAcOStepStatue_c::executeState_SandSink() {}
void dAcOStepStatue_c::finalizeState_SandSink() {}
void dAcOStepStatue_c::initializeState_End() {}
void dAcOStepStatue_c::executeState_End() {}
void dAcOStepStatue_c::finalizeState_End() {}
