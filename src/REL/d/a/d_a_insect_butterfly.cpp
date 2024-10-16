#include "d/a/d_a_insect_butterfly.h"

SPECIAL_ACTOR_PROFILE(INSECT_BUTTERFLY, dAcInsectButterfly_c, fProfile::INSECT_BUTTERFLY, 0x16E, 0, 3);

STATE_DEFINE(dAcInsectButterfly_c, Move);
STATE_DEFINE(dAcInsectButterfly_c, Escape);
STATE_DEFINE(dAcInsectButterfly_c, Fly);
STATE_DEFINE(dAcInsectButterfly_c, Finalize);
STATE_DEFINE(dAcInsectButterfly_c, Dead);

void dAcInsectButterfly_c::initializeState_Move() {}
void dAcInsectButterfly_c::executeState_Move() {}
void dAcInsectButterfly_c::finalizeState_Move() {}
void dAcInsectButterfly_c::initializeState_Escape() {}
void dAcInsectButterfly_c::executeState_Escape() {}
void dAcInsectButterfly_c::finalizeState_Escape() {}
void dAcInsectButterfly_c::initializeState_Fly() {}
void dAcInsectButterfly_c::executeState_Fly() {}
void dAcInsectButterfly_c::finalizeState_Fly() {}
void dAcInsectButterfly_c::initializeState_Finalize() {}
void dAcInsectButterfly_c::executeState_Finalize() {}
void dAcInsectButterfly_c::finalizeState_Finalize() {}
void dAcInsectButterfly_c::initializeState_Dead() {}
void dAcInsectButterfly_c::executeState_Dead() {}
void dAcInsectButterfly_c::finalizeState_Dead() {}
