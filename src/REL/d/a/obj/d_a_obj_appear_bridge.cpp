#include "d/a/obj/d_a_obj_appear_bridge.h"

#include "d/a/d_a_player.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_stage.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/types_nw4r.h"
#include "toBeSorted/area_utils.h"

SPECIAL_ACTOR_PROFILE(OBJ_APPEAR_BRIDGE, dAcOappearBridge_c, fProfile::OBJ_APPEAR_BRIDGE, 0x01FC, 0, 6);

static mVec3_c positionOffset(-1600.0f, 550.0f, 0.0f);

const f32 dAcOappearBridge_c::sMovementRate = 4.0f;

STATE_DEFINE(dAcOappearBridge_c, Wait);
STATE_DEFINE(dAcOappearBridge_c, Appear);
STATE_DEFINE(dAcOappearBridge_c, Disappear);

bool dAcOappearBridge_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(getOarcResFile("TongueStage"));
    dStage_c::bindStageResToFile(&mResFile);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("TongueStage");
    TRY_CREATE(mModel.create(mdl, &heap_allocator, 0x128));
    nw4r::g3d::ResAnmTexSrt srt = mResFile.GetResAnmTexSrt("TongueStage");
    TRY_CREATE(mSrtAnm.create(mdl, srt, &heap_allocator, nullptr, 1));
    nw4r::g3d::ResAnmClr clr = mResFile.GetResAnmClr("TongueStage");
    TRY_CREATE(mClrAnm.create(mdl, clr, &heap_allocator, nullptr, 1));
    cBgD_t *dzb = (cBgD_t *)getOarcFile("TongueStage", "dzb/TongueStage.dzb");
    PLC *plc = (PLC *)getOarcFile("TongueStage", "dat/TongueStage.plc");
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);
    TRY_CREATE(!mCollision.Set(dzb, plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mCollision.Lock();
    return true;
}

int dAcOappearBridge_c::create() {
    CREATE_ALLOCATOR(dAcOappearBridge_c);
    dBgS::GetInstance()->Regist(&mCollision, this);
    mAreaIdx = mParams & 0xFF;
    mEventId = (mParams >> 8) & 0xFF;
    mSoundPosition = mPosition + positionOffset;
    obj_pos = &mSoundPosition;
    mSceneCallback.attach(mModel);
    mModel.setAnm(mSrtAnm);
    mModel.setAnm(mClrAnm);
    field_0x65C = mSrtAnm.getFrameMax(0) - 1.0f;

    mStateMgr.changeState(StateID_Wait);
    mModel.setPriorityDraw(0x1C, 0x9);

    boundingBox.Set(mVec3_c(-3000.0f, -200.0f, -600.0f), mVec3_c(50.0f, 800.0f, 600.0f));

    return SUCCEEDED;
}

int dAcOappearBridge_c::doDelete() {
    return SUCCEEDED;
}

int dAcOappearBridge_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dAcOappearBridge_c::draw() {
    drawModelType1(&mModel);
    return SUCCEEDED;
}

void dAcOappearBridge_c::initializeState_Wait() {
    dBgS::GetInstance()->Release(&mCollision);
}
void dAcOappearBridge_c::executeState_Wait() {
    if (checkPosInArea(mAreaIdx, roomid, dAcPy_c::LINK->mPosition, nullptr)) {
        mStateMgr.changeState(StateID_Appear);
    }
}

// this works but I don't like it
volatile u32 FLAGS_1 = 0x00000001;
u32 FLAGS_2 = 0x00100001;

void dAcOappearBridge_c::finalizeState_Wait() {
    if (mEventId != 0xFF) {
        u32 f1 = FLAGS_1;
        u32 f2 = FLAGS_2;
        Event ev = Event(mEventId, roomid, f2 & ~f1, nullptr, nullptr);
        mActorEvent.scheduleEvent(ev, 0);
    }
}

void dAcOappearBridge_c::initializeState_Appear() {
    mSrtAnm.setRate(sMovementRate, 0);
    mClrAnm.setRate(sMovementRate, 0);
    dBgS::GetInstance()->Regist(&mCollision, this);
    startSound(SE_ApBridg_APPEAR_LV);
}
void dAcOappearBridge_c::executeState_Appear() {
    mClrAnm.isStop(0); // ?
    if (field_0x65C == mSrtAnm.getFrame(0)) {
        mSrtAnm.setRate(0.0f, 0);
    }
    mSrtAnm.play();
    mClrAnm.play();
    if (!checkPosInArea(mAreaIdx, roomid, dAcPy_c::LINK->mPosition, nullptr)) {
        mStateMgr.changeState(StateID_Disappear);
    }
}
void dAcOappearBridge_c::finalizeState_Appear() {}

void dAcOappearBridge_c::initializeState_Disappear() {
    mSrtAnm.setRate(-sMovementRate, 0);
    mClrAnm.setRate(-sMovementRate, 0);
}
void dAcOappearBridge_c::executeState_Disappear() {
    if (mSrtAnm.getFrame(0) == 0.0f) {
        mSrtAnm.setRate(0.0f, 0);
    }
    mSrtAnm.play();
    mClrAnm.play();
    if (checkPosInArea(mAreaIdx, roomid, dAcPy_c::LINK->mPosition, nullptr)) {
        mStateMgr.changeState(StateID_Appear);
    } else {
        if (mClrAnm.isStop(0)) {
            mStateMgr.changeState(StateID_Wait);
        }
    }
}
void dAcOappearBridge_c::finalizeState_Disappear() {}
