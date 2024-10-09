#include <d/a/obj/d_a_obj_tumble_weed.h>

SPECIAL_ACTOR_PROFILE(OBJ_TUMBLE_WEED, dAcOTumbleWeed_c, fProfile::OBJ_TUMBLE_WEED, 0x243, 0, 2);

STATE_DEFINE(dAcOTumbleWeed_c, Wait);
STATE_DEFINE(dAcOTumbleWeed_c, Slope);

void dAcOTumbleWeed_c::initializeState_Wait() {}
void dAcOTumbleWeed_c::executeState_Wait() {}
void dAcOTumbleWeed_c::finalizeState_Wait() {}
void dAcOTumbleWeed_c::initializeState_Slope() {}
void dAcOTumbleWeed_c::executeState_Slope() {}
void dAcOTumbleWeed_c::finalizeState_Slope() {}
