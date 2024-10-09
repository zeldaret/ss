#include <d/a/d_a_insect_ladybug.h>

SPECIAL_ACTOR_PROFILE(INSECT_LADYBUG, dAcInsectLadybug_c, fProfile::INSECT_LADYBUG, 0X168, 0, 3);

STATE_DEFINE(dAcInsectLadybug_c, Wait);
STATE_DEFINE(dAcInsectLadybug_c, Walk);
STATE_DEFINE(dAcInsectLadybug_c, Escape);
STATE_DEFINE(dAcInsectLadybug_c, Fly);
STATE_DEFINE(dAcInsectLadybug_c, Finalize);
STATE_DEFINE(dAcInsectLadybug_c, Dead);

void dAcInsectLadybug_c::initializeState_Wait() {}
void dAcInsectLadybug_c::executeState_Wait() {}
void dAcInsectLadybug_c::finalizeState_Wait() {}
void dAcInsectLadybug_c::initializeState_Walk() {}
void dAcInsectLadybug_c::executeState_Walk() {}
void dAcInsectLadybug_c::finalizeState_Walk() {}
void dAcInsectLadybug_c::initializeState_Escape() {}
void dAcInsectLadybug_c::executeState_Escape() {}
void dAcInsectLadybug_c::finalizeState_Escape() {}
void dAcInsectLadybug_c::initializeState_Fly() {}
void dAcInsectLadybug_c::executeState_Fly() {}
void dAcInsectLadybug_c::finalizeState_Fly() {}
void dAcInsectLadybug_c::initializeState_Finalize() {}
void dAcInsectLadybug_c::executeState_Finalize() {}
void dAcInsectLadybug_c::finalizeState_Finalize() {}
void dAcInsectLadybug_c::initializeState_Dead() {}
void dAcInsectLadybug_c::executeState_Dead() {}
void dAcInsectLadybug_c::finalizeState_Dead() {}
