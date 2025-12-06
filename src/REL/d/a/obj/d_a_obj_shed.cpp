#include "d/a/obj/d_a_obj_shed.h"

#include "c/c_lib.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_stage.h"
#include "d/flag/sceneflag_manager.h"
#include "f/f_base.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "s/s_Math.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"
#include "toBeSorted/event.h"

SPECIAL_ACTOR_PROFILE(OBJ_SHED, dAcOShed_c, fProfile::OBJ_SHED, 0x256, 0, 6);

STATE_DEFINE(dAcOShed_c, Wait);
STATE_DEFINE(dAcOShed_c, Move);

// copy from d_a_obj_fairy - TODO move it to a shared file
inline static void vecCylCalc(mVec3_c &target, const mAng &rot, f32 factor) {
    target.x += factor * rot.sin();
    target.z += factor * rot.cos();
}

bool dAcOShed_c::createHeap() {
    void *data = CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres");
    mResFile = nw4r::g3d::ResFile(data);
    dStage_c::bindSkyCmnToResFile(&mResFile);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("StageF000Shed");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x120, 1, nullptr));
    void *dzb = CurrentStageArcManager::GetInstance()->getData("dzb/StageF000Shed.dzb");
    void *plc = CurrentStageArcManager::GetInstance()->getData("dat/StageF000Shed.plc");
    updateMatrix();
    return !mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale);
}

int dAcOShed_c::create() {
    mEventId = getFromParams(0, 0xFF);
    mSceneFlag = getFromParams(8, 0xFF);
    CREATE_ALLOCATOR(dAcOShed_c);
    mBgW.SetCrrFunc(dBgS_MoveBGProc_Typical);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mAcceleration = 0.0f;
    mMaxSpeed = -40.0f;
    if (mSceneFlag < 0xFF) {
        bool flag = SceneflagManager::sInstance->checkFlag(mRoomID, mSceneFlag);
        if (flag) {
            mIsOpen = true;
            mVec = mPosition;
            vecCylCalc(mVec, mRotation.y - 0x4000, 230.0f);
            mPosition = mVec;
        }
    }
    mStateMgr.changeState(StateID_Wait);
    mBoundingBox.Set(mVec3_c(-115.0f, -0.0f, -10.0f), mVec3_c(115.0f, 260.0f, 10.0f));
    return SUCCEEDED;
}

int dAcOShed_c::doDelete() {
    return SUCCEEDED;
}

int dAcOShed_c::actorExecute() {
    mStateMgr.executeState();
    calcVelocity();
    mPosition += mVelocity;
    mPosition += mStts.mCcMove;
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mBgW.Move();
    return SUCCEEDED;
}

int dAcOShed_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOShed_c::initializeState_Wait() {}

void dAcOShed_c::executeState_Wait() {
    if (!mIsOpen && mSceneFlag < 0xFF) {
        if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneFlag)) {
            mStateMgr.changeState(StateID_Move);
        }
    }
}

void dAcOShed_c::finalizeState_Wait() {}

volatile u32 FLAGS_1 = 0x00000001;
u32 FLAGS_2 = 0x00100001;

void dAcOShed_c::initializeState_Move() {
    if (mEventId != 0xFF) {
        u32 f1 = FLAGS_1;
        u32 f2 = FLAGS_2;
        Event ev(mEventId, mRoomID, f2 & ~f1, 0, 0);
        mEvent.scheduleEvent(ev, 0);
    }
    mAngle.y = mRotation.y + 0x4000;
    mVec = mPosition;
    vecCylCalc(mVec, mAngle.y, 230.0f);
    mMoveTimer = 0x3c;
}

void dAcOShed_c::executeState_Move() {
    if (sLib::calcTimer(&mMoveTimer) == 0 && cLib::chasePosXZ(mPosition, mVec, 5.f)) {
        mStateMgr.changeState(StateID_Wait);
    }
}

void dAcOShed_c::finalizeState_Move() {
    mIsOpen = true;
}
