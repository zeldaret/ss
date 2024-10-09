#include <d/a/obj/d_a_obj_bamboo.h>

SPECIAL_ACTOR_PROFILE(OBJ_BAMBOO, dAcObamboo_c, fProfile::OBJ_BAMBOO, 0x131, 0, 35);

STATE_DEFINE(dAcObamboo_c, Wait);
STATE_DEFINE(dAcObamboo_c, Fall);
STATE_DEFINE(dAcObamboo_c, CutMark);

dAcObamboo_c::dAcObamboo_c() : mStateMgr(*this, sStateID::null) {}
dAcObamboo_c::~dAcObamboo_c() {}

void dAcObamboo_c::initializeState_Wait() {}
void dAcObamboo_c::executeState_Wait() {}
void dAcObamboo_c::finalizeState_Wait() {}
void dAcObamboo_c::initializeState_Fall() {}
void dAcObamboo_c::executeState_Fall() {}
void dAcObamboo_c::finalizeState_Fall() {}
void dAcObamboo_c::initializeState_CutMark() {}
void dAcObamboo_c::executeState_CutMark() {}
void dAcObamboo_c::finalizeState_CutMark() {}
