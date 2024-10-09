#include <d/a/d_a_insect_beetle.h>

SPECIAL_ACTOR_PROFILE(INSECT_BEETLE, dAcInsectBeetle_c, fProfile::INSECT_BEETLE, 0X16A, 0, 3);

STATE_DEFINE(dAcInsectBeetle_c, Wait);
STATE_DEFINE(dAcInsectBeetle_c, Walk);
STATE_DEFINE(dAcInsectBeetle_c, Escape);
STATE_DEFINE(dAcInsectBeetle_c, Fly);
STATE_DEFINE(dAcInsectBeetle_c, Fall);
STATE_DEFINE(dAcInsectBeetle_c, Getup);
STATE_DEFINE(dAcInsectBeetle_c, Finalize);
STATE_DEFINE(dAcInsectBeetle_c, Dead);
STATE_DEFINE(dAcInsectBeetle_c, EscapeChild);

void dAcInsectBeetle_c::initializeState_Wait() {}
void dAcInsectBeetle_c::executeState_Wait() {}
void dAcInsectBeetle_c::finalizeState_Wait() {}
void dAcInsectBeetle_c::initializeState_Walk() {}
void dAcInsectBeetle_c::executeState_Walk() {}
void dAcInsectBeetle_c::finalizeState_Walk() {}
void dAcInsectBeetle_c::initializeState_Escape() {}
void dAcInsectBeetle_c::executeState_Escape() {}
void dAcInsectBeetle_c::finalizeState_Escape() {}
void dAcInsectBeetle_c::initializeState_Fly() {}
void dAcInsectBeetle_c::executeState_Fly() {}
void dAcInsectBeetle_c::finalizeState_Fly() {}
void dAcInsectBeetle_c::initializeState_Fall() {}
void dAcInsectBeetle_c::executeState_Fall() {}
void dAcInsectBeetle_c::finalizeState_Fall() {}
void dAcInsectBeetle_c::initializeState_Getup() {}
void dAcInsectBeetle_c::executeState_Getup() {}
void dAcInsectBeetle_c::finalizeState_Getup() {}
void dAcInsectBeetle_c::initializeState_Finalize() {}
void dAcInsectBeetle_c::executeState_Finalize() {}
void dAcInsectBeetle_c::finalizeState_Finalize() {}
void dAcInsectBeetle_c::initializeState_Dead() {}
void dAcInsectBeetle_c::executeState_Dead() {}
void dAcInsectBeetle_c::finalizeState_Dead() {}
void dAcInsectBeetle_c::initializeState_EscapeChild() {}
void dAcInsectBeetle_c::executeState_EscapeChild() {}
void dAcInsectBeetle_c::finalizeState_EscapeChild() {}
