#include <d/a/obj/d_a_obj_F302_light.h>

SPECIAL_ACTOR_PROFILE(OBJ_F302_LIGHT, dAcOF302Light_c, fProfile::OBJ_F302_LIGHT, 0X1B2, 0, 3);

STATE_DEFINE(dAcOF302Light_c, Wait);
STATE_DEFINE(dAcOF302Light_c, SwitchOn);

void dAcOF302Light_c::initializeState_Wait() {}
void dAcOF302Light_c::executeState_Wait() {}
void dAcOF302Light_c::finalizeState_Wait() {}
void dAcOF302Light_c::initializeState_SwitchOn() {}
void dAcOF302Light_c::executeState_SwitchOn() {}
void dAcOF302Light_c::finalizeState_SwitchOn() {}
