#include <d/a/e/d_a_e_siren.h>

SPECIAL_ACTOR_PROFILE(E_SIREN, dAcESiren_c, fProfile::E_SIREN, 0XEF, 0, 3);

STATE_DEFINE(dAcESiren_c, Wait);
STATE_DEFINE(dAcESiren_c, Revival);
STATE_DEFINE(dAcESiren_c, Move);
STATE_DEFINE(dAcESiren_c, PathMove);
STATE_DEFINE(dAcESiren_c, Turn);
STATE_DEFINE(dAcESiren_c, Event);

void dAcESiren_c::initializeState_Wait() {}
void dAcESiren_c::executeState_Wait() {}
void dAcESiren_c::finalizeState_Wait() {}
void dAcESiren_c::initializeState_Revival() {}
void dAcESiren_c::executeState_Revival() {}
void dAcESiren_c::finalizeState_Revival() {}
void dAcESiren_c::initializeState_Move() {}
void dAcESiren_c::executeState_Move() {}
void dAcESiren_c::finalizeState_Move() {}
void dAcESiren_c::initializeState_PathMove() {}
void dAcESiren_c::executeState_PathMove() {}
void dAcESiren_c::finalizeState_PathMove() {}
void dAcESiren_c::initializeState_Turn() {}
void dAcESiren_c::executeState_Turn() {}
void dAcESiren_c::finalizeState_Turn() {}
void dAcESiren_c::initializeState_Event() {}
void dAcESiren_c::executeState_Event() {}
void dAcESiren_c::finalizeState_Event() {}
