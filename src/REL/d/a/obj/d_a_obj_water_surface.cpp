#include <d/a/obj/d_a_obj_water_surface.h>

SPECIAL_ACTOR_PROFILE(OBJ_WATER_SURFACE, dAcOwaterSurface_c, fProfile::OBJ_WATER_SURFACE, 0X1EC, 0, 6);

STATE_DEFINE(dAcOwaterSurface_c, Wait);
STATE_DEFINE(dAcOwaterSurface_c, Move);
STATE_DEFINE(dAcOwaterSurface_c, After1);
STATE_DEFINE(dAcOwaterSurface_c, After2);

void dAcOwaterSurface_c::initializeState_Wait() {}
void dAcOwaterSurface_c::executeState_Wait() {}
void dAcOwaterSurface_c::finalizeState_Wait() {}
void dAcOwaterSurface_c::initializeState_Move() {}
void dAcOwaterSurface_c::executeState_Move() {}
void dAcOwaterSurface_c::finalizeState_Move() {}
void dAcOwaterSurface_c::initializeState_After1() {}
void dAcOwaterSurface_c::executeState_After1() {}
void dAcOwaterSurface_c::finalizeState_After1() {}
void dAcOwaterSurface_c::initializeState_After2() {}
void dAcOwaterSurface_c::executeState_After2() {}
void dAcOwaterSurface_c::finalizeState_After2() {}
