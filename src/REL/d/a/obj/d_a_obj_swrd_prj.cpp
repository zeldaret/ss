#include <d/a/obj/d_a_obj_swrd_prj.h>

SPECIAL_ACTOR_PROFILE(OBJ_SWRD_PRJ, dAcOSwrdPrj, fProfile::OBJ_SWRD_PRJ, 0X27D, 0, 3);

STATE_DEFINE(dAcOSwrdPrj, WaitHarp);
STATE_DEFINE(dAcOSwrdPrj, WaitAppear);
STATE_DEFINE(dAcOSwrdPrj, Wait);
STATE_DEFINE(dAcOSwrdPrj, WaitCross);
STATE_DEFINE(dAcOSwrdPrj, ChkTri);
STATE_DEFINE(dAcOSwrdPrj, End);
STATE_DEFINE(dAcOSwrdPrj, SWWait);
STATE_DEFINE(dAcOSwrdPrj, SWWaitCross);
STATE_DEFINE(dAcOSwrdPrj, SWChkTri);
STATE_DEFINE(dAcOSwrdPrj, SWEnd);

dAcOSwrdPrj::dAcOSwrdPrj() : mStateMgr(*this, sStateID::null) {}
dAcOSwrdPrj::~dAcOSwrdPrj() {}

void dAcOSwrdPrj::initializeState_WaitHarp() {}
void dAcOSwrdPrj::executeState_WaitHarp() {}
void dAcOSwrdPrj::finalizeState_WaitHarp() {}
void dAcOSwrdPrj::initializeState_WaitAppear() {}
void dAcOSwrdPrj::executeState_WaitAppear() {}
void dAcOSwrdPrj::finalizeState_WaitAppear() {}
void dAcOSwrdPrj::initializeState_Wait() {}
void dAcOSwrdPrj::executeState_Wait() {}
void dAcOSwrdPrj::finalizeState_Wait() {}
void dAcOSwrdPrj::initializeState_WaitCross() {}
void dAcOSwrdPrj::executeState_WaitCross() {}
void dAcOSwrdPrj::finalizeState_WaitCross() {}
void dAcOSwrdPrj::initializeState_ChkTri() {}
void dAcOSwrdPrj::executeState_ChkTri() {}
void dAcOSwrdPrj::finalizeState_ChkTri() {}
void dAcOSwrdPrj::initializeState_End() {}
void dAcOSwrdPrj::executeState_End() {}
void dAcOSwrdPrj::finalizeState_End() {}
void dAcOSwrdPrj::initializeState_SWWait() {}
void dAcOSwrdPrj::executeState_SWWait() {}
void dAcOSwrdPrj::finalizeState_SWWait() {}
void dAcOSwrdPrj::initializeState_SWWaitCross() {}
void dAcOSwrdPrj::executeState_SWWaitCross() {}
void dAcOSwrdPrj::finalizeState_SWWaitCross() {}
void dAcOSwrdPrj::initializeState_SWChkTri() {}
void dAcOSwrdPrj::executeState_SWChkTri() {}
void dAcOSwrdPrj::finalizeState_SWChkTri() {}
void dAcOSwrdPrj::initializeState_SWEnd() {}
void dAcOSwrdPrj::executeState_SWEnd() {}
void dAcOSwrdPrj::finalizeState_SWEnd() {}
