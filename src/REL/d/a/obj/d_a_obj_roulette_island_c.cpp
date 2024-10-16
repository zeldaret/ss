#include <d/a/obj/d_a_obj_roulette_island_c.h>

SPECIAL_ACTOR_PROFILE(OBJ_ROULETTE_ISLAND_C, dAcOrouletteIslandC_c, fProfile::OBJ_ROULETTE_ISLAND_C, 0x1CA, 0, 2);

STATE_DEFINE(dAcOrouletteIslandC_c, Wait);
STATE_DEFINE(dAcOrouletteIslandC_c, Play);

dAcOrouletteIslandC_c::dAcOrouletteIslandC_c() : mStateMgr(*this, sStateID::null) {}
dAcOrouletteIslandC_c::~dAcOrouletteIslandC_c() {}

void dAcOrouletteIslandC_c::initializeState_Wait() {}
void dAcOrouletteIslandC_c::executeState_Wait() {}
void dAcOrouletteIslandC_c::finalizeState_Wait() {}
void dAcOrouletteIslandC_c::initializeState_Play() {}
void dAcOrouletteIslandC_c::executeState_Play() {}
void dAcOrouletteIslandC_c::finalizeState_Play() {}
