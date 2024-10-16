#include "d/a/obj/d_a_obj_tableware.h"

SPECIAL_ACTOR_PROFILE(OBJ_TABLEWARE, dAcOTableWare_c, fProfile::OBJ_TABLEWARE, 0x138, 0, 2);

STATE_DEFINE(dAcOTableWare_c, Wait);
STATE_DEFINE(dAcOTableWare_c, Down);
STATE_DEFINE(dAcOTableWare_c, Drop);
STATE_DEFINE(dAcOTableWare_c, WindMove);

void dAcOTableWare_c::initializeState_Wait() {}
void dAcOTableWare_c::executeState_Wait() {}
void dAcOTableWare_c::finalizeState_Wait() {}
void dAcOTableWare_c::initializeState_Down() {}
void dAcOTableWare_c::executeState_Down() {}
void dAcOTableWare_c::finalizeState_Down() {}
void dAcOTableWare_c::initializeState_Drop() {}
void dAcOTableWare_c::executeState_Drop() {}
void dAcOTableWare_c::finalizeState_Drop() {}
void dAcOTableWare_c::initializeState_WindMove() {}
void dAcOTableWare_c::executeState_WindMove() {}
void dAcOTableWare_c::finalizeState_WindMove() {}
