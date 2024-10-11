#include <d/a/obj/d_a_obj_tower_bomb.h>

SPECIAL_ACTOR_PROFILE(OBJ_TOWER_BOMB, dAcOTowerBomb_c, fProfile::OBJ_TOWER_BOMB, 0x247, 0, 6);

STATE_DEFINE(dAcOTowerBomb_c, Wait);
STATE_DEFINE(dAcOTowerBomb_c, Bomb);
STATE_DEFINE(dAcOTowerBomb_c, Broken);

void dAcOTowerBomb_c::initializeState_Wait() {}
void dAcOTowerBomb_c::executeState_Wait() {}
void dAcOTowerBomb_c::finalizeState_Wait() {}
void dAcOTowerBomb_c::initializeState_Bomb() {}
void dAcOTowerBomb_c::executeState_Bomb() {}
void dAcOTowerBomb_c::finalizeState_Bomb() {}
void dAcOTowerBomb_c::initializeState_Broken() {}
void dAcOTowerBomb_c::executeState_Broken() {}
void dAcOTowerBomb_c::finalizeState_Broken() {}
