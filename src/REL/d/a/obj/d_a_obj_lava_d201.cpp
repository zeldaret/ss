#include "d/a/obj/d_a_obj_lava_d201.h"

SPECIAL_ACTOR_PROFILE(OBJ_LAVA_D201, dAcOLavaD201_c, fProfile::OBJ_LAVA_D201, 0x254, 0, 6);

STATE_DEFINE(dAcOLavaD201_c, Wait);
STATE_DEFINE(dAcOLavaD201_c, WaitOff);
STATE_DEFINE(dAcOLavaD201_c, LavaIn);
STATE_DEFINE(dAcOLavaD201_c, LavaOut);

void dAcOLavaD201_c::initializeState_Wait() {}
void dAcOLavaD201_c::executeState_Wait() {}
void dAcOLavaD201_c::finalizeState_Wait() {}
void dAcOLavaD201_c::initializeState_WaitOff() {}
void dAcOLavaD201_c::executeState_WaitOff() {}
void dAcOLavaD201_c::finalizeState_WaitOff() {}
void dAcOLavaD201_c::initializeState_LavaIn() {}
void dAcOLavaD201_c::executeState_LavaIn() {}
void dAcOLavaD201_c::finalizeState_LavaIn() {}
void dAcOLavaD201_c::initializeState_LavaOut() {}
void dAcOLavaD201_c::executeState_LavaOut() {}
void dAcOLavaD201_c::finalizeState_LavaOut() {}
