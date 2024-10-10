#include <d/t/d_t_group_test.h>

SPECIAL_ACTOR_PROFILE(GROUP_TEST, dTgGroupTest_c, fProfile::GROUP_TEST, 0xC4, 0, 0);

STATE_DEFINE(dTgGroupTest_c, Wait);
STATE_DEFINE(dTgGroupTest_c, CreateLeft);
STATE_DEFINE(dTgGroupTest_c, Fight);
STATE_DEFINE(dTgGroupTest_c, DoNothing);

void dTgGroupTest_c::initializeState_Wait() {}
void dTgGroupTest_c::executeState_Wait() {}
void dTgGroupTest_c::finalizeState_Wait() {}
void dTgGroupTest_c::initializeState_CreateLeft() {}
void dTgGroupTest_c::executeState_CreateLeft() {}
void dTgGroupTest_c::finalizeState_CreateLeft() {}
void dTgGroupTest_c::initializeState_Fight() {}
void dTgGroupTest_c::executeState_Fight() {}
void dTgGroupTest_c::finalizeState_Fight() {}
void dTgGroupTest_c::initializeState_DoNothing() {}
void dTgGroupTest_c::executeState_DoNothing() {}
void dTgGroupTest_c::finalizeState_DoNothing() {}
