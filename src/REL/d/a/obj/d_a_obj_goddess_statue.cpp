#include "d/a/obj/d_a_obj_goddess_statue.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_sc_game.h"
#include "d/flag/sceneflag_manager.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/event.h"

SPECIAL_ACTOR_PROFILE(OBJ_GODDESS_STATUE, dAcOGoddessStatue_c, fProfile::OBJ_GODDESS_STATUE, 0x24A, 0, 3);

STATE_DEFINE(dAcOGoddessStatue_c, Wait);

bool dAcOGoddessStatue_c::createHeap() {
    void *data = getOarcResFile("GoddessStatue");
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("GoddessStatue");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x120, 1, nullptr));
    void *dzb = getOarcFile("GoddessStatue", "dzb/GoddessStatue.dzb");
    void *plc = getOarcFile("GoddessStatue", "dat/GoddessStatue.plc");
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.calc(true);
    mMdl.getNodeWorldMtx(0, mMtx);
    mBgWScale.set(1.f, 1.f, 1.f);
    return !mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mMtx, &mBgWScale);
}

f32 dAcOGoddessStatue_c::sSendToSwSB = 100.f;

int dAcOGoddessStatue_c::create() {
    mTransferToCrest = true;
    mExitID = getFromParams(8, 0xFF);
    mSendToSwSB = sSendToSwSB;
    CREATE_ALLOCATOR(dAcOGoddessStatue_c);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mAcceleration = -0.f;
    mMaxSpeed = -40.f;
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.calc(true);
    mSceneFlag = getFromParams(0, 0xFF);
    setCrestAtBone("SetGS", &mMdl, &mSceneFlag);
    if (mSceneFlag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneFlag)) {
        mCrestActivated = true;
    }
    mMdl.setPriorityDraw(0x1C, 9);
    mStateMgr.changeState(StateID_Wait);
    mVec3_c min, max;
    mMdl.getBounds(&min, &max);
    mBoundingBox.Set(min, max);
    return SUCCEEDED;
}

int dAcOGoddessStatue_c::doDelete() {
    return SUCCEEDED;
}

int dAcOGoddessStatue_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dAcOGoddessStatue_c::actorExecuteInEvent() {
    if (mEvent.isThisActorInEvent() && mExitID != 0xFF) {
        dScGame_c::sInstance->triggerExit(mRoomID, mExitID);
    }
    return SUCCEEDED;
}

int dAcOGoddessStatue_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOGoddessStatue_c::initializeState_Wait() {}

void dAcOGoddessStatue_c::executeState_Wait() {
    if (!mCrestActivated && mSceneFlag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneFlag)) {
        Event event("GSSceneChange", 1, 0x100001, nullptr, nullptr);
        mEvent.scheduleEvent(event, 0);
    }
}

void dAcOGoddessStatue_c::finalizeState_Wait() {}

void dAcOGoddessStatue_c::vt_0x88(f32 &param) {}
