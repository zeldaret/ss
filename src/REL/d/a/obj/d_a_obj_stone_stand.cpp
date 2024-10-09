#include <d/a/obj/d_a_obj_stone_stand.h>

SPECIAL_ACTOR_PROFILE(OBJ_STONE_STAND, dAcOStoneStand_c, fProfile::OBJ_STONE_STAND, 0x24B, 0, 3);

STATE_DEFINE(dAcOStoneStand_c, Wait);
STATE_DEFINE(dAcOStoneStand_c, Shake);
STATE_DEFINE(dAcOStoneStand_c, OnSwitch);

void dAcOStoneStand_c::initializeState_Wait() {}
void dAcOStoneStand_c::executeState_Wait() {}
void dAcOStoneStand_c::finalizeState_Wait() {}
void dAcOStoneStand_c::initializeState_Shake() {}
void dAcOStoneStand_c::executeState_Shake() {}
void dAcOStoneStand_c::finalizeState_Shake() {}
void dAcOStoneStand_c::initializeState_OnSwitch() {}
void dAcOStoneStand_c::executeState_OnSwitch() {}
void dAcOStoneStand_c::finalizeState_OnSwitch() {}
