#include "d/a/obj/d_a_obj_girahimu_floor.h"

#include "c/c_lib.h"
#include "common.h"
#include "d/a/b/d_a_b_girahimu3_first.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_plc.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_w.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_fanm.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(OBJ_GIRAHIMU_FLOOR, dAcOGirahimuFloor_c, fProfile::OBJ_GIRAHIMU_FLOOR, 0x25F, 0, 2);

STATE_DEFINE(dAcOGirahimuFloor_c, Wait);
STATE_DEFINE(dAcOGirahimuFloor_c, Return);
STATE_DEFINE(dAcOGirahimuFloor_c, Generate);
STATE_DEFINE(dAcOGirahimuFloor_c, Broken);
STATE_DEFINE(dAcOGirahimuFloor_c, Move);

static const char *sResAnmNames[3] = {
    "F403Floor",
    "F403FloorSpread",
    "F403FloorSpread2",
};

struct dAcOGirahimuFloor_HIO_c {
    const f32 field_0x00;
    const f32 field_0x04;
    const f32 field_0x08;
    static const dAcOGirahimuFloor_HIO_c sHio;
};
const dAcOGirahimuFloor_HIO_c dAcOGirahimuFloor_HIO_c::sHio = {
    1.0f,
    10.0f,
    0.4f,
};

bool dAcOGirahimuFloor_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcFile("F403Floor", "g3d/model.brres"));
    nw4r::g3d::ResMdl resMdl = mRes.GetResMdl("F403Floor");
    TRY_CREATE(mMdl.create(resMdl, &mAllocator, 0x324));

    nw4r::g3d::ResAnmTexSrt resTexSrt = mRes.GetResAnmTexSrt(sResAnmNames[0]);
    TRY_CREATE(mAnmTexSrt.create(resMdl, resTexSrt, &mAllocator, nullptr, 1));
    mMdl.setAnm(mAnmTexSrt);
    mAnmTexSrt.setRate(1.0f, 0);

    for (int i = 0; i < 3; ++i) {
        nw4r::g3d::ResAnmClr resAnmClr = mRes.GetResAnmClr(sResAnmNames[i]);
        TRY_CREATE(mAnmMatClr[i].create(resMdl, resAnmClr, &mAllocator, nullptr, 1));

        mAnmMatClr[i].setAnm(mMdl, resAnmClr, 0, m3d::PLAY_MODE_4);
        mAnmMatClr[i].setRate(0.0f, 0);
        mAnmMatClr[i].setFrame(mAnmTexSrt.getFrameMax(i) - 1.0f, 0);
        mAnmMatClr[i].setPlayMode(m3d::PLAY_MODE_1, 0);
    }

    cBgD_t *dzb = static_cast<cBgD_t *>(getOarcDZB("F403Floor", "F403Floor"));
    PLC *plc = static_cast<PLC *>(getOarcPLC("F403Floor", "F403Floor"));
    updateMatrix();

    mScaleBg0.set(0.5f, 0.5f, 0.5f);
    mScaleBg1.set(0.735f, 0.735f, 0.735f);
    TRY_CREATE(!mBg[0].Set(dzb, plc, 1, &mWorldMtx, &mScaleBg0));
    TRY_CREATE(!mBg[1].Set(dzb, plc, 1, &mWorldMtx, &mScaleBg1));
    TRY_CREATE(!mBg[2].Set(dzb, plc, 1, &mWorldMtx, &mScale));

    return true;
}

int dAcOGirahimuFloor_c::actorCreate() {
    mType = getFromParams(0, 0x3);
    if (mType >= TYPE_Max) {
        mType = TYPE_Normal;
    }

    CREATE_ALLOCATOR(dAcOGirahimuFloor_c);
    for (int i = 0; i < 3; ++i) {
        mBg[i].SetCrrFunc(dBgS_MoveBGProc_Typical);
    }
    dBgS::GetInstance()->Regist(&mBg[1], this);

    mAnmMatClr[2].setFrame(0.0f, 0);

    mAcceleration = 0.0f;
    mMaxSpeed = -40.0f;
    mMdl.setPriorityDraw(0x22, 9);
    if (mType == TYPE_Generate) {
        mMdl.setAnm(mAnmMatClr[0]);
        changeState(StateID_Generate);
    } else {
        mMdl.setAnm(mAnmMatClr[1]);
        changeState(StateID_Wait);
    }

    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    for (int i = 0; i < 3; ++i) {
        mBg[i].Move();
    }

    mBoundingBox.Set(mVec3_c(-2900.0f, -120.0f, -2900.0f), mVec3_c(3100.0f, 100.0f, 2900.0f));
    return true;
}

