#include "d/a/obj/d_a_obj_fruit.h"

SPECIAL_ACTOR_PROFILE(OBJ_FRUIT, dAcOFruit_c, fProfile::OBJ_FRUIT, 0x164, 0, 3);

STATE_DEFINE(dAcOFruit_c, NormalWait);
STATE_DEFINE(dAcOFruit_c, StickWait);
STATE_DEFINE(dAcOFruit_c, GrabCarry);
STATE_DEFINE(dAcOFruit_c, SwordCarry);
STATE_DEFINE(dAcOFruit_c, BoomerangCarry);
STATE_DEFINE(dAcOFruit_c, WhipCarry);
STATE_DEFINE(dAcOFruit_c, Dead);

void dAcOFruit_c::initializeState_NormalWait() {}
void dAcOFruit_c::executeState_NormalWait() {}
void dAcOFruit_c::finalizeState_NormalWait() {}
void dAcOFruit_c::initializeState_StickWait() {}
void dAcOFruit_c::executeState_StickWait() {}
void dAcOFruit_c::finalizeState_StickWait() {}
void dAcOFruit_c::initializeState_GrabCarry() {}
void dAcOFruit_c::executeState_GrabCarry() {}
void dAcOFruit_c::finalizeState_GrabCarry() {}
void dAcOFruit_c::initializeState_SwordCarry() {}
void dAcOFruit_c::executeState_SwordCarry() {}
void dAcOFruit_c::finalizeState_SwordCarry() {}
void dAcOFruit_c::initializeState_BoomerangCarry() {}
void dAcOFruit_c::executeState_BoomerangCarry() {}
void dAcOFruit_c::finalizeState_BoomerangCarry() {}
void dAcOFruit_c::initializeState_WhipCarry() {}
void dAcOFruit_c::executeState_WhipCarry() {}
void dAcOFruit_c::finalizeState_WhipCarry() {}
void dAcOFruit_c::initializeState_Dead() {}
void dAcOFruit_c::executeState_Dead() {}
void dAcOFruit_c::finalizeState_Dead() {}
