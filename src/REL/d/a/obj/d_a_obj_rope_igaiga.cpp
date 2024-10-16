#include "d/a/obj/d_a_obj_rope_igaiga.h"

SPECIAL_ACTOR_PROFILE(OBJ_ROPE_IGAIGA, dAcOropeIgaiga_c, fProfile::OBJ_ROPE_IGAIGA, 0x265, 0, 2);

STATE_DEFINE(dAcOropeIgaiga_c, Init);
STATE_DEFINE(dAcOropeIgaiga_c, Rope);
STATE_DEFINE(dAcOropeIgaiga_c, Ground);
STATE_DEFINE(dAcOropeIgaiga_c, Tree);
STATE_DEFINE(dAcOropeIgaiga_c, Stick);
STATE_DEFINE(dAcOropeIgaiga_c, Water);

void dAcOropeIgaiga_c::initializeState_Init() {}
void dAcOropeIgaiga_c::executeState_Init() {}
void dAcOropeIgaiga_c::finalizeState_Init() {}
void dAcOropeIgaiga_c::initializeState_Rope() {}
void dAcOropeIgaiga_c::executeState_Rope() {}
void dAcOropeIgaiga_c::finalizeState_Rope() {}
void dAcOropeIgaiga_c::initializeState_Ground() {}
void dAcOropeIgaiga_c::executeState_Ground() {}
void dAcOropeIgaiga_c::finalizeState_Ground() {}
void dAcOropeIgaiga_c::initializeState_Tree() {}
void dAcOropeIgaiga_c::executeState_Tree() {}
void dAcOropeIgaiga_c::finalizeState_Tree() {}
void dAcOropeIgaiga_c::initializeState_Stick() {}
void dAcOropeIgaiga_c::executeState_Stick() {}
void dAcOropeIgaiga_c::finalizeState_Stick() {}
void dAcOropeIgaiga_c::initializeState_Water() {}
void dAcOropeIgaiga_c::executeState_Water() {}
void dAcOropeIgaiga_c::finalizeState_Water() {}