int dAcOGirahimuFloor_c::actorPostCreate() {
    fBase_c *pActor = nullptr;
    dAcGirahimu3First_c *pGirahim;
    while (
        (pGirahim =
             static_cast<dAcGirahimu3First_c *>(fManager_c::searchBaseByProfName(fProfile::B_GIRAHIMU3_FIRST, pActor)),
         pActor = pGirahim)
    ) {
        if (mPosition.squareDistanceToXZ(pGirahim->getPosition()) < 9000000.0f) {
            mGhirahimRef.link(pGirahim);
        }
    }

    if (mType == TYPE_Generate) {
        while ((pActor = fManager_c::searchBaseByProfName(fProfile::OBJ_GIRAHIMU_FLOOR, pActor))) {
            dAcOGirahimuFloor_c *pChild = static_cast<dAcOGirahimuFloor_c *>(pActor);
            if (pChild != this) {
                mChild.link(pChild);
                mHomePos = pChild->mHomePos;
            }
        }
    } else {
        mHomePos = mPosition;
    }

    return SUCCEEDED;
}

int dAcOGirahimuFloor_c::doDelete() {
    return SUCCEEDED;
}

int dAcOGirahimuFloor_c::actorExecute() {
    if (!isState(StateID_Broken)) {
        if (isSuccessfulFinalBlow()) {
            changeState(StateID_Broken);
            return SUCCEEDED;
        }

        if (0 == sLib::calcTimer(&field_0xABE)) {
            field_0xABE = 15;
            if (isPlayerAndGhirahimOffFloor()) {
                changeState(StateID_Broken);
                return SUCCEEDED;
            }
        }

        if (mbRequestShatter) {
            doShatter();
            return SUCCEEDED;
        }

        if (mbMediumSize) {
            mAnmMatClr[1].setRate(1.0f, 0);
            dBgS::GetInstance()->Regist(&mBg[1], this);
            dBgS::GetInstance()->Release(&mBg[0]);

            if (field_0xAC0 == 1 && mAnmMatClr[1].getFrame(0) == mAnmMatClr[1].getFrameMax(0) - 1.0f) {
                mMdl.setAnm(mAnmMatClr[2]);
                mAnmMatClr[2].setRate(1.0f, 0);

                dBgS::GetInstance()->Regist(&mBg[2], this);
                dBgS::GetInstance()->Release(&mBg[1]);
            } else {
                mMdl.setAnm(mAnmMatClr[1]);
            }
        }

        if (!isState(StateID_Move) && mbRequestMove) {
            changeState(StateID_Move);
            movePosition();
            updateMatrix();
            mMdl.setLocalMtx(mWorldMtx);
            for (int i = 0; i < 3; ++i) {
                mBg[i].Move();
            }
            return SUCCEEDED;
        }
    }

    executeState();
    mAnmTexSrt.play();
    for (int i = 0; i < 3; ++i) {
        mAnmMatClr[i].play();
    }
    if (!isState(StateID_Wait)) {
        updateMatrix();
        mMdl.setLocalMtx(mWorldMtx);
        for (int i = 0; i < 3; ++i) {
            mBg[i].Move();
        }
    } else {
        updateMatrix();
        mMdl.setScale(mScale);
        mMdl.setLocalMtx(mWorldMtx);
        for (int i = 0; i < 3; ++i) {
            mBg[i].Move();
        }
    }
    return SUCCEEDED;
}

int dAcOGirahimuFloor_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOGirahimuFloor_c::initializeState_Wait() {
    for (int i = 0; i < 3; ++i) {
        mAnmMatClr[i].setRate(0.0f, 0);
    }
}
void dAcOGirahimuFloor_c::executeState_Wait() {
    if (!mbIsHome && 0 == sLib::calcTimer(&field_0xABD)) {
        field_0xABD = 15;
        if (isGenerateNoChild()) {
            changeState(StateID_Return);
        }
    }
}
void dAcOGirahimuFloor_c::finalizeState_Wait() {}

void dAcOGirahimuFloor_c::initializeState_Return() {}
void dAcOGirahimuFloor_c::executeState_Return() {
    if (!mbRequestMove && cLib::chasePos(mPosition, mHomePos, 10.0f)) {
        changeState(StateID_Wait);
    }
}
void dAcOGirahimuFloor_c::finalizeState_Return() {
    mbIsHome = true;
}

void dAcOGirahimuFloor_c::initializeState_Generate() {
    for (int i = 0; i < 3; ++i) {
        mAnmMatClr[i].setFrame(0.0f, 0);
        if (i != 0) {
            mAnmMatClr[i].setRate(0.0f, 0);
        }
    }
    mAnmMatClr[0].setRate(1.0f, 0);
    mMdl.setAnm(mAnmMatClr[0]);
}
void dAcOGirahimuFloor_c::executeState_Generate() {
    if (mAnmMatClr[0].getFrame(0) == mAnmMatClr[0].getFrameMax(0) - 1.0f) {
        mAnmMatClr[0].setRate(0.0f, 0);
    }
    if (!mbMediumSize) {
        dBgS::GetInstance()->Regist(&mBg[0], this);
        dBgS::GetInstance()->Release(&mBg[1]);
        dBgS::GetInstance()->Release(&mBg[2]);
    }
    if (mAnmMatClr[2].isStop(0)) {
        if (mAnmMatClr[2].getFrame(0) == mAnmMatClr[2].getFrameMax(0) - 1.0f) {
            changeState(StateID_Wait);
        }
    }
}
void dAcOGirahimuFloor_c::finalizeState_Generate() {}

