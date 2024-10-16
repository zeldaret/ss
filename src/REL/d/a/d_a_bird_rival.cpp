#include "d/a/d_a_bird_rival.h"

SPECIAL_ACTOR_PROFILE(BIRD_RIVAL, dBird_Rival_c, fProfile::BIRD_RIVAL, 0xAA, 0, 2);

STATE_DEFINE(dBird_Rival_c, Wait);
STATE_DEFINE(dBird_Rival_c, Chase);
STATE_DEFINE(dBird_Rival_c, FrontAttack);
STATE_DEFINE(dBird_Rival_c, BodyAttack);
STATE_DEFINE(dBird_Rival_c, LevelUpDemo);
STATE_DEFINE(dBird_Rival_c, CatchDemo);
STATE_DEFINE(dBird_Rival_c, PathMove);

void dBird_Rival_c::initializeState_Wait() {}
void dBird_Rival_c::executeState_Wait() {}
void dBird_Rival_c::finalizeState_Wait() {}
void dBird_Rival_c::initializeState_Chase() {}
void dBird_Rival_c::executeState_Chase() {}
void dBird_Rival_c::finalizeState_Chase() {}
void dBird_Rival_c::initializeState_FrontAttack() {}
void dBird_Rival_c::executeState_FrontAttack() {}
void dBird_Rival_c::finalizeState_FrontAttack() {}
void dBird_Rival_c::initializeState_BodyAttack() {}
void dBird_Rival_c::executeState_BodyAttack() {}
void dBird_Rival_c::finalizeState_BodyAttack() {}
void dBird_Rival_c::initializeState_LevelUpDemo() {}
void dBird_Rival_c::executeState_LevelUpDemo() {}
void dBird_Rival_c::finalizeState_LevelUpDemo() {}
void dBird_Rival_c::initializeState_CatchDemo() {}
void dBird_Rival_c::executeState_CatchDemo() {}
void dBird_Rival_c::finalizeState_CatchDemo() {}
void dBird_Rival_c::initializeState_PathMove() {}
void dBird_Rival_c::executeState_PathMove() {}
void dBird_Rival_c::finalizeState_PathMove() {}
