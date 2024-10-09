#include <d/a/e/d_a_e_spark.h>

SPECIAL_ACTOR_PROFILE(E_SPARK, dAcEspark_c, fProfile::E_SPARK, 0XF9, 0, 0);

STATE_DEFINE(dAcEspark_c, Wait);
STATE_DEFINE(dAcEspark_c, EyeWait);
STATE_DEFINE(dAcEspark_c, Move);
STATE_DEFINE(dAcEspark_c, Damage);
STATE_DEFINE(dAcEspark_c, Dead);
STATE_DEFINE(dAcEspark_c, Stun);

void dAcEspark_c::initializeState_Wait() {}
void dAcEspark_c::executeState_Wait() {}
void dAcEspark_c::finalizeState_Wait() {}
void dAcEspark_c::initializeState_EyeWait() {}
void dAcEspark_c::executeState_EyeWait() {}
void dAcEspark_c::finalizeState_EyeWait() {}
void dAcEspark_c::initializeState_Move() {}
void dAcEspark_c::executeState_Move() {}
void dAcEspark_c::finalizeState_Move() {}
void dAcEspark_c::initializeState_Damage() {}
void dAcEspark_c::executeState_Damage() {}
void dAcEspark_c::finalizeState_Damage() {}
void dAcEspark_c::initializeState_Dead() {}
void dAcEspark_c::executeState_Dead() {}
void dAcEspark_c::finalizeState_Dead() {}
void dAcEspark_c::initializeState_Stun() {}
void dAcEspark_c::executeState_Stun() {}
void dAcEspark_c::finalizeState_Stun() {}
