#include "d/a/obj/d_a_obj_chandelier.h"

SPECIAL_ACTOR_PROFILE(OBJ_CHANDELIER, dAcOChandelier_c, fProfile::OBJ_CHANDELIER, 0x14F, 0, 2);

STATE_DEFINE(dAcOChandelier_c, Wait);
STATE_DEFINE(dAcOChandelier_c, Fall);
STATE_DEFINE(dAcOChandelier_c, End);

void dAcOChandelier_c::initializeState_Wait() {}
void dAcOChandelier_c::executeState_Wait() {}
void dAcOChandelier_c::finalizeState_Wait() {}
void dAcOChandelier_c::initializeState_Fall() {}
void dAcOChandelier_c::executeState_Fall() {}
void dAcOChandelier_c::finalizeState_Fall() {}
void dAcOChandelier_c::initializeState_End() {}
void dAcOChandelier_c::executeState_End() {}
void dAcOChandelier_c::finalizeState_End() {}
