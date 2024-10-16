#include "d/a/obj/d_a_obj_updown_lava.h"

SPECIAL_ACTOR_PROFILE(OBJ_UPDOWN_LAVA, dAcOupdownLava_c, fProfile::OBJ_UPDOWN_LAVA, 0x1C5, 0, 0);

STATE_DEFINE(dAcOupdownLava_c, Wait);
STATE_DEFINE(dAcOupdownLava_c, UpDown);
STATE_DEFINE(dAcOupdownLava_c, Stop);
STATE_DEFINE(dAcOupdownLava_c, Up);
STATE_DEFINE(dAcOupdownLava_c, Sink);
STATE_DEFINE(dAcOupdownLava_c, Down);

void dAcOupdownLava_c::initializeState_Wait() {}
void dAcOupdownLava_c::executeState_Wait() {}
void dAcOupdownLava_c::finalizeState_Wait() {}
void dAcOupdownLava_c::initializeState_UpDown() {}
void dAcOupdownLava_c::executeState_UpDown() {}
void dAcOupdownLava_c::finalizeState_UpDown() {}
void dAcOupdownLava_c::initializeState_Stop() {}
void dAcOupdownLava_c::executeState_Stop() {}
void dAcOupdownLava_c::finalizeState_Stop() {}
void dAcOupdownLava_c::initializeState_Up() {}
void dAcOupdownLava_c::executeState_Up() {}
void dAcOupdownLava_c::finalizeState_Up() {}
void dAcOupdownLava_c::initializeState_Sink() {}
void dAcOupdownLava_c::executeState_Sink() {}
void dAcOupdownLava_c::finalizeState_Sink() {}
void dAcOupdownLava_c::initializeState_Down() {}
void dAcOupdownLava_c::executeState_Down() {}
void dAcOupdownLava_c::finalizeState_Down() {}
