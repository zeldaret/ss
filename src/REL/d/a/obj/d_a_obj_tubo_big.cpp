#include <d/a/obj/d_a_obj_tubo_big.h>

SPECIAL_ACTOR_PROFILE(OBJ_TUBO_BIG, dAcOTuboBig_c, fProfile::OBJ_TUBO_BIG, 0X195, 0, 3);

STATE_DEFINE(dAcOTuboBig_c, Wait);
STATE_DEFINE(dAcOTuboBig_c, Grab);
STATE_DEFINE(dAcOTuboBig_c, Water);

void dAcOTuboBig_c::initializeState_Wait() {}
void dAcOTuboBig_c::executeState_Wait() {}
void dAcOTuboBig_c::finalizeState_Wait() {}
void dAcOTuboBig_c::initializeState_Grab() {}
void dAcOTuboBig_c::executeState_Grab() {}
void dAcOTuboBig_c::finalizeState_Grab() {}
void dAcOTuboBig_c::initializeState_Water() {}
void dAcOTuboBig_c::executeState_Water() {}
void dAcOTuboBig_c::finalizeState_Water() {}
