#include "d/a/obj/d_a_obj_blow_coal.h"

SPECIAL_ACTOR_PROFILE(OBJ_BLOW_COAL, dAcOblowCoal_c, fProfile::OBJ_BLOW_COAL, 0x1CD, 0, 3);

STATE_DEFINE(dAcOblowCoal_c, Wait);
STATE_DEFINE(dAcOblowCoal_c, Dead);

void dAcOblowCoal_c::initializeState_Wait() {}
void dAcOblowCoal_c::executeState_Wait() {}
void dAcOblowCoal_c::finalizeState_Wait() {}
void dAcOblowCoal_c::initializeState_Dead() {}
void dAcOblowCoal_c::executeState_Dead() {}
void dAcOblowCoal_c::finalizeState_Dead() {}
