#include <d/a/d_a_bird_kobunB.h>

SPECIAL_ACTOR_PROFILE(BIRD_KOBUNB, dBird_KobunB_c, fProfile::BIRD_KOBUNB, 0xAC, 0, 2);

STATE_DEFINE(dBird_KobunB_c, Wait);
STATE_DEFINE(dBird_KobunB_c, Chase);
STATE_DEFINE(dBird_KobunB_c, FrontAttack);
STATE_DEFINE(dBird_KobunB_c, PathMove);
STATE_DEFINE(dBird_KobunB_c, BodyAttack);
STATE_DEFINE(dBird_KobunB_c, LevelUpDemo);

void dBird_KobunB_c::initializeState_Wait() {}
void dBird_KobunB_c::executeState_Wait() {}
void dBird_KobunB_c::finalizeState_Wait() {}
void dBird_KobunB_c::initializeState_Chase() {}
void dBird_KobunB_c::executeState_Chase() {}
void dBird_KobunB_c::finalizeState_Chase() {}
void dBird_KobunB_c::initializeState_FrontAttack() {}
void dBird_KobunB_c::executeState_FrontAttack() {}
void dBird_KobunB_c::finalizeState_FrontAttack() {}
void dBird_KobunB_c::initializeState_PathMove() {}
void dBird_KobunB_c::executeState_PathMove() {}
void dBird_KobunB_c::finalizeState_PathMove() {}
void dBird_KobunB_c::initializeState_BodyAttack() {}
void dBird_KobunB_c::executeState_BodyAttack() {}
void dBird_KobunB_c::finalizeState_BodyAttack() {}
void dBird_KobunB_c::initializeState_LevelUpDemo() {}
void dBird_KobunB_c::executeState_LevelUpDemo() {}
void dBird_KobunB_c::finalizeState_LevelUpDemo() {}
