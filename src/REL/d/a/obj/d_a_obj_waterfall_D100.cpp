#include <d/a/obj/d_a_obj_waterfall_D100.h>

SPECIAL_ACTOR_PROFILE(OBJ_WATERFALL_D100, dAcOwaterfallD100_c, fProfile::OBJ_WATERFALL_D100, 0x20D, 0, 6);

STATE_DEFINE(dAcOwaterfallD100_c, Wait);
STATE_DEFINE(dAcOwaterfallD100_c, Spout);
STATE_DEFINE(dAcOwaterfallD100_c, After);

void dAcOwaterfallD100_c::initializeState_Wait() {}
void dAcOwaterfallD100_c::executeState_Wait() {}
void dAcOwaterfallD100_c::finalizeState_Wait() {}
void dAcOwaterfallD100_c::initializeState_Spout() {}
void dAcOwaterfallD100_c::executeState_Spout() {}
void dAcOwaterfallD100_c::finalizeState_Spout() {}
void dAcOwaterfallD100_c::initializeState_After() {}
void dAcOwaterfallD100_c::executeState_After() {}
void dAcOwaterfallD100_c::finalizeState_After() {}
