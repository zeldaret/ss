#include "d/a/e/d_a_e_skytail.h"

SPECIAL_ACTOR_PROFILE(E_SKYTAIL, dAcESkytail_c, fProfile::E_SKYTAIL, 0xD7, 0, 2);

STATE_DEFINE(dAcESkytail_c, RandomMove);
STATE_DEFINE(dAcESkytail_c, Move);
STATE_DEFINE(dAcESkytail_c, ParallelMove);
STATE_DEFINE(dAcESkytail_c, FrontAttack);
STATE_DEFINE(dAcESkytail_c, Charge);
STATE_DEFINE(dAcESkytail_c, Dead);
STATE_DEFINE(dAcESkytail_c, Skytail);

void dAcESkytail_c::initializeState_RandomMove() {}
void dAcESkytail_c::executeState_RandomMove() {}
void dAcESkytail_c::finalizeState_RandomMove() {}
void dAcESkytail_c::initializeState_Move() {}
void dAcESkytail_c::executeState_Move() {}
void dAcESkytail_c::finalizeState_Move() {}
void dAcESkytail_c::initializeState_ParallelMove() {}
void dAcESkytail_c::executeState_ParallelMove() {}
void dAcESkytail_c::finalizeState_ParallelMove() {}
void dAcESkytail_c::initializeState_FrontAttack() {}
void dAcESkytail_c::executeState_FrontAttack() {}
void dAcESkytail_c::finalizeState_FrontAttack() {}
void dAcESkytail_c::initializeState_Charge() {}
void dAcESkytail_c::executeState_Charge() {}
void dAcESkytail_c::finalizeState_Charge() {}
void dAcESkytail_c::initializeState_Dead() {}
void dAcESkytail_c::executeState_Dead() {}
void dAcESkytail_c::finalizeState_Dead() {}
void dAcESkytail_c::initializeState_Skytail() {}
void dAcESkytail_c::executeState_Skytail() {}
void dAcESkytail_c::finalizeState_Skytail() {}
