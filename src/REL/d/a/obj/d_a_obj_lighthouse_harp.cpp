#include "d/a/obj/d_a_obj_lighthouse_harp.h"

SPECIAL_ACTOR_PROFILE(OBJ_LIGHTHOUSE_HARP, dAcOlighthouseHarp_c, fProfile::OBJ_LIGHTHOUSE_HARP, 0x1DF, 0, 2);

STATE_DEFINE(dAcOlighthouseHarp_c, Wait);
STATE_DEFINE(dAcOlighthouseHarp_c, Move);
STATE_DEFINE(dAcOlighthouseHarp_c, Appear);
STATE_DEFINE(dAcOlighthouseHarp_c, Roll);
STATE_DEFINE(dAcOlighthouseHarp_c, HarpFinish);

void dAcOlighthouseHarp_c::initializeState_Wait() {}
void dAcOlighthouseHarp_c::executeState_Wait() {}
void dAcOlighthouseHarp_c::finalizeState_Wait() {}
void dAcOlighthouseHarp_c::initializeState_Move() {}
void dAcOlighthouseHarp_c::executeState_Move() {}
void dAcOlighthouseHarp_c::finalizeState_Move() {}
void dAcOlighthouseHarp_c::initializeState_Appear() {}
void dAcOlighthouseHarp_c::executeState_Appear() {}
void dAcOlighthouseHarp_c::finalizeState_Appear() {}
void dAcOlighthouseHarp_c::initializeState_Roll() {}
void dAcOlighthouseHarp_c::executeState_Roll() {}
void dAcOlighthouseHarp_c::finalizeState_Roll() {}
void dAcOlighthouseHarp_c::initializeState_HarpFinish() {}
void dAcOlighthouseHarp_c::executeState_HarpFinish() {}
void dAcOlighthouseHarp_c::finalizeState_HarpFinish() {}
