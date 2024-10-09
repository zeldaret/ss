#include <d/a/obj/d_a_obj_tower_hand_D101.h>

SPECIAL_ACTOR_PROFILE(OBJ_TOWER_HAND_D101, dAcOTowerHandD101_c, fProfile::OBJ_TOWER_HAND_D101, 0x180, 0, 6);

STATE_DEFINE(dAcOTowerHandD101_c, RemainOpen);
STATE_DEFINE(dAcOTowerHandD101_c, Close);
STATE_DEFINE(dAcOTowerHandD101_c, RemainClosed);
STATE_DEFINE(dAcOTowerHandD101_c, Open);
STATE_DEFINE(dAcOTowerHandD101_c, Hold);

void dAcOTowerHandD101_c::initializeState_RemainOpen() {}
void dAcOTowerHandD101_c::executeState_RemainOpen() {}
void dAcOTowerHandD101_c::finalizeState_RemainOpen() {}
void dAcOTowerHandD101_c::initializeState_Close() {}
void dAcOTowerHandD101_c::executeState_Close() {}
void dAcOTowerHandD101_c::finalizeState_Close() {}
void dAcOTowerHandD101_c::initializeState_RemainClosed() {}
void dAcOTowerHandD101_c::executeState_RemainClosed() {}
void dAcOTowerHandD101_c::finalizeState_RemainClosed() {}
void dAcOTowerHandD101_c::initializeState_Open() {}
void dAcOTowerHandD101_c::executeState_Open() {}
void dAcOTowerHandD101_c::finalizeState_Open() {}
void dAcOTowerHandD101_c::initializeState_Hold() {}
void dAcOTowerHandD101_c::executeState_Hold() {}
void dAcOTowerHandD101_c::finalizeState_Hold() {}
