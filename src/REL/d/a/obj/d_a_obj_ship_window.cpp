#include "d/a/obj/d_a_obj_ship_window.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/event.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/time_area_mgr.h"

SPECIAL_ACTOR_PROFILE(OBJ_SHIP_WINDOW, dAcOshipWindow_c, fProfile::OBJ_SHIP_WINDOW, 0x1EB, 0, 6);

// mMoveDist is never read, the code repeats it as a literal
struct dAcOshipWindow_HIO_c {
    /* 0x00 */ f32 mMoveDist;
    /* 0x04 */ s16 mMoveTime;

    static const dAcOshipWindow_HIO_c sHio;
};

const dAcOshipWindow_HIO_c dAcOshipWindow_HIO_c::sHio = {300.0f, 20};

STATE_DEFINE(dAcOshipWindow_c, Close);
STATE_DEFINE(dAcOshipWindow_c, Move);
STATE_DEFINE(dAcOshipWindow_c, Open);

bool dAcOshipWindow_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("D301Window"));
    nw4r::g3d::ResMdl mdl = mRes.GetResMdl("D301Window_N");
    TRY_CREATE(mMdls[0].create(mdl, &mAllocator, 0x120, 1, nullptr));
    mdl = mRes.GetResMdl("D301Window_T");
    TRY_CREATE(mMdls[1].create(mdl, &mAllocator, 0x120, 1, nullptr));
    void *dzb = getOarcFile("D301Window", "dzb/D301WindowN.dzb");
    void *plc = getOarcFile("D301Window", "dat/D301WindowN.plc");
    updateMatrix();
    return !mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale);
}

int dAcOshipWindow_c::actorCreate() {
    CREATE_ALLOCATOR(dAcOshipWindow_c);
    mBgW.SetCrrFunc(dBgS_MoveBGProc_Typical);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mOpenSceneflag = getFromParams(0, 0xFF);
    mEventDoneSceneflag = mRotation.z;
    mOpenEventId = getFromParams(8, 0xFF);
    mCloseEventId = getFromParams(16, 0xFF);
    mEventOnce = getFromParams(24, 0x1);
    mMaxSpeed = -40.0f;
    mRotation.z.set(0);
    mMdls[1].setPriorityDraw(0xF, 0);
    mMdls[0].setPriorityDraw(0xB, 0);
    mBoundingBox.Set(mVec3_c(-150.0f, -0.0f, -25.0f), mVec3_c(150.0f, 300.0f, 25.0f));
    return SUCCEEDED;
}

int dAcOshipWindow_c::actorPostCreate() {
    if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mOpenSceneflag)) {
        mPosition.y += 300.0f;
        mStateMgr.changeState(StateID_Open);
    } else {
        mStateMgr.changeState(StateID_Close);
    }
    return SUCCEEDED;
}

int dAcOshipWindow_c::doDelete() {
    return SUCCEEDED;
}

int dAcOshipWindow_c::actorExecute() {
    mPastState = dTimeAreaMgr_c::GetInstance()->checkPositionIsInPastState(mRoomID, mPosition, nullptr, 100.0f);
    mStateMgr.executeState();
    calcVelocity();
    mPosition += mVelocity;
    mPosition += mStts.GetCcMove();
    updateMatrix();
    for (int i = 0; i < 2; i++) {
        mMdls[i].setLocalMtx(mWorldMtx);
    }
    mBgW.Move();
    return SUCCEEDED;
}

int dAcOshipWindow_c::actorExecuteInEvent() {
    switch (mEvent.getCurrentEventCommand()) {
        case 'act0': mEvent.advanceNext(); break;
        case 'act1':
            actorExecute();
            if (mStateMgr.isState(StateID_Open) || mStateMgr.isState(StateID_Close)) {
                mEvent.advanceNext();
            }
            break;
        case '????': actorExecute(); break;
        default:     actorExecute(); break;
    }
    return SUCCEEDED;
}

