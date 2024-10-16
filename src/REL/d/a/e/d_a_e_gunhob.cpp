#include "d/a/e/d_a_e_gunhob.h"

SPECIAL_ACTOR_PROFILE(E_GUNHOB, dAcEgunhob_c, fProfile::E_GUNHOB, 0xE8, 0, 4130);

STATE_DEFINE(dAcEgunhob_c, BulletMove);
STATE_DEFINE(dAcEgunhob_c, BulletReflection);
STATE_DEFINE(dAcEgunhob_c, BombWait);
STATE_DEFINE(dAcEgunhob_c, BombMove);
STATE_DEFINE(dAcEgunhob_c, BombDelete);
STATE_DEFINE(dAcEgunhob_c, BombBomb);

void dAcEgunhob_c::initializeState_BulletMove() {}
void dAcEgunhob_c::executeState_BulletMove() {}
void dAcEgunhob_c::finalizeState_BulletMove() {}
void dAcEgunhob_c::initializeState_BulletReflection() {}
void dAcEgunhob_c::executeState_BulletReflection() {}
void dAcEgunhob_c::finalizeState_BulletReflection() {}
void dAcEgunhob_c::initializeState_BombWait() {}
void dAcEgunhob_c::executeState_BombWait() {}
void dAcEgunhob_c::finalizeState_BombWait() {}
void dAcEgunhob_c::initializeState_BombMove() {}
void dAcEgunhob_c::executeState_BombMove() {}
void dAcEgunhob_c::finalizeState_BombMove() {}
void dAcEgunhob_c::initializeState_BombDelete() {}
void dAcEgunhob_c::executeState_BombDelete() {}
void dAcEgunhob_c::finalizeState_BombDelete() {}
void dAcEgunhob_c::initializeState_BombBomb() {}
void dAcEgunhob_c::executeState_BombBomb() {}
void dAcEgunhob_c::finalizeState_BombBomb() {}
