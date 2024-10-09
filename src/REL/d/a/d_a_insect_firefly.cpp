#include <d/a/d_a_insect_firefly.h>

SPECIAL_ACTOR_PROFILE(INSECT_FIREFLY, dAcInsectFirefly_c, fProfile::INSECT_FIREFLY, 0X170, 0, 3);

STATE_DEFINE(dAcInsectFirefly_c, Move);
STATE_DEFINE(dAcInsectFirefly_c, Escape);
STATE_DEFINE(dAcInsectFirefly_c, Fly);
STATE_DEFINE(dAcInsectFirefly_c, Finalize);
STATE_DEFINE(dAcInsectFirefly_c, Dead);

void dAcInsectFirefly_c::initializeState_Move() {}
void dAcInsectFirefly_c::executeState_Move() {}
void dAcInsectFirefly_c::finalizeState_Move() {}
void dAcInsectFirefly_c::initializeState_Escape() {}
void dAcInsectFirefly_c::executeState_Escape() {}
void dAcInsectFirefly_c::finalizeState_Escape() {}
void dAcInsectFirefly_c::initializeState_Fly() {}
void dAcInsectFirefly_c::executeState_Fly() {}
void dAcInsectFirefly_c::finalizeState_Fly() {}
void dAcInsectFirefly_c::initializeState_Finalize() {}
void dAcInsectFirefly_c::executeState_Finalize() {}
void dAcInsectFirefly_c::finalizeState_Finalize() {}
void dAcInsectFirefly_c::initializeState_Dead() {}
void dAcInsectFirefly_c::executeState_Dead() {}
void dAcInsectFirefly_c::finalizeState_Dead() {}
