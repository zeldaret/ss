#include <d/a/d_a_asura_bullet.h>

SPECIAL_ACTOR_PROFILE(ASURA_BULLET, dAcAsuraBullet_c, fProfile::ASURA_BULLET, 0X11A, 0, 0);

STATE_DEFINE(dAcAsuraBullet_c, Attack);
STATE_DEFINE(dAcAsuraBullet_c, Wait);

void dAcAsuraBullet_c::initializeState_Attack() {}
void dAcAsuraBullet_c::executeState_Attack() {}
void dAcAsuraBullet_c::finalizeState_Attack() {}
void dAcAsuraBullet_c::initializeState_Wait() {}
void dAcAsuraBullet_c::executeState_Wait() {}
void dAcAsuraBullet_c::finalizeState_Wait() {}
