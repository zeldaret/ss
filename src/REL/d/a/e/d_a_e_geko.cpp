#include <d/a/e/d_a_e_geko.h>

SPECIAL_ACTOR_PROFILE(E_GEKO, dAcEgeko_c, fProfile::E_GEKO, 0XED, 0, 4099);

STATE_DEFINE(dAcEgeko_c, Wait);
STATE_DEFINE(dAcEgeko_c, Move);
STATE_DEFINE(dAcEgeko_c, Search);
STATE_DEFINE(dAcEgeko_c, Shield);
STATE_DEFINE(dAcEgeko_c, Reflect);
STATE_DEFINE(dAcEgeko_c, Damage);

void dAcEgeko_c::initializeState_Wait() {}
void dAcEgeko_c::executeState_Wait() {}
void dAcEgeko_c::finalizeState_Wait() {}
void dAcEgeko_c::initializeState_Move() {}
void dAcEgeko_c::executeState_Move() {}
void dAcEgeko_c::finalizeState_Move() {}
void dAcEgeko_c::initializeState_Search() {}
void dAcEgeko_c::executeState_Search() {}
void dAcEgeko_c::finalizeState_Search() {}
void dAcEgeko_c::initializeState_Shield() {}
void dAcEgeko_c::executeState_Shield() {}
void dAcEgeko_c::finalizeState_Shield() {}
void dAcEgeko_c::initializeState_Reflect() {}
void dAcEgeko_c::executeState_Reflect() {}
void dAcEgeko_c::finalizeState_Reflect() {}
void dAcEgeko_c::initializeState_Damage() {}
void dAcEgeko_c::executeState_Damage() {}
void dAcEgeko_c::finalizeState_Damage() {}
