#include "d/a/e/d_a_e_hidokaris.h"

SPECIAL_ACTOR_PROFILE(E_HIDOKARIS, dAcEhidokaris_c, fProfile::E_HIDOKARIS, 0xE4, 0, 2);

STATE_DEFINE(dAcEhidokaris_c, Wait);
STATE_DEFINE(dAcEhidokaris_c, BombWait);
STATE_DEFINE(dAcEhidokaris_c, Dead);

void dAcEhidokaris_c::initializeState_Wait() {}
void dAcEhidokaris_c::executeState_Wait() {}
void dAcEhidokaris_c::finalizeState_Wait() {}
void dAcEhidokaris_c::initializeState_BombWait() {}
void dAcEhidokaris_c::executeState_BombWait() {}
void dAcEhidokaris_c::finalizeState_BombWait() {}
void dAcEhidokaris_c::initializeState_Dead() {}
void dAcEhidokaris_c::executeState_Dead() {}
void dAcEhidokaris_c::finalizeState_Dead() {}
