#include "d/a/obj/d_a_obj_bulb_switch.h"

SPECIAL_ACTOR_PROFILE(OBJ_BULB_SWITCH, dAcObulbSwitch_c, fProfile::OBJ_BULB_SWITCH, 0x1BC, 0, 2);

STATE_DEFINE(dAcObulbSwitch_c, Close);
STATE_DEFINE(dAcObulbSwitch_c, Open);
STATE_DEFINE(dAcObulbSwitch_c, Move);

void dAcObulbSwitch_c::initializeState_Close() {}
void dAcObulbSwitch_c::executeState_Close() {}
void dAcObulbSwitch_c::finalizeState_Close() {}
void dAcObulbSwitch_c::initializeState_Open() {}
void dAcObulbSwitch_c::executeState_Open() {}
void dAcObulbSwitch_c::finalizeState_Open() {}
void dAcObulbSwitch_c::initializeState_Move() {}
void dAcObulbSwitch_c::executeState_Move() {}
void dAcObulbSwitch_c::finalizeState_Move() {}
