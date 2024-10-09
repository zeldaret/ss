#include <d/a/obj/d_a_obj_clef.h>

SPECIAL_ACTOR_PROFILE(OBJ_CLEF, dAcOClef_c, fProfile::OBJ_CLEF, 0x2D, 0, 7);

STATE_DEFINE(dAcOClef_c, WaitPhantom);
STATE_DEFINE(dAcOClef_c, Wait);
STATE_DEFINE(dAcOClef_c, WaitPathMove);
STATE_DEFINE(dAcOClef_c, MoveTowardPath);
STATE_DEFINE(dAcOClef_c, PathMove);
STATE_DEFINE(dAcOClef_c, Gravitate);
STATE_DEFINE(dAcOClef_c, Die);

dAcOClef_c::dAcOClef_c() : mStateMgr(*this, sStateID::null) {}
dAcOClef_c::~dAcOClef_c() {}

void dAcOClef_c::initializeState_WaitPhantom() {}
void dAcOClef_c::executeState_WaitPhantom() {}
void dAcOClef_c::finalizeState_WaitPhantom() {}
void dAcOClef_c::initializeState_Wait() {}
void dAcOClef_c::executeState_Wait() {}
void dAcOClef_c::finalizeState_Wait() {}
void dAcOClef_c::initializeState_WaitPathMove() {}
void dAcOClef_c::executeState_WaitPathMove() {}
void dAcOClef_c::finalizeState_WaitPathMove() {}
void dAcOClef_c::initializeState_MoveTowardPath() {}
void dAcOClef_c::executeState_MoveTowardPath() {}
void dAcOClef_c::finalizeState_MoveTowardPath() {}
void dAcOClef_c::initializeState_PathMove() {}
void dAcOClef_c::executeState_PathMove() {}
void dAcOClef_c::finalizeState_PathMove() {}
void dAcOClef_c::initializeState_Gravitate() {}
void dAcOClef_c::executeState_Gravitate() {}
void dAcOClef_c::finalizeState_Gravitate() {}
void dAcOClef_c::initializeState_Die() {}
void dAcOClef_c::executeState_Die() {}
void dAcOClef_c::finalizeState_Die() {}
