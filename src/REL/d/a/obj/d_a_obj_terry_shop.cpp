#include "d/a/obj/d_a_obj_terry_shop.h"

SPECIAL_ACTOR_PROFILE(OBJ_TERRY_SHOP, dAcOterryShop_c, fProfile::OBJ_TERRY_SHOP, 0x26E, 0, 2);

STATE_DEFINE(dAcOterryShop_c, Move);
STATE_DEFINE(dAcOterryShop_c, Stop);
STATE_DEFINE(dAcOterryShop_c, Enter);
STATE_DEFINE(dAcOterryShop_c, TakeOff);

void dAcOterryShop_c::initializeState_Move() {}
void dAcOterryShop_c::executeState_Move() {}
void dAcOterryShop_c::finalizeState_Move() {}
void dAcOterryShop_c::initializeState_Stop() {}
void dAcOterryShop_c::executeState_Stop() {}
void dAcOterryShop_c::finalizeState_Stop() {}
void dAcOterryShop_c::initializeState_Enter() {}
void dAcOterryShop_c::executeState_Enter() {}
void dAcOterryShop_c::finalizeState_Enter() {}
void dAcOterryShop_c::initializeState_TakeOff() {}
void dAcOterryShop_c::executeState_TakeOff() {}
void dAcOterryShop_c::finalizeState_TakeOff() {}