void dAcOGirahimuFloor_c::initializeState_Broken() {
    mAnmMatClr[2].setRate(-1.0f, 0);
    mScaleBg0.set(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 3; ++i) {
        dBgS::GetInstance()->Release(&mBg[i]);
    }
    mMdl.setAnm(mAnmMatClr[2]);
}
void dAcOGirahimuFloor_c::executeState_Broken() {
    if (mAnmMatClr[2].getFrame(0) == 0.0f) {
        mAnmMatClr[1].setRate(-1.0f, 0);
        mAnmMatClr[2].setRate(0.0f, 0);
        if (mAnmMatClr[1].getFrame(0) == 0.0f) {
            mAnmMatClr[0].setRate(-1.0f, 0);
            mAnmMatClr[1].setRate(0.0f, 0);
            mMdl.setAnm(mAnmMatClr[0]);
        } else {
            mMdl.setAnm(mAnmMatClr[1]);
        }
    }

    if (mAnmMatClr[0].getFrame(0) == 0.0f) {
        deleteRequest();
    }
}
void dAcOGirahimuFloor_c::finalizeState_Broken() {}

void dAcOGirahimuFloor_c::initializeState_Move() {}
void dAcOGirahimuFloor_c::executeState_Move() {
    movePosition();

    if (dBgS_ObjGndChk::CheckPos(mPosition)) {
        mGndHeight = dBgS_ObjGndChk::GetGroundHeight();
        if (mPosition.y <= mGndHeight) {
            doShatter();
            return;
        }
    } else {
        mPosition.y = mGndHeight;
        doShatter();
        return;
    }

    if (!mbRequestMove) {
        changeState(StateID_Wait);
    }
}
void dAcOGirahimuFloor_c::finalizeState_Move() {}

bool dAcOGirahimuFloor_c::isPlayerAndGhirahimOffFloor() {
    const dAcObjBase_c *pObj = nullptr;

    if (dBgS_ObjGndChk::CheckPos(dAcPy_c::GetLink()->mPosition + mVec3_c::Ey * 50.0f)) {
        pObj = dBgS::GetInstance()->GetActorPointer(dBgS_ObjGndChk::GetInstance());
    }
    if (pObj == this || !(pObj != nullptr && pObj->mProfileName == fProfile::OBJ_GIRAHIMU_FLOOR)) {
        return false;
    }

    pObj = nullptr;
    bool bGetObj = mGhirahimRef.isLinked();
    if (bGetObj) {
        bGetObj = dBgS_ObjGndChk::CheckPos(mGhirahimRef.get()->mPosition + mVec3_c::Ey * 50.0f);
    }
    if (bGetObj) {
        pObj = dBgS::GetInstance()->GetActorPointer(dBgS_ObjGndChk::GetInstance());
    }

    if (pObj == this || !(pObj != nullptr && pObj->mProfileName == fProfile::OBJ_GIRAHIMU_FLOOR)) {
        return false;
    }

    return true;
}

bool dAcOGirahimuFloor_c::isGenerateNoChild() {
    return mType == TYPE_Generate && !mChild.isLinked();
}

bool dAcOGirahimuFloor_c::isSuccessfulFinalBlow() {
    if (dAcPy_c::GetLink()->checkCurrentAction(96 /* FINAL_BLOW */)) {
        const dAcObjBase_c *pObj = nullptr;
        bool bGetObj = mGhirahimRef.isLinked();
        if (bGetObj) {
            bGetObj = dBgS_ObjGndChk::CheckPos(mGhirahimRef.get()->mPosition + mVec3_c::Ey * 50.0f);
        }
        if (bGetObj) {
            pObj = dBgS::GetInstance()->GetActorPointer(dBgS_ObjGndChk::GetInstance());
        }

        if (pObj != this) {
            return true;
        }
    }
    return false;
}

void dAcOGirahimuFloor_c::movePosition() {
    if (mbRequestMove && cLib::chasePos(mPosition, mTargetPos, mMoveRate)) {
        mbRequestMove = false;
    }
}

void dAcOGirahimuFloor_c::doShatter() {
    dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_876_, mPosition, nullptr, nullptr, nullptr, nullptr, 0, 0);
    startSound(SE_BGh3_GIRAHIM_STAGE_BREAK);
    for (int i = 0; i < 3; ++i) {
        dBgS::GetInstance()->Release(&mBg[i]);
    }
    deleteRequest();
}

void something() {
    static const f32 f = 9000000.0f;
}
