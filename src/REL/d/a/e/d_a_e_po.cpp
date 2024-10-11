#include <d/a/e/d_a_e_po.h>

SPECIAL_ACTOR_PROFILE(E_PO, dAcEPo_c, fProfile::E_PO, 0xF0, 0, 3);

STATE_DEFINE(dAcEPo_c, Stop);
STATE_DEFINE(dAcEPo_c, Revival);
STATE_DEFINE(dAcEPo_c, Wait);
STATE_DEFINE(dAcEPo_c, Move);
STATE_DEFINE(dAcEPo_c, PathMove);
STATE_DEFINE(dAcEPo_c, Find);
STATE_DEFINE(dAcEPo_c, Event);
STATE_DEFINE(dAcEPo_c, SearchMove);

void dAcEPo_c::initializeState_Stop() {}
void dAcEPo_c::executeState_Stop() {}
void dAcEPo_c::finalizeState_Stop() {}
void dAcEPo_c::initializeState_Revival() {}
void dAcEPo_c::executeState_Revival() {}
void dAcEPo_c::finalizeState_Revival() {}
void dAcEPo_c::initializeState_Wait() {}
void dAcEPo_c::executeState_Wait() {}
void dAcEPo_c::finalizeState_Wait() {}
void dAcEPo_c::initializeState_Move() {}
void dAcEPo_c::executeState_Move() {}
void dAcEPo_c::finalizeState_Move() {}
void dAcEPo_c::initializeState_PathMove() {}
void dAcEPo_c::executeState_PathMove() {}
void dAcEPo_c::finalizeState_PathMove() {}
void dAcEPo_c::initializeState_Find() {}
void dAcEPo_c::executeState_Find() {}
void dAcEPo_c::finalizeState_Find() {}
void dAcEPo_c::initializeState_Event() {}
void dAcEPo_c::executeState_Event() {}
void dAcEPo_c::finalizeState_Event() {}
void dAcEPo_c::initializeState_SearchMove() {}
void dAcEPo_c::executeState_SearchMove() {}
void dAcEPo_c::finalizeState_SearchMove() {}
