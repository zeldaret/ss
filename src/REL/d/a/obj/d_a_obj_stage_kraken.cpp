#include <d/a/obj/d_a_obj_stage_kraken.h>

SPECIAL_ACTOR_PROFILE(OBJ_STAGE_KRAKEN, dAcOstageKraken_c, fProfile::OBJ_STAGE_KRAKEN, 0X1BE, 0, 0);

STATE_DEFINE(dAcOstageKraken_c, Wait);
STATE_DEFINE(dAcOstageKraken_c, MarkLight);
STATE_DEFINE(dAcOstageKraken_c, AroundMarkLight);
STATE_DEFINE(dAcOstageKraken_c, MarkVanish);

void dAcOstageKraken_c::initializeState_Wait() {}
void dAcOstageKraken_c::executeState_Wait() {}
void dAcOstageKraken_c::finalizeState_Wait() {}
void dAcOstageKraken_c::initializeState_MarkLight() {}
void dAcOstageKraken_c::executeState_MarkLight() {}
void dAcOstageKraken_c::finalizeState_MarkLight() {}
void dAcOstageKraken_c::initializeState_AroundMarkLight() {}
void dAcOstageKraken_c::executeState_AroundMarkLight() {}
void dAcOstageKraken_c::finalizeState_AroundMarkLight() {}
void dAcOstageKraken_c::initializeState_MarkVanish() {}
void dAcOstageKraken_c::executeState_MarkVanish() {}
void dAcOstageKraken_c::finalizeState_MarkVanish() {}
