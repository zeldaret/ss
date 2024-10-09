#include <d/a/obj/d_a_obj_cannon_cover.h>

SPECIAL_ACTOR_PROFILE(OBJ_CANNON_COVER, dAcOcannonCover_c, fProfile::OBJ_CANNON_COVER, 0X1D3, 0, 4);

STATE_DEFINE(dAcOcannonCover_c, Wait);
STATE_DEFINE(dAcOcannonCover_c, Open);
STATE_DEFINE(dAcOcannonCover_c, Shake);
STATE_DEFINE(dAcOcannonCover_c, Close);
STATE_DEFINE(dAcOcannonCover_c, Play);

dAcOcannonCover_c::dAcOcannonCover_c() : mStateMgr(*this, sStateID::null) {}
dAcOcannonCover_c::~dAcOcannonCover_c() {}

void dAcOcannonCover_c::initializeState_Wait() {}
void dAcOcannonCover_c::executeState_Wait() {}
void dAcOcannonCover_c::finalizeState_Wait() {}
void dAcOcannonCover_c::initializeState_Open() {}
void dAcOcannonCover_c::executeState_Open() {}
void dAcOcannonCover_c::finalizeState_Open() {}
void dAcOcannonCover_c::initializeState_Shake() {}
void dAcOcannonCover_c::executeState_Shake() {}
void dAcOcannonCover_c::finalizeState_Shake() {}
void dAcOcannonCover_c::initializeState_Close() {}
void dAcOcannonCover_c::executeState_Close() {}
void dAcOcannonCover_c::finalizeState_Close() {}
void dAcOcannonCover_c::initializeState_Play() {}
void dAcOcannonCover_c::executeState_Play() {}
void dAcOcannonCover_c::finalizeState_Play() {}
