#include <d/a/obj/d_a_obj_water_igaiga.h>

SPECIAL_ACTOR_PROFILE(OBJ_WATER_IGAIGA, dAcOwaterIgaiga_c, fProfile::OBJ_WATER_IGAIGA, 0x1E2, 0, 7);

STATE_DEFINE(dAcOwaterIgaiga_c, Wait);
STATE_DEFINE(dAcOwaterIgaiga_c, Stick);
STATE_DEFINE(dAcOwaterIgaiga_c, Away);
STATE_DEFINE(dAcOwaterIgaiga_c, Explode);

void dAcOwaterIgaiga_c::initializeState_Wait() {}
void dAcOwaterIgaiga_c::executeState_Wait() {}
void dAcOwaterIgaiga_c::finalizeState_Wait() {}
void dAcOwaterIgaiga_c::initializeState_Stick() {}
void dAcOwaterIgaiga_c::executeState_Stick() {}
void dAcOwaterIgaiga_c::finalizeState_Stick() {}
void dAcOwaterIgaiga_c::initializeState_Away() {}
void dAcOwaterIgaiga_c::executeState_Away() {}
void dAcOwaterIgaiga_c::finalizeState_Away() {}
void dAcOwaterIgaiga_c::initializeState_Explode() {}
void dAcOwaterIgaiga_c::executeState_Explode() {}
void dAcOwaterIgaiga_c::finalizeState_Explode() {}
