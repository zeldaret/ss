#include <d/a/obj/d_a_obj_chest.h>

SPECIAL_ACTOR_PROFILE(OBJ_CHEST, dAcOChest_c, fProfile::OBJ_CHEST, 0X13E, 0, 3);

STATE_DEFINE(dAcOChest_c, Wait);
STATE_DEFINE(dAcOChest_c, OpenEvent);
STATE_DEFINE(dAcOChest_c, FailEvent);
STATE_DEFINE(dAcOChest_c, OrderOpenEventAfter);

void dAcOChest_c::initializeState_Wait() {}
void dAcOChest_c::executeState_Wait() {}
void dAcOChest_c::finalizeState_Wait() {}
void dAcOChest_c::initializeState_OpenEvent() {}
void dAcOChest_c::executeState_OpenEvent() {}
void dAcOChest_c::finalizeState_OpenEvent() {}
void dAcOChest_c::initializeState_FailEvent() {}
void dAcOChest_c::executeState_FailEvent() {}
void dAcOChest_c::finalizeState_FailEvent() {}
void dAcOChest_c::initializeState_OrderOpenEventAfter() {}
void dAcOChest_c::executeState_OrderOpenEventAfter() {}
void dAcOChest_c::finalizeState_OrderOpenEventAfter() {}
