#include "d/a/obj/d_a_obj_rock_dragon.h"

SPECIAL_ACTOR_PROFILE(OBJ_ROCK_DRAGON, dAcORockDragon_c, fProfile::OBJ_ROCK_DRAGON, 0x147, 0, 3);

STATE_DEFINE(dAcORockDragon_c, Upper_Wait);
STATE_DEFINE(dAcORockDragon_c, Upper_Move);
STATE_DEFINE(dAcORockDragon_c, Upper_End);
STATE_DEFINE(dAcORockDragon_c, Lower_Wait);
STATE_DEFINE(dAcORockDragon_c, Lower_RockCatch);
STATE_DEFINE(dAcORockDragon_c, Lower_End);
STATE_DEFINE(dAcORockDragon_c, BossRoom_Wait);

void dAcORockDragon_c::initializeState_Upper_Wait() {}
void dAcORockDragon_c::executeState_Upper_Wait() {}
void dAcORockDragon_c::finalizeState_Upper_Wait() {}
void dAcORockDragon_c::initializeState_Upper_Move() {}
void dAcORockDragon_c::executeState_Upper_Move() {}
void dAcORockDragon_c::finalizeState_Upper_Move() {}
void dAcORockDragon_c::initializeState_Upper_End() {}
void dAcORockDragon_c::executeState_Upper_End() {}
void dAcORockDragon_c::finalizeState_Upper_End() {}
void dAcORockDragon_c::initializeState_Lower_Wait() {}
void dAcORockDragon_c::executeState_Lower_Wait() {}
void dAcORockDragon_c::finalizeState_Lower_Wait() {}
void dAcORockDragon_c::initializeState_Lower_RockCatch() {}
void dAcORockDragon_c::executeState_Lower_RockCatch() {}
void dAcORockDragon_c::finalizeState_Lower_RockCatch() {}
void dAcORockDragon_c::initializeState_Lower_End() {}
void dAcORockDragon_c::executeState_Lower_End() {}
void dAcORockDragon_c::finalizeState_Lower_End() {}
void dAcORockDragon_c::initializeState_BossRoom_Wait() {}
void dAcORockDragon_c::executeState_BossRoom_Wait() {}
void dAcORockDragon_c::finalizeState_BossRoom_Wait() {}
