#include <d/a/e/d_a_e_magma.h>

SPECIAL_ACTOR_PROFILE(E_MAGMA, dAcEmagma_c, fProfile::E_MAGMA, 0xFA, 0, 0);

STATE_DEFINE(dAcEmagma_c, Birth);
STATE_DEFINE(dAcEmagma_c, Move);
STATE_DEFINE(dAcEmagma_c, BulletMove);
STATE_DEFINE(dAcEmagma_c, Frame);
STATE_DEFINE(dAcEmagma_c, Damage);

void dAcEmagma_c::initializeState_Birth() {}
void dAcEmagma_c::executeState_Birth() {}
void dAcEmagma_c::finalizeState_Birth() {}
void dAcEmagma_c::initializeState_Move() {}
void dAcEmagma_c::executeState_Move() {}
void dAcEmagma_c::finalizeState_Move() {}
void dAcEmagma_c::initializeState_BulletMove() {}
void dAcEmagma_c::executeState_BulletMove() {}
void dAcEmagma_c::finalizeState_BulletMove() {}
void dAcEmagma_c::initializeState_Frame() {}
void dAcEmagma_c::executeState_Frame() {}
void dAcEmagma_c::finalizeState_Frame() {}
void dAcEmagma_c::initializeState_Damage() {}
void dAcEmagma_c::executeState_Damage() {}
void dAcEmagma_c::finalizeState_Damage() {}
