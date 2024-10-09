#include <d/a/e/d_a_e_battleshipfish.h>

SPECIAL_ACTOR_PROFILE(E_BFISH, dAcEbfish_c, fProfile::E_BFISH, 0XE9, 0, 4098);

STATE_DEFINE(dAcEbfish_c, Wait);
STATE_DEFINE(dAcEbfish_c, Move);
STATE_DEFINE(dAcEbfish_c, Find);
STATE_DEFINE(dAcEbfish_c, Attack);
STATE_DEFINE(dAcEbfish_c, Piyo);
STATE_DEFINE(dAcEbfish_c, NoDamage);
STATE_DEFINE(dAcEbfish_c, Dead);
STATE_DEFINE(dAcEbfish_c, BirthWait);

void dAcEbfish_c::initializeState_Wait() {}
void dAcEbfish_c::executeState_Wait() {}
void dAcEbfish_c::finalizeState_Wait() {}
void dAcEbfish_c::initializeState_Move() {}
void dAcEbfish_c::executeState_Move() {}
void dAcEbfish_c::finalizeState_Move() {}
void dAcEbfish_c::initializeState_Find() {}
void dAcEbfish_c::executeState_Find() {}
void dAcEbfish_c::finalizeState_Find() {}
void dAcEbfish_c::initializeState_Attack() {}
void dAcEbfish_c::executeState_Attack() {}
void dAcEbfish_c::finalizeState_Attack() {}
void dAcEbfish_c::initializeState_Piyo() {}
void dAcEbfish_c::executeState_Piyo() {}
void dAcEbfish_c::finalizeState_Piyo() {}
void dAcEbfish_c::initializeState_NoDamage() {}
void dAcEbfish_c::executeState_NoDamage() {}
void dAcEbfish_c::finalizeState_NoDamage() {}
void dAcEbfish_c::initializeState_Dead() {}
void dAcEbfish_c::executeState_Dead() {}
void dAcEbfish_c::finalizeState_Dead() {}
void dAcEbfish_c::initializeState_BirthWait() {}
void dAcEbfish_c::executeState_BirthWait() {}
void dAcEbfish_c::finalizeState_BirthWait() {}
