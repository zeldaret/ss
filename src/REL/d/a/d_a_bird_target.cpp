#include "d/a/d_a_bird_target.h"

SPECIAL_ACTOR_PROFILE(BIRD_TARGET, dBird_Target_c, fProfile::BIRD_TARGET, 0xAD, 0, 0);

STATE_DEFINE(dBird_Target_c, Wait);
STATE_DEFINE(dBird_Target_c, Move);
STATE_DEFINE(dBird_Target_c, LevelUpDemo);
STATE_DEFINE(dBird_Target_c, NpcWait);
STATE_DEFINE(dBird_Target_c, NpcGlide);

void dBird_Target_c::initializeState_Wait() {}
void dBird_Target_c::executeState_Wait() {}
void dBird_Target_c::finalizeState_Wait() {}
void dBird_Target_c::initializeState_Move() {}
void dBird_Target_c::executeState_Move() {}
void dBird_Target_c::finalizeState_Move() {}
void dBird_Target_c::initializeState_LevelUpDemo() {}
void dBird_Target_c::executeState_LevelUpDemo() {}
void dBird_Target_c::finalizeState_LevelUpDemo() {}
void dBird_Target_c::initializeState_NpcWait() {}
void dBird_Target_c::executeState_NpcWait() {}
void dBird_Target_c::finalizeState_NpcWait() {}
void dBird_Target_c::initializeState_NpcGlide() {}
void dBird_Target_c::executeState_NpcGlide() {}
void dBird_Target_c::finalizeState_NpcGlide() {}
