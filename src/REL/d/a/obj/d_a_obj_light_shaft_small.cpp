#include <d/a/obj/d_a_obj_light_shaft_small.h>

SPECIAL_ACTOR_PROFILE(OBJ_LIGHT_SHAFT_SMALL, dAcOLightShaftSmall_c, fProfile::OBJ_LIGHT_SHAFT_SMALL, 0X198, 0, 4);

STATE_DEFINE(dAcOLightShaftSmall_c, Wait);
STATE_DEFINE(dAcOLightShaftSmall_c, Inside);

void dAcOLightShaftSmall_c::initializeState_Wait() {}
void dAcOLightShaftSmall_c::executeState_Wait() {}
void dAcOLightShaftSmall_c::finalizeState_Wait() {}
void dAcOLightShaftSmall_c::initializeState_Inside() {}
void dAcOLightShaftSmall_c::executeState_Inside() {}
void dAcOLightShaftSmall_c::finalizeState_Inside() {}
