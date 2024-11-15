#include "d/a/obj/d_a_obj_tbox.h"
#include "m/m_vec.h"
#include "toBeSorted/dowsing_target.h"

SPECIAL_ACTOR_PROFILE(TBOX, dAcTbox_c, fProfile::TBOX, 0x018D, 0, 6);

extern "C" mVec3_c vecs[] = {
    mVec3_c(0.0f, 54.0f, 0.0f),
    mVec3_c(0.0f, 34.0f, 0.0f),
    mVec3_c(0.0f, 66.5f, 0.0f),
    mVec3_c(0.0f, 49.0f, 0.0f)
};

extern "C" mVec3_c v1 = mVec3_c(0.0f, 0.0f, -2.0f);
extern "C" mVec3_c v2 = mVec3_c(1.0f, 1.0f, 0.8f);
extern "C" mVec3_c v3 = mVec3_c(1.0f, 1.0f, 1.0f);
extern "C" mVec3_c v4 = mVec3_c(0.0f, 0.0f, 88.0f);

STATE_DEFINE(dAcTbox_c, DugOut);
STATE_DEFINE(dAcTbox_c, WaitAppear);
STATE_DEFINE(dAcTbox_c, DemoAppear);
STATE_DEFINE(dAcTbox_c, WaitOpen);
STATE_DEFINE(dAcTbox_c, GoddessWaitOff);
STATE_DEFINE(dAcTbox_c, GoddessWaitOn);
STATE_DEFINE(dAcTbox_c, DeleteArchive);
STATE_DEFINE(dAcTbox_c, LoadArchive);
STATE_DEFINE(dAcTbox_c, Open);
STATE_DEFINE(dAcTbox_c, PresentItem);
STATE_DEFINE(dAcTbox_c, Close);
STATE_DEFINE(dAcTbox_c, Wait);
STATE_DEFINE(dAcTbox_c, GoddessWait);

fLiMgBa_c dAcTbox_c::sTboxActorList;

dAcTbox_c::dAcTbox_c() : mStateMgr(*this, sStateID::null), mScnCallback(this), mEvent(*this, nullptr), mTboxListNode(this), mDowsingTarget1(this, DowsingTarget::SLOT_NONE), mDowsingTarget2(this, DowsingTarget::SLOT_NONE) {
    sTboxActorList.append(&mTboxListNode);
    unkByteTargetFiRelated = 2;
}
dAcTbox_c::~dAcTbox_c() {
    sTboxActorList.remove(&mTboxListNode);
}

void dAcTbox_c::initializeState_DugOut() {}
void dAcTbox_c::executeState_DugOut() {}
void dAcTbox_c::finalizeState_DugOut() {}
void dAcTbox_c::initializeState_WaitAppear() {}
void dAcTbox_c::executeState_WaitAppear() {}
void dAcTbox_c::finalizeState_WaitAppear() {}
void dAcTbox_c::initializeState_DemoAppear() {}
void dAcTbox_c::executeState_DemoAppear() {}
void dAcTbox_c::finalizeState_DemoAppear() {}
void dAcTbox_c::initializeState_WaitOpen() {}
void dAcTbox_c::executeState_WaitOpen() {}
void dAcTbox_c::finalizeState_WaitOpen() {}
void dAcTbox_c::initializeState_GoddessWaitOff() {}
void dAcTbox_c::executeState_GoddessWaitOff() {}
void dAcTbox_c::finalizeState_GoddessWaitOff() {}
void dAcTbox_c::initializeState_GoddessWaitOn() {}
void dAcTbox_c::executeState_GoddessWaitOn() {}
void dAcTbox_c::finalizeState_GoddessWaitOn() {}
void dAcTbox_c::initializeState_DeleteArchive() {}
void dAcTbox_c::executeState_DeleteArchive() {}
void dAcTbox_c::finalizeState_DeleteArchive() {}
void dAcTbox_c::initializeState_LoadArchive() {}
void dAcTbox_c::executeState_LoadArchive() {}
void dAcTbox_c::finalizeState_LoadArchive() {}
void dAcTbox_c::initializeState_Open() {}
void dAcTbox_c::executeState_Open() {}
void dAcTbox_c::finalizeState_Open() {}
void dAcTbox_c::initializeState_PresentItem() {}
void dAcTbox_c::executeState_PresentItem() {}
void dAcTbox_c::finalizeState_PresentItem() {}
void dAcTbox_c::initializeState_Close() {}
void dAcTbox_c::executeState_Close() {}
void dAcTbox_c::finalizeState_Close() {}
void dAcTbox_c::initializeState_Wait() {}
void dAcTbox_c::executeState_Wait() {}
void dAcTbox_c::finalizeState_Wait() {}
void dAcTbox_c::initializeState_GoddessWait() {}
void dAcTbox_c::executeState_GoddessWait() {}
void dAcTbox_c::finalizeState_GoddessWait() {}