int dAcOshipWindow_c::draw() {
    if (mPastState <= 0.0f ||
        (dTimeAreaMgr_c::GetInstance()->isField0x7A() && !dTimeAreaMgr_c::GetInstance()->isField0x78())) {
        drawModelType1(&mMdls[0]);
    }
    if (mPastState > 0.0f) {
        drawModelType1(&mMdls[1]);
    }
    return SUCCEEDED;
}

void dAcOshipWindow_c::initializeState_Close() {}
void dAcOshipWindow_c::executeState_Close() {
    if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mOpenSceneflag)) {
        if ((!mOpenEventDone || !mEventOnce) &&
            !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mEventDoneSceneflag)) {
            if (mOpenEventId != 0xFF) {
                static volatile u32 FLAGS_1 = 0x00000001;
                static u32 FLAGS_2 = 0x00100001;
                {
                    u32 f1 = FLAGS_1;
                    u32 f2 = FLAGS_2;
                    Event event(mOpenEventId, mRoomID, f2 & ~f1, nullptr, nullptr);
                    mEvent.scheduleEvent(event, 0);
                }
                if (EventManager::isInEvent(this, nullptr)) {
                    mOpenEventDone = true;
                    SceneflagManager::sInstance->setFlag(mRoomID, mEventDoneSceneflag);
                    mStateMgr.changeState(StateID_Move);
                }
            } else {
                mStateMgr.changeState(StateID_Move);
            }
        } else {
            mStateMgr.changeState(StateID_Move);
        }
    }
}
void dAcOshipWindow_c::finalizeState_Close() {
    startSound(SE_ShipWin_Open);
}

void dAcOshipWindow_c::initializeState_Move() {
    mMoveTimer = dAcOshipWindow_HIO_c::sHio.mMoveTime;
    if (*mStateMgr.getOldStateID() == StateID_Close) {
        mVelocity.y = 300.0f / mMoveTimer;
    } else {
        mVelocity.y = -300.0f / mMoveTimer;
    }
}
void dAcOshipWindow_c::executeState_Move() {
    if (sLib::calcTimer(&mMoveTimer) == 0) {
        if (*mStateMgr.getOldStateID() == StateID_Close) {
            mStateMgr.changeState(StateID_Open);
        } else {
            mStateMgr.changeState(StateID_Close);
        }
    }
}
void dAcOshipWindow_c::finalizeState_Move() {
    mVelocity.y = 0.0f;
}

void dAcOshipWindow_c::initializeState_Open() {}
void dAcOshipWindow_c::executeState_Open() {
    if (!SceneflagManager::sInstance->checkBoolFlag(mRoomID, mOpenSceneflag)) {
        if ((!mCloseEventDone || !mEventOnce) &&
            (mEventDoneSceneflag >= 0xFF ||
             !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mEventDoneSceneflag + 1))) {
            if (mCloseEventId != 0xFF) {
                static volatile u32 FLAGS_1 = 0x00000001;
                static u32 FLAGS_2 = 0x00100001;
                {
                    u32 f1 = FLAGS_1;
                    u32 f2 = FLAGS_2;
                    Event event(mCloseEventId, mRoomID, f2 & ~f1, nullptr, nullptr);
                    mEvent.scheduleEvent(event, 0);
                }
                if (EventManager::isInEvent(this, nullptr)) {
                    mCloseEventDone = true;
                    if (mEventDoneSceneflag < 0xFF) {
                        SceneflagManager::sInstance->setFlag(mRoomID, mEventDoneSceneflag + 1);
                    }
                    mStateMgr.changeState(StateID_Move);
                }
            } else {
                mStateMgr.changeState(StateID_Move);
            }
        } else {
            mStateMgr.changeState(StateID_Move);
        }
    }
}
void dAcOshipWindow_c::finalizeState_Open() {
    startSound(SE_ShipWin_Close);
}
