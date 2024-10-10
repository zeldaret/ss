#include <d/a/obj/d_a_obj_lighthouse_light.h>

SPECIAL_ACTOR_PROFILE(OBJ_LIGHTHOUSE_LIGHT, dAcOlighthouseLight_c, fProfile::OBJ_LIGHTHOUSE_LIGHT, 0x1DE, 0, 0);

STATE_DEFINE(dAcOlighthouseLight_c, Wait);
STATE_DEFINE(dAcOlighthouseLight_c, Move);
STATE_DEFINE(dAcOlighthouseLight_c, Appear);

void dAcOlighthouseLight_c::initializeState_Wait() {}
void dAcOlighthouseLight_c::executeState_Wait() {}
void dAcOlighthouseLight_c::finalizeState_Wait() {}
void dAcOlighthouseLight_c::initializeState_Move() {}
void dAcOlighthouseLight_c::executeState_Move() {}
void dAcOlighthouseLight_c::finalizeState_Move() {}
void dAcOlighthouseLight_c::initializeState_Appear() {}
void dAcOlighthouseLight_c::executeState_Appear() {}
void dAcOlighthouseLight_c::finalizeState_Appear() {}
