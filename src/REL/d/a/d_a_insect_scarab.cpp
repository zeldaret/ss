#include <d/a/d_a_insect_scarab.h>

SPECIAL_ACTOR_PROFILE(INSECT_SCARAB, dAcInsectScarab_c, fProfile::INSECT_SCARAB, 0X16F, 0, 3);

STATE_DEFINE(dAcInsectScarab_c, Wait);
STATE_DEFINE(dAcInsectScarab_c, Walk);
STATE_DEFINE(dAcInsectScarab_c, Escape);
STATE_DEFINE(dAcInsectScarab_c, Fly);
STATE_DEFINE(dAcInsectScarab_c, Finalize);
STATE_DEFINE(dAcInsectScarab_c, Excrement);
STATE_DEFINE(dAcInsectScarab_c, Dead);

void dAcInsectScarab_c::initializeState_Wait() {}
void dAcInsectScarab_c::executeState_Wait() {}
void dAcInsectScarab_c::finalizeState_Wait() {}
void dAcInsectScarab_c::initializeState_Walk() {}
void dAcInsectScarab_c::executeState_Walk() {}
void dAcInsectScarab_c::finalizeState_Walk() {}
void dAcInsectScarab_c::initializeState_Escape() {}
void dAcInsectScarab_c::executeState_Escape() {}
void dAcInsectScarab_c::finalizeState_Escape() {}
void dAcInsectScarab_c::initializeState_Fly() {}
void dAcInsectScarab_c::executeState_Fly() {}
void dAcInsectScarab_c::finalizeState_Fly() {}
void dAcInsectScarab_c::initializeState_Finalize() {}
void dAcInsectScarab_c::executeState_Finalize() {}
void dAcInsectScarab_c::finalizeState_Finalize() {}
void dAcInsectScarab_c::initializeState_Excrement() {}
void dAcInsectScarab_c::executeState_Excrement() {}
void dAcInsectScarab_c::finalizeState_Excrement() {}
void dAcInsectScarab_c::initializeState_Dead() {}
void dAcInsectScarab_c::executeState_Dead() {}
void dAcInsectScarab_c::finalizeState_Dead() {}
