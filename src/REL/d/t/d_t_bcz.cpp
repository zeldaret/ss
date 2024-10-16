#include "d/t/d_t_bcz.h"

SPECIAL_ACTOR_PROFILE(BCZ_TAG, dTgBcZ_c, fProfile::BCZ_TAG, 0xD5, 0, 0);

STATE_DEFINE(dTgBcZ_c, Wait);
STATE_DEFINE(dTgBcZ_c, RaiseBcZ);
STATE_DEFINE(dTgBcZ_c, RaiseLeft);
STATE_DEFINE(dTgBcZ_c, ControlBcZ);
STATE_DEFINE(dTgBcZ_c, DoNothing);

void dTgBcZ_c::initializeState_Wait() {}
void dTgBcZ_c::executeState_Wait() {}
void dTgBcZ_c::finalizeState_Wait() {}
void dTgBcZ_c::initializeState_RaiseBcZ() {}
void dTgBcZ_c::executeState_RaiseBcZ() {}
void dTgBcZ_c::finalizeState_RaiseBcZ() {}
void dTgBcZ_c::initializeState_RaiseLeft() {}
void dTgBcZ_c::executeState_RaiseLeft() {}
void dTgBcZ_c::finalizeState_RaiseLeft() {}
void dTgBcZ_c::initializeState_ControlBcZ() {}
void dTgBcZ_c::executeState_ControlBcZ() {}
void dTgBcZ_c::finalizeState_ControlBcZ() {}
void dTgBcZ_c::initializeState_DoNothing() {}
void dTgBcZ_c::executeState_DoNothing() {}
void dTgBcZ_c::finalizeState_DoNothing() {}
