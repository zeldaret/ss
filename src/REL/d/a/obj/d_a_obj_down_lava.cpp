#include <d/a/obj/d_a_obj_down_lava.h>

SPECIAL_ACTOR_PROFILE(OBJ_DOWN_LAVA, dAcOdownLava_c, fProfile::OBJ_DOWN_LAVA, 0x1FB, 0, 6);

STATE_DEFINE(dAcOdownLava_c, Wait);
STATE_DEFINE(dAcOdownLava_c, Move);
STATE_DEFINE(dAcOdownLava_c, After);

void dAcOdownLava_c::initializeState_Wait() {}
void dAcOdownLava_c::executeState_Wait() {}
void dAcOdownLava_c::finalizeState_Wait() {}
void dAcOdownLava_c::initializeState_Move() {}
void dAcOdownLava_c::executeState_Move() {}
void dAcOdownLava_c::finalizeState_Move() {}
void dAcOdownLava_c::initializeState_After() {}
void dAcOdownLava_c::executeState_After() {}
void dAcOdownLava_c::finalizeState_After() {}
