#include <d/a/d_a_insect_cicada.h>

SPECIAL_ACTOR_PROFILE(INSECT_CICADA, dAcInsectCicada_c, fProfile::INSECT_CICADA, 0X16C, 0, 2);

STATE_DEFINE(dAcInsectCicada_c, Wait);
STATE_DEFINE(dAcInsectCicada_c, Fly);
STATE_DEFINE(dAcInsectCicada_c, Finalize);
STATE_DEFINE(dAcInsectCicada_c, Dead);

void dAcInsectCicada_c::initializeState_Wait() {}
void dAcInsectCicada_c::executeState_Wait() {}
void dAcInsectCicada_c::finalizeState_Wait() {}
void dAcInsectCicada_c::initializeState_Fly() {}
void dAcInsectCicada_c::executeState_Fly() {}
void dAcInsectCicada_c::finalizeState_Fly() {}
void dAcInsectCicada_c::initializeState_Finalize() {}
void dAcInsectCicada_c::executeState_Finalize() {}
void dAcInsectCicada_c::finalizeState_Finalize() {}
void dAcInsectCicada_c::initializeState_Dead() {}
void dAcInsectCicada_c::executeState_Dead() {}
void dAcInsectCicada_c::finalizeState_Dead() {}
