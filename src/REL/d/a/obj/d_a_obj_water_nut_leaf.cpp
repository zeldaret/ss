#include <d/a/obj/d_a_obj_water_nut_leaf.h>

SPECIAL_ACTOR_PROFILE(OBJ_WATER_NUT_LEAF, dAcOWaterNutLeaf_c, fProfile::OBJ_WATER_NUT_LEAF, 0x136, 0, 3);

STATE_DEFINE(dAcOWaterNutLeaf_c, Wait);
STATE_DEFINE(dAcOWaterNutLeaf_c, WaitBirth);

void dAcOWaterNutLeaf_c::initializeState_Wait() {}
void dAcOWaterNutLeaf_c::executeState_Wait() {}
void dAcOWaterNutLeaf_c::finalizeState_Wait() {}
void dAcOWaterNutLeaf_c::initializeState_WaitBirth() {}
void dAcOWaterNutLeaf_c::executeState_WaitBirth() {}
void dAcOWaterNutLeaf_c::finalizeState_WaitBirth() {}
