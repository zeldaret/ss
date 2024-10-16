#include "d/a/d_a_insect_ant.h"

SPECIAL_ACTOR_PROFILE(INSECT_ANT, dAcInsectAnt_c, fProfile::INSECT_ANT, 0x16D, 0, 3);

STATE_DEFINE(dAcInsectAnt_c, Wait);
STATE_DEFINE(dAcInsectAnt_c, Walk);
STATE_DEFINE(dAcInsectAnt_c, Escape);
STATE_DEFINE(dAcInsectAnt_c, Dig);
STATE_DEFINE(dAcInsectAnt_c, Dead);
STATE_DEFINE(dAcInsectAnt_c, Finalize);

void dAcInsectAnt_c::initializeState_Wait() {}
void dAcInsectAnt_c::executeState_Wait() {}
void dAcInsectAnt_c::finalizeState_Wait() {}
void dAcInsectAnt_c::initializeState_Walk() {}
void dAcInsectAnt_c::executeState_Walk() {}
void dAcInsectAnt_c::finalizeState_Walk() {}
void dAcInsectAnt_c::initializeState_Escape() {}
void dAcInsectAnt_c::executeState_Escape() {}
void dAcInsectAnt_c::finalizeState_Escape() {}
void dAcInsectAnt_c::initializeState_Dig() {}
void dAcInsectAnt_c::executeState_Dig() {}
void dAcInsectAnt_c::finalizeState_Dig() {}
void dAcInsectAnt_c::initializeState_Dead() {}
void dAcInsectAnt_c::executeState_Dead() {}
void dAcInsectAnt_c::finalizeState_Dead() {}
void dAcInsectAnt_c::initializeState_Finalize() {}
void dAcInsectAnt_c::executeState_Finalize() {}
void dAcInsectAnt_c::finalizeState_Finalize() {}
