#include "d/t/d_t_ks.h"

SPECIAL_ACTOR_PROFILE(KIESU_TAG, dTgKiesuTag_c, fProfile::KIESU_TAG, 0xFD, 0, 0);

STATE_DEFINE(dTgKiesuTag_c, CreateChildren);
STATE_DEFINE(dTgKiesuTag_c, ControlWait);
STATE_DEFINE(dTgKiesuTag_c, ControlReturnToWait);
STATE_DEFINE(dTgKiesuTag_c, ControlMove);
STATE_DEFINE(dTgKiesuTag_c, ControlFighting);
STATE_DEFINE(dTgKiesuTag_c, ControlPathMove);
STATE_DEFINE(dTgKiesuTag_c, DoNothing);

void dTgKiesuTag_c::initializeState_CreateChildren() {}
void dTgKiesuTag_c::executeState_CreateChildren() {}
void dTgKiesuTag_c::finalizeState_CreateChildren() {}
void dTgKiesuTag_c::initializeState_ControlWait() {}
void dTgKiesuTag_c::executeState_ControlWait() {}
void dTgKiesuTag_c::finalizeState_ControlWait() {}
void dTgKiesuTag_c::initializeState_ControlReturnToWait() {}
void dTgKiesuTag_c::executeState_ControlReturnToWait() {}
void dTgKiesuTag_c::finalizeState_ControlReturnToWait() {}
void dTgKiesuTag_c::initializeState_ControlMove() {}
void dTgKiesuTag_c::executeState_ControlMove() {}
void dTgKiesuTag_c::finalizeState_ControlMove() {}
void dTgKiesuTag_c::initializeState_ControlFighting() {}
void dTgKiesuTag_c::executeState_ControlFighting() {}
void dTgKiesuTag_c::finalizeState_ControlFighting() {}
void dTgKiesuTag_c::initializeState_ControlPathMove() {}
void dTgKiesuTag_c::executeState_ControlPathMove() {}
void dTgKiesuTag_c::finalizeState_ControlPathMove() {}
void dTgKiesuTag_c::initializeState_DoNothing() {}
void dTgKiesuTag_c::executeState_DoNothing() {}
void dTgKiesuTag_c::finalizeState_DoNothing() {}
