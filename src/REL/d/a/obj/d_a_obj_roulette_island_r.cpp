#include "d/a/obj/d_a_obj_roulette_island_r.h"

SPECIAL_ACTOR_PROFILE(OBJ_ROULETTE_ISLAND_R, dAcOrouletteIslandR_c, fProfile::OBJ_ROULETTE_ISLAND_R, 0x1CB, 0, 2);

STATE_DEFINE(dAcOrouletteIslandR_c, Wait);
STATE_DEFINE(dAcOrouletteIslandR_c, Move);
STATE_DEFINE(dAcOrouletteIslandR_c, Reduce);
STATE_DEFINE(dAcOrouletteIslandR_c, End);

dAcOrouletteIslandR_c::dAcOrouletteIslandR_c() : mStateMgr(*this) {}
dAcOrouletteIslandR_c::~dAcOrouletteIslandR_c() {}

void dAcOrouletteIslandR_c::initializeState_Wait() {}
void dAcOrouletteIslandR_c::executeState_Wait() {}
void dAcOrouletteIslandR_c::finalizeState_Wait() {}
void dAcOrouletteIslandR_c::initializeState_Move() {}
void dAcOrouletteIslandR_c::executeState_Move() {}
void dAcOrouletteIslandR_c::finalizeState_Move() {}
void dAcOrouletteIslandR_c::initializeState_Reduce() {}
void dAcOrouletteIslandR_c::executeState_Reduce() {}
void dAcOrouletteIslandR_c::finalizeState_Reduce() {}
void dAcOrouletteIslandR_c::initializeState_End() {}
void dAcOrouletteIslandR_c::executeState_End() {}
void dAcOrouletteIslandR_c::finalizeState_End() {}
