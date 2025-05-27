#include "d/a/d_a_bird.h"

SPECIAL_ACTOR_PROFILE(BIRD, dBird_c, fProfile::BIRD, 0xA8, 0, 2);

STATE_DEFINE(dBird_c, Wait);
STATE_DEFINE(dBird_c, PreMove);
STATE_DEFINE(dBird_c, Move);
STATE_DEFINE(dBird_c, Hide);
STATE_DEFINE(dBird_c, RideDemo);
STATE_DEFINE(dBird_c, FirstRideDemo);
STATE_DEFINE(dBird_c, OutAreaDemo);
STATE_DEFINE(dBird_c, RideOff);
STATE_DEFINE(dBird_c, OnGround);
STATE_DEFINE(dBird_c, Tornado);
STATE_DEFINE(dBird_c, SaveStartDemo);
STATE_DEFINE(dBird_c, MiniGameWait);
STATE_DEFINE(dBird_c, MiniGameMove);
STATE_DEFINE(dBird_c, MiniGameEnd);
STATE_DEFINE(dBird_c, MiniGameLevelUpDemo);

dBird_c *dBird_c::spInstance;

void dBird_c::initializeState_Wait() {}
void dBird_c::executeState_Wait() {}
void dBird_c::finalizeState_Wait() {}
void dBird_c::initializeState_PreMove() {}
void dBird_c::executeState_PreMove() {}
void dBird_c::finalizeState_PreMove() {}
void dBird_c::initializeState_Move() {}
void dBird_c::executeState_Move() {}
void dBird_c::finalizeState_Move() {}
void dBird_c::initializeState_Hide() {}
void dBird_c::executeState_Hide() {}
void dBird_c::finalizeState_Hide() {}
void dBird_c::initializeState_RideDemo() {}
void dBird_c::executeState_RideDemo() {}
void dBird_c::finalizeState_RideDemo() {}
void dBird_c::initializeState_FirstRideDemo() {}
void dBird_c::executeState_FirstRideDemo() {}
void dBird_c::finalizeState_FirstRideDemo() {}
void dBird_c::initializeState_OutAreaDemo() {}
void dBird_c::executeState_OutAreaDemo() {}
void dBird_c::finalizeState_OutAreaDemo() {}
void dBird_c::initializeState_RideOff() {}
void dBird_c::executeState_RideOff() {}
void dBird_c::finalizeState_RideOff() {}
void dBird_c::initializeState_OnGround() {}
void dBird_c::executeState_OnGround() {}
void dBird_c::finalizeState_OnGround() {}
void dBird_c::initializeState_Tornado() {}
void dBird_c::executeState_Tornado() {}
void dBird_c::finalizeState_Tornado() {}
void dBird_c::initializeState_SaveStartDemo() {}
void dBird_c::executeState_SaveStartDemo() {}
void dBird_c::finalizeState_SaveStartDemo() {}
void dBird_c::initializeState_MiniGameWait() {}
void dBird_c::executeState_MiniGameWait() {}
void dBird_c::finalizeState_MiniGameWait() {}
void dBird_c::initializeState_MiniGameMove() {}
void dBird_c::executeState_MiniGameMove() {}
void dBird_c::finalizeState_MiniGameMove() {}
void dBird_c::initializeState_MiniGameEnd() {}
void dBird_c::executeState_MiniGameEnd() {}
void dBird_c::finalizeState_MiniGameEnd() {}
void dBird_c::initializeState_MiniGameLevelUpDemo() {}
void dBird_c::executeState_MiniGameLevelUpDemo() {}
void dBird_c::finalizeState_MiniGameLevelUpDemo() {}

void speedUpAccel(mVec3_c &param) {}
