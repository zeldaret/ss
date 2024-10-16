#include "d/a/obj/d_a_obj_soil.h"

SPECIAL_ACTOR_PROFILE(OBJ_SOIL, dAcOsoil_c, fProfile::OBJ_SOIL, 0x268, 0, 7);

STATE_DEFINE(dAcOsoil_c, Soil);
STATE_DEFINE(dAcOsoil_c, Hole);
STATE_DEFINE(dAcOsoil_c, Ready);

void dAcOsoil_c::initializeState_Soil() {}
void dAcOsoil_c::executeState_Soil() {}
void dAcOsoil_c::finalizeState_Soil() {}
void dAcOsoil_c::initializeState_Hole() {}
void dAcOsoil_c::executeState_Hole() {}
void dAcOsoil_c::finalizeState_Hole() {}
void dAcOsoil_c::initializeState_Ready() {}
void dAcOsoil_c::executeState_Ready() {}
void dAcOsoil_c::finalizeState_Ready() {}
