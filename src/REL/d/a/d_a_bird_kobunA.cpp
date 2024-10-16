#include "d/a/d_a_bird_kobunA.h"

SPECIAL_ACTOR_PROFILE(BIRD_KOBUNA, dBird_KobunA_c, fProfile::BIRD_KOBUNA, 0xAB, 0, 2);

STATE_DEFINE(dBird_KobunA_c, Wait);
STATE_DEFINE(dBird_KobunA_c, Chase);
STATE_DEFINE(dBird_KobunA_c, FrontAttack);
STATE_DEFINE(dBird_KobunA_c, PathMove);
STATE_DEFINE(dBird_KobunA_c, LevelUpDemo);
STATE_DEFINE(dBird_KobunA_c, BodyAttack);

void dBird_KobunA_c::initializeState_Wait() {}
void dBird_KobunA_c::executeState_Wait() {}
void dBird_KobunA_c::finalizeState_Wait() {}
void dBird_KobunA_c::initializeState_Chase() {}
void dBird_KobunA_c::executeState_Chase() {}
void dBird_KobunA_c::finalizeState_Chase() {}
void dBird_KobunA_c::initializeState_FrontAttack() {}
void dBird_KobunA_c::executeState_FrontAttack() {}
void dBird_KobunA_c::finalizeState_FrontAttack() {}
void dBird_KobunA_c::initializeState_PathMove() {}
void dBird_KobunA_c::executeState_PathMove() {}
void dBird_KobunA_c::finalizeState_PathMove() {}
void dBird_KobunA_c::initializeState_LevelUpDemo() {}
void dBird_KobunA_c::executeState_LevelUpDemo() {}
void dBird_KobunA_c::finalizeState_LevelUpDemo() {}
void dBird_KobunA_c::initializeState_BodyAttack() {}
void dBird_KobunA_c::executeState_BodyAttack() {}
void dBird_KobunA_c::finalizeState_BodyAttack() {}
