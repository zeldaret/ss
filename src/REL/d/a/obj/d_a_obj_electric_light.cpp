#include <d/a/obj/d_a_obj_electric_light.h>

SPECIAL_ACTOR_PROFILE(OBJ_ELECTRIC_LIGHT, dAcOelectricLight_c, fProfile::OBJ_ELECTRIC_LIGHT, 0X1B8, 0, 2);

STATE_DEFINE(dAcOelectricLight_c, Wait);

void dAcOelectricLight_c::initializeState_Wait() {}
void dAcOelectricLight_c::executeState_Wait() {}
void dAcOelectricLight_c::finalizeState_Wait() {}
