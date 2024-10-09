#include <d/a/obj/d_a_obj_step_time_slip.h>

SPECIAL_ACTOR_PROFILE(OBJ_STEP_TIME_SLIP, dAcOStepTimeSlip_c, fProfile::OBJ_STEP_TIME_SLIP, 0X19D, 0, 4102);

STATE_DEFINE(dAcOStepTimeSlip_c, Wait);

void dAcOStepTimeSlip_c::initializeState_Wait() {}
void dAcOStepTimeSlip_c::executeState_Wait() {}
void dAcOStepTimeSlip_c::finalizeState_Wait() {}
