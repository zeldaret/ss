#include <d/a/obj/d_a_obj_tubo.h>

SPECIAL_ACTOR_PROFILE(OBJ_TUBO, dAcOtubo_c, fProfile::OBJ_TUBO, 0X194, 0, 130);

STATE_DEFINE(dAcOtubo_c, Wait);
STATE_DEFINE(dAcOtubo_c, Grab);
STATE_DEFINE(dAcOtubo_c, Put);
STATE_DEFINE(dAcOtubo_c, Slope);
STATE_DEFINE(dAcOtubo_c, Rebirth);

void dAcOtubo_c::initializeState_Wait() {}
void dAcOtubo_c::executeState_Wait() {}
void dAcOtubo_c::finalizeState_Wait() {}
void dAcOtubo_c::initializeState_Grab() {}
void dAcOtubo_c::executeState_Grab() {}
void dAcOtubo_c::finalizeState_Grab() {}
void dAcOtubo_c::initializeState_Put() {}
void dAcOtubo_c::executeState_Put() {}
void dAcOtubo_c::finalizeState_Put() {}
void dAcOtubo_c::initializeState_Slope() {}
void dAcOtubo_c::executeState_Slope() {}
void dAcOtubo_c::finalizeState_Slope() {}
void dAcOtubo_c::initializeState_Rebirth() {}
void dAcOtubo_c::executeState_Rebirth() {}
void dAcOtubo_c::finalizeState_Rebirth() {}
