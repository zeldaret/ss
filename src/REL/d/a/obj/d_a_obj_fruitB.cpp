#include <d/a/obj/d_a_obj_fruitB.h>

SPECIAL_ACTOR_PROFILE(OBJ_FRUIT_B, dAcOFruitB_c, fProfile::OBJ_FRUIT_B, 0X153, 0, 3);

STATE_DEFINE(dAcOFruitB_c, Stick);
STATE_DEFINE(dAcOFruitB_c, Wait);
STATE_DEFINE(dAcOFruitB_c, DummyEvent);

void dAcOFruitB_c::initializeState_Stick() {}
void dAcOFruitB_c::executeState_Stick() {}
void dAcOFruitB_c::finalizeState_Stick() {}
void dAcOFruitB_c::initializeState_Wait() {}
void dAcOFruitB_c::executeState_Wait() {}
void dAcOFruitB_c::finalizeState_Wait() {}
void dAcOFruitB_c::initializeState_DummyEvent() {}
void dAcOFruitB_c::executeState_DummyEvent() {}
void dAcOFruitB_c::finalizeState_DummyEvent() {}
