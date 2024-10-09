#include <d/a/obj/d_a_obj_barrel.h>

SPECIAL_ACTOR_PROFILE(OBJ_BARREL, dAcOBarrel_c, fProfile::OBJ_BARREL, 0x239, 0, 3);

STATE_DEFINE(dAcOBarrel_c, Wait);
STATE_DEFINE(dAcOBarrel_c, Grab);
STATE_DEFINE(dAcOBarrel_c, GrabUp);
STATE_DEFINE(dAcOBarrel_c, GrabPut);
STATE_DEFINE(dAcOBarrel_c, Slope);
STATE_DEFINE(dAcOBarrel_c, Water);
STATE_DEFINE(dAcOBarrel_c, Water2);
STATE_DEFINE(dAcOBarrel_c, Explode);
STATE_DEFINE(dAcOBarrel_c, KrakenBorn);
STATE_DEFINE(dAcOBarrel_c, Rebirth);

void dAcOBarrel_c::initializeState_Wait() {}
void dAcOBarrel_c::executeState_Wait() {}
void dAcOBarrel_c::finalizeState_Wait() {}
void dAcOBarrel_c::initializeState_Grab() {}
void dAcOBarrel_c::executeState_Grab() {}
void dAcOBarrel_c::finalizeState_Grab() {}
void dAcOBarrel_c::initializeState_GrabUp() {}
void dAcOBarrel_c::executeState_GrabUp() {}
void dAcOBarrel_c::finalizeState_GrabUp() {}
void dAcOBarrel_c::initializeState_GrabPut() {}
void dAcOBarrel_c::executeState_GrabPut() {}
void dAcOBarrel_c::finalizeState_GrabPut() {}
void dAcOBarrel_c::initializeState_Slope() {}
void dAcOBarrel_c::executeState_Slope() {}
void dAcOBarrel_c::finalizeState_Slope() {}
void dAcOBarrel_c::initializeState_Water() {}
void dAcOBarrel_c::executeState_Water() {}
void dAcOBarrel_c::finalizeState_Water() {}
void dAcOBarrel_c::initializeState_Water2() {}
void dAcOBarrel_c::executeState_Water2() {}
void dAcOBarrel_c::finalizeState_Water2() {}
void dAcOBarrel_c::initializeState_Explode() {}
void dAcOBarrel_c::executeState_Explode() {}
void dAcOBarrel_c::finalizeState_Explode() {}
void dAcOBarrel_c::initializeState_KrakenBorn() {}
void dAcOBarrel_c::executeState_KrakenBorn() {}
void dAcOBarrel_c::finalizeState_KrakenBorn() {}
void dAcOBarrel_c::initializeState_Rebirth() {}
void dAcOBarrel_c::executeState_Rebirth() {}
void dAcOBarrel_c::finalizeState_Rebirth() {}
