#include <d/a/obj/d_a_obj_seat_sword.h>

SPECIAL_ACTOR_PROFILE(OBJ_SEAT_SWORD, dAcOSeatSword_c, fProfile::OBJ_SEAT_SWORD, 0X249, 0, 3);

STATE_DEFINE(dAcOSeatSword_c, Wait);
STATE_DEFINE(dAcOSeatSword_c, Get);

void dAcOSeatSword_c::initializeState_Wait() {}
void dAcOSeatSword_c::executeState_Wait() {}
void dAcOSeatSword_c::finalizeState_Wait() {}
void dAcOSeatSword_c::initializeState_Get() {}
void dAcOSeatSword_c::executeState_Get() {}
void dAcOSeatSword_c::finalizeState_Get() {}
