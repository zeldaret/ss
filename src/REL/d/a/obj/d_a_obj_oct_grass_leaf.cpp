#include <d/a/obj/d_a_obj_oct_grass_leaf.h>

SPECIAL_ACTOR_PROFILE(OBJ_OCT_GRASS_LEAF, dAcOOctGrassLeaf_c, fProfile::OBJ_OCT_GRASS_LEAF, 0x237, 0, 3);

STATE_DEFINE(dAcOOctGrassLeaf_c, Wait);
STATE_DEFINE(dAcOOctGrassLeaf_c, Shake);
STATE_DEFINE(dAcOOctGrassLeaf_c, Touch);
STATE_DEFINE(dAcOOctGrassLeaf_c, Grab);
STATE_DEFINE(dAcOOctGrassLeaf_c, Broken);

void dAcOOctGrassLeaf_c::initializeState_Wait() {}
void dAcOOctGrassLeaf_c::executeState_Wait() {}
void dAcOOctGrassLeaf_c::finalizeState_Wait() {}
void dAcOOctGrassLeaf_c::initializeState_Shake() {}
void dAcOOctGrassLeaf_c::executeState_Shake() {}
void dAcOOctGrassLeaf_c::finalizeState_Shake() {}
void dAcOOctGrassLeaf_c::initializeState_Touch() {}
void dAcOOctGrassLeaf_c::executeState_Touch() {}
void dAcOOctGrassLeaf_c::finalizeState_Touch() {}
void dAcOOctGrassLeaf_c::initializeState_Grab() {}
void dAcOOctGrassLeaf_c::executeState_Grab() {}
void dAcOOctGrassLeaf_c::finalizeState_Grab() {}
void dAcOOctGrassLeaf_c::initializeState_Broken() {}
void dAcOOctGrassLeaf_c::executeState_Broken() {}
void dAcOOctGrassLeaf_c::finalizeState_Broken() {}
