#include <d/a/obj/d_a_obj_treasure_island_b.h>

SPECIAL_ACTOR_PROFILE(OBJ_TREASURE_ISLAND_B, dAcOtreasureIslandB_c, fProfile::OBJ_TREASURE_ISLAND_B, 0X206, 0, 2);

STATE_DEFINE(dAcOtreasureIslandB_c, Wait);
STATE_DEFINE(dAcOtreasureIslandB_c, CoverBreak);

void dAcOtreasureIslandB_c::initializeState_Wait() {}
void dAcOtreasureIslandB_c::executeState_Wait() {}
void dAcOtreasureIslandB_c::finalizeState_Wait() {}
void dAcOtreasureIslandB_c::initializeState_CoverBreak() {}
void dAcOtreasureIslandB_c::executeState_CoverBreak() {}
void dAcOtreasureIslandB_c::finalizeState_CoverBreak() {}
