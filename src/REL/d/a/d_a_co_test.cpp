#include <d/a/d_a_co_test.h>

SPECIAL_ACTOR_PROFILE(CO_TEST, dAcCoTest, fProfile::CO_TEST, 0X1B4, 0, 1);

STATE_DEFINE(dAcCoTest, Wait);

void dAcCoTest::initializeState_Wait() {}
void dAcCoTest::executeState_Wait() {}
void dAcCoTest::finalizeState_Wait() {}
