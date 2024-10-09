#include <d/t/d_t_insect.h>

SPECIAL_ACTOR_PROFILE(TAG_INSECT, dTgInsect_c, fProfile::TAG_INSECT, 0x167, 0, 0);

STATE_DEFINE(dTgInsect_c, Wait);
STATE_DEFINE(dTgInsect_c, WaitCreate);
STATE_DEFINE(dTgInsect_c, WaitForceEscape);
STATE_DEFINE(dTgInsect_c, End);

void dTgInsect_c::initializeState_Wait() {}
void dTgInsect_c::executeState_Wait() {}
void dTgInsect_c::finalizeState_Wait() {}
void dTgInsect_c::initializeState_WaitCreate() {}
void dTgInsect_c::executeState_WaitCreate() {}
void dTgInsect_c::finalizeState_WaitCreate() {}
void dTgInsect_c::initializeState_WaitForceEscape() {}
void dTgInsect_c::executeState_WaitForceEscape() {}
void dTgInsect_c::finalizeState_WaitForceEscape() {}
void dTgInsect_c::initializeState_End() {}
void dTgInsect_c::executeState_End() {}
void dTgInsect_c::finalizeState_End() {}
