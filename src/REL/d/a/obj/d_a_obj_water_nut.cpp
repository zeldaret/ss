#include <d/a/obj/d_a_obj_water_nut.h>

SPECIAL_ACTOR_PROFILE(OBJ_WATER_NUT, dAcOWaterNut_c, fProfile::OBJ_WATER_NUT, 0X137, 0, 3);

STATE_DEFINE(dAcOWaterNut_c, Wait);
STATE_DEFINE(dAcOWaterNut_c, SwordCarry);
STATE_DEFINE(dAcOWaterNut_c, Throw);
STATE_DEFINE(dAcOWaterNut_c, Drop);
STATE_DEFINE(dAcOWaterNut_c, Explode);
STATE_DEFINE(dAcOWaterNut_c, Birth);
STATE_DEFINE(dAcOWaterNut_c, Peel);

void dAcOWaterNut_c::initializeState_Wait() {}
void dAcOWaterNut_c::executeState_Wait() {}
void dAcOWaterNut_c::finalizeState_Wait() {}
void dAcOWaterNut_c::initializeState_SwordCarry() {}
void dAcOWaterNut_c::executeState_SwordCarry() {}
void dAcOWaterNut_c::finalizeState_SwordCarry() {}
void dAcOWaterNut_c::initializeState_Throw() {}
void dAcOWaterNut_c::executeState_Throw() {}
void dAcOWaterNut_c::finalizeState_Throw() {}
void dAcOWaterNut_c::initializeState_Drop() {}
void dAcOWaterNut_c::executeState_Drop() {}
void dAcOWaterNut_c::finalizeState_Drop() {}
void dAcOWaterNut_c::initializeState_Explode() {}
void dAcOWaterNut_c::executeState_Explode() {}
void dAcOWaterNut_c::finalizeState_Explode() {}
void dAcOWaterNut_c::initializeState_Birth() {}
void dAcOWaterNut_c::executeState_Birth() {}
void dAcOWaterNut_c::finalizeState_Birth() {}
void dAcOWaterNut_c::initializeState_Peel() {}
void dAcOWaterNut_c::executeState_Peel() {}
void dAcOWaterNut_c::finalizeState_Peel() {}
