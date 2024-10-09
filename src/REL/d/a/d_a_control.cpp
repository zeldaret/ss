#include <d/a/d_a_control.h>

SPECIAL_ACTOR_PROFILE(ENEMY_CONTROL, dAcEnCl_c, fProfile::ENEMY_CONTROL, 0xE0, 0, 0);

STATE_DEFINE(dAcEnCl_c, pathMove);
STATE_DEFINE(dAcEnCl_c, pathRoof);
STATE_DEFINE(dAcEnCl_c, pathStop);
STATE_DEFINE(dAcEnCl_c, rangeCheck_All);
STATE_DEFINE(dAcEnCl_c, infinite);
STATE_DEFINE(dAcEnCl_c, rangeMove);
STATE_DEFINE(dAcEnCl_c, rangeSearch);

void dAcEnCl_c::initializeState_pathMove() {}
void dAcEnCl_c::executeState_pathMove() {}
void dAcEnCl_c::finalizeState_pathMove() {}
void dAcEnCl_c::initializeState_pathRoof() {}
void dAcEnCl_c::executeState_pathRoof() {}
void dAcEnCl_c::finalizeState_pathRoof() {}
void dAcEnCl_c::initializeState_pathStop() {}
void dAcEnCl_c::executeState_pathStop() {}
void dAcEnCl_c::finalizeState_pathStop() {}
void dAcEnCl_c::initializeState_rangeCheck_All() {}
void dAcEnCl_c::executeState_rangeCheck_All() {}
void dAcEnCl_c::finalizeState_rangeCheck_All() {}
void dAcEnCl_c::initializeState_infinite() {}
void dAcEnCl_c::executeState_infinite() {}
void dAcEnCl_c::finalizeState_infinite() {}
void dAcEnCl_c::initializeState_rangeMove() {}
void dAcEnCl_c::executeState_rangeMove() {}
void dAcEnCl_c::finalizeState_rangeMove() {}
void dAcEnCl_c::initializeState_rangeSearch() {}
void dAcEnCl_c::executeState_rangeSearch() {}
void dAcEnCl_c::finalizeState_rangeSearch() {}
