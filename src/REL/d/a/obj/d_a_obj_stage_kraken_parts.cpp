#include "d/a/obj/d_a_obj_stage_kraken_parts.h"

SPECIAL_ACTOR_PROFILE(OBJ_STAGE_KRAKEN_PARTS, dAcOstageKrakenParts_c, fProfile::OBJ_STAGE_KRAKEN_PARTS, 0x1BF, 0, 0);

STATE_DEFINE(dAcOstageKrakenParts_c, Shake);
STATE_DEFINE(dAcOstageKrakenParts_c, Fall);

void dAcOstageKrakenParts_c::initializeState_Shake() {}
void dAcOstageKrakenParts_c::executeState_Shake() {}
void dAcOstageKrakenParts_c::finalizeState_Shake() {}
void dAcOstageKrakenParts_c::initializeState_Fall() {}
void dAcOstageKrakenParts_c::executeState_Fall() {}
void dAcOstageKrakenParts_c::finalizeState_Fall() {}
