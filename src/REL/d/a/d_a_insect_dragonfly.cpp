#include <d/a/d_a_insect_dragonfly.h>

SPECIAL_ACTOR_PROFILE(INSECT_DRAGONFLY, dAcInsectDragonfly_c, fProfile::INSECT_DRAGONFLY, 0x169, 0, 3);

STATE_DEFINE(dAcInsectDragonfly_c, Wait);
STATE_DEFINE(dAcInsectDragonfly_c, Move);
STATE_DEFINE(dAcInsectDragonfly_c, Escape);
STATE_DEFINE(dAcInsectDragonfly_c, Fly);
STATE_DEFINE(dAcInsectDragonfly_c, Finalize);
STATE_DEFINE(dAcInsectDragonfly_c, Dead);

void dAcInsectDragonfly_c::initializeState_Wait() {}
void dAcInsectDragonfly_c::executeState_Wait() {}
void dAcInsectDragonfly_c::finalizeState_Wait() {}
void dAcInsectDragonfly_c::initializeState_Move() {}
void dAcInsectDragonfly_c::executeState_Move() {}
void dAcInsectDragonfly_c::finalizeState_Move() {}
void dAcInsectDragonfly_c::initializeState_Escape() {}
void dAcInsectDragonfly_c::executeState_Escape() {}
void dAcInsectDragonfly_c::finalizeState_Escape() {}
void dAcInsectDragonfly_c::initializeState_Fly() {}
void dAcInsectDragonfly_c::executeState_Fly() {}
void dAcInsectDragonfly_c::finalizeState_Fly() {}
void dAcInsectDragonfly_c::initializeState_Finalize() {}
void dAcInsectDragonfly_c::executeState_Finalize() {}
void dAcInsectDragonfly_c::finalizeState_Finalize() {}
void dAcInsectDragonfly_c::initializeState_Dead() {}
void dAcInsectDragonfly_c::executeState_Dead() {}
void dAcInsectDragonfly_c::finalizeState_Dead() {}
