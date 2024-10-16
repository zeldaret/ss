#include "d/t/d_t_bcal_tag.h"

SPECIAL_ACTOR_PROFILE(T_BCAL, dTgBcArL_c, fProfile::T_BCAL, 0xC6, 0, 0);

STATE_DEFINE(dTgBcArL_c, Wait);
STATE_DEFINE(dTgBcArL_c, Create);
STATE_DEFINE(dTgBcArL_c, AttackWait);
STATE_DEFINE(dTgBcArL_c, Attack);

void dTgBcArL_c::initializeState_Wait() {}
void dTgBcArL_c::executeState_Wait() {}
void dTgBcArL_c::finalizeState_Wait() {}
void dTgBcArL_c::initializeState_Create() {}
void dTgBcArL_c::executeState_Create() {}
void dTgBcArL_c::finalizeState_Create() {}
void dTgBcArL_c::initializeState_AttackWait() {}
void dTgBcArL_c::executeState_AttackWait() {}
void dTgBcArL_c::finalizeState_AttackWait() {}
void dTgBcArL_c::initializeState_Attack() {}
void dTgBcArL_c::executeState_Attack() {}
void dTgBcArL_c::finalizeState_Attack() {}
