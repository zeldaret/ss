#include "d/a/d_a_little_bird.h"

SPECIAL_ACTOR_PROFILE(LITTLE_BIRD, dAcLittleBird_c, fProfile::LITTLE_BIRD, 0x99, 0, 391);

STATE_DEFINE(dAcLittleBird_c, Init);
STATE_DEFINE(dAcLittleBird_c, Wait);
STATE_DEFINE(dAcLittleBird_c, Scratch);
STATE_DEFINE(dAcLittleBird_c, Hop);
STATE_DEFINE(dAcLittleBird_c, Eat);
STATE_DEFINE(dAcLittleBird_c, Sing);
STATE_DEFINE(dAcLittleBird_c, PreEscape);
STATE_DEFINE(dAcLittleBird_c, PreEscape2);
STATE_DEFINE(dAcLittleBird_c, FlyReady);
STATE_DEFINE(dAcLittleBird_c, FlyEscape);
STATE_DEFINE(dAcLittleBird_c, FlyKakku);
STATE_DEFINE(dAcLittleBird_c, FlyReturn);
STATE_DEFINE(dAcLittleBird_c, LandOn);
STATE_DEFINE(dAcLittleBird_c, LandReady);

void dAcLittleBird_c::initializeState_Init() {}
void dAcLittleBird_c::executeState_Init() {}
void dAcLittleBird_c::finalizeState_Init() {}
void dAcLittleBird_c::initializeState_Wait() {}
void dAcLittleBird_c::executeState_Wait() {}
void dAcLittleBird_c::finalizeState_Wait() {}
void dAcLittleBird_c::initializeState_Scratch() {}
void dAcLittleBird_c::executeState_Scratch() {}
void dAcLittleBird_c::finalizeState_Scratch() {}
void dAcLittleBird_c::initializeState_Hop() {}
void dAcLittleBird_c::executeState_Hop() {}
void dAcLittleBird_c::finalizeState_Hop() {}
void dAcLittleBird_c::initializeState_Eat() {}
void dAcLittleBird_c::executeState_Eat() {}
void dAcLittleBird_c::finalizeState_Eat() {}
void dAcLittleBird_c::initializeState_Sing() {}
void dAcLittleBird_c::executeState_Sing() {}
void dAcLittleBird_c::finalizeState_Sing() {}
void dAcLittleBird_c::initializeState_PreEscape() {}
void dAcLittleBird_c::executeState_PreEscape() {}
void dAcLittleBird_c::finalizeState_PreEscape() {}
void dAcLittleBird_c::initializeState_PreEscape2() {}
void dAcLittleBird_c::executeState_PreEscape2() {}
void dAcLittleBird_c::finalizeState_PreEscape2() {}
void dAcLittleBird_c::initializeState_FlyReady() {}
void dAcLittleBird_c::executeState_FlyReady() {}
void dAcLittleBird_c::finalizeState_FlyReady() {}
void dAcLittleBird_c::initializeState_FlyEscape() {}
void dAcLittleBird_c::executeState_FlyEscape() {}
void dAcLittleBird_c::finalizeState_FlyEscape() {}
void dAcLittleBird_c::initializeState_FlyKakku() {}
void dAcLittleBird_c::executeState_FlyKakku() {}
void dAcLittleBird_c::finalizeState_FlyKakku() {}
void dAcLittleBird_c::initializeState_FlyReturn() {}
void dAcLittleBird_c::executeState_FlyReturn() {}
void dAcLittleBird_c::finalizeState_FlyReturn() {}
void dAcLittleBird_c::initializeState_LandOn() {}
void dAcLittleBird_c::executeState_LandOn() {}
void dAcLittleBird_c::finalizeState_LandOn() {}
void dAcLittleBird_c::initializeState_LandReady() {}
void dAcLittleBird_c::executeState_LandReady() {}
void dAcLittleBird_c::finalizeState_LandReady() {}
