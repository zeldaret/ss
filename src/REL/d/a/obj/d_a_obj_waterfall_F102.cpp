#include <d/a/obj/d_a_obj_waterfall_F102.h>

SPECIAL_ACTOR_PROFILE(OBJ_WATERFALL_F102, dAcOwaterfallF102_c, fProfile::OBJ_WATERFALL_F102, 0X203, 0, 6);

STATE_DEFINE(dAcOwaterfallF102_c, Wait);
STATE_DEFINE(dAcOwaterfallF102_c, Vanish);
STATE_DEFINE(dAcOwaterfallF102_c, Stop);

void dAcOwaterfallF102_c::initializeState_Wait() {}
void dAcOwaterfallF102_c::executeState_Wait() {}
void dAcOwaterfallF102_c::finalizeState_Wait() {}
void dAcOwaterfallF102_c::initializeState_Vanish() {}
void dAcOwaterfallF102_c::executeState_Vanish() {}
void dAcOwaterfallF102_c::finalizeState_Vanish() {}
void dAcOwaterfallF102_c::initializeState_Stop() {}
void dAcOwaterfallF102_c::executeState_Stop() {}
void dAcOwaterfallF102_c::finalizeState_Stop() {}
