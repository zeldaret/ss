#include "d/a/obj/d_a_obj_stone_stand.h"

#include "c/c_lib.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_rumble.h"
#include "d/d_sc_game.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "f/f_profile.h"
#include "m/m3d/m_smdl.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/math/math_triangular.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/event.h"

SPECIAL_ACTOR_PROFILE(OBJ_STONE_STAND, dAcOStoneStand_c, fProfile::OBJ_STONE_STAND, 0x24B, 0, 3);

STATE_DEFINE(dAcOStoneStand_c, Wait);
STATE_DEFINE(dAcOStoneStand_c, Shake);
STATE_DEFINE(dAcOStoneStand_c, OnSwitch);

char *dAcOStoneStand_c::sResMdlAnmNames[3] = {"SekibanMapADemo", "SekibanMapBDemo", "SekibanMapCDemo"};

const InteractionTargetDef dAcOStoneStand_c::sInteraction1 = {
    0, 3, 0, EXAMINE_TALK, 0, 250.f, 60.f, 60.f, -50.f, 150.f, 50.f, 1.f,
};

const InteractionTargetDef dAcOStoneStand_c::sInteraction2 = {
    1, 3, 0, EXAMINE_TALK, 0, 250.f, 60.f, 60.f, -50.f, 150.f, 50.f, 1.f,
};

bool dAcOStoneStand_c::createHeap() {
    void *data = getOarcResFile("LithographyStand");
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("LithographyStand");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x120, 1, nullptr));
    for (s32 i = 0; i < 3; i++) {
        mdl = mResFile.GetResMdl(sResMdlAnmNames[i]);
        TRY_CREATE(mTabletMdls[i].create(mdl, &mAllocator, 0x128));
        nw4r::g3d::ResAnmClr anm = mResFile.GetResAnmClr(sResMdlAnmNames[i]);
        mAnm[i].create(mdl, anm, &mAllocator, nullptr, 1);
        mTabletMdls[i].setAnm(mAnm[i]);
        mAnm[i].setRate(0.f, 0);
        f32 frameMax = mAnm[i].getFrameMax(0);
        mAnm[i].setFrame(frameMax - 1.f, 0);
    }
    void *dzb = getOarcFile("LithographyStand", "dzb/LithographyStand.dzb");
    void *plc = getOarcFile("LithographyStand", "dat/LithographyStand.plc");
    updateMatrix();
    return !mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale);
}

int dAcOStoneStand_c::create() {
    CREATE_ALLOCATOR(dAcOStoneStand_c);
    mBgW.SetCrrFunc(dBgS_MoveBGProc_Typical);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mActivatedSceneflag = getFromParams(0, 0xFF);
    mPillarCSExitOffset = getFromParams(8, 0xFF);
    field_0x78C = mPosition;
    if (mActivatedSceneflag >= 0xFF || !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mActivatedSceneflag)) {
        mPosition.y -= 204.f;
        mOldPosition.y -= 204.f;
    }
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.calc(true);
    if (dAcItem_c::checkFlag(ITEM_AMBER_TABLET)) {
        mTabletCSIndex = 2;
        mVisibleTabletState = 2;
    } else if (dAcItem_c::checkFlag(ITEM_RUBY_TABLET)) {
        mTabletCSIndex = 1;
        mVisibleTabletState = 1;
    } else {
        mTabletCSIndex = 0;
        mVisibleTabletState = 0;
    }
    if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_LANAYRU_PILLAR_OPENED)) {
        mOpenedLightPillarState = 3;
    } else if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_ELDIN_PILLAR_OPENED)) {
        mOpenedLightPillarState = 2;
    } else if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_FARON_PILLAR_OPENED)) {
        mOpenedLightPillarState = 1;
    } else {
        mOpenedLightPillarState = 0;
    }

    if (mOpenedLightPillarState < 3) {
        mAnm[mOpenedLightPillarState].setFrame(0.f, 0);
    }
    nw4r::g3d::ResMdl mdl = mMdl.getResMdl();
    mLocatorBones[0] = mdl.GetResNode("locator_A").GetID();
    mdl = mMdl.getResMdl();
    mLocatorBones[1] = mdl.GetResNode("locator_B").GetID();
    mdl = mMdl.getResMdl();
    mLocatorBones[2] = mdl.GetResNode("locator_C").GetID();
    for (s32 i = 0; i < 3; i++) {
        mMdl.getNodeWorldMtx(mLocatorBones[i], mTabletMatrix[i]);
    }
    setCrestAtBone("SetGS", &mMdl, &mActivatedSceneflag);
    mMdl.setPriorityDraw(0x1C, 9);
    mAcceleration = 0.f;
    mMaxSpeed = -40.f;
    if (mActivatedSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, (u8)mRoomID)) {
        mStateMgr.changeState(StateID_OnSwitch);
    } else {
        mStateMgr.changeState(StateID_Wait);
    }
    mBoundingBox.Set(mVec3_c(-75.f, -15.f, -75.f), mVec3_c(75.f, 205.f, 75.f));
    return SUCCEEDED;
}

int dAcOStoneStand_c::doDelete() {
    return SUCCEEDED;
}

int dAcOStoneStand_c::actorExecute() {
    mStateMgr.executeState();
    updateMatrix();
    mMtx_c mtx = mWorldMtx;
    mMtx_c mtx2;
    mtx2.transS(0.f, field_0x7A4, 0.f);
    mtx.concat(mtx2);
    mMdl.setLocalMtx(mtx);
    mMdl.calc(false);
    for (s32 i = 0; i < 3; i++) {
        mAnm[i].play();
        mMdl.getNodeWorldMtx(mLocatorBones[i], mTabletMatrix[i]);
        mTabletMdls[i].setLocalMtx(mTabletMatrix[i]);
    }
    mBgW.Move();
    mPositionCopy2 = mPosition;
    mPositionCopy2.y += 150.f;
    mPositionCopy3 = mPositionCopy2;
    mVec3_c vec(0.f, 75.f, 30.f);
    vec.rotY(mRotation.y);
    mPositionCopy3 += vec;
    return SUCCEEDED;
}

int dAcOStoneStand_c::actorExecuteInEvent() {
    dAcPy_c *link = dAcPy_c::LINK;
    bool a = false;
    if (mEvent.isThisActorInEvent()) {
        switch (mEvent.getCurrentEventCommand()) {
            case 'act0': {
                if (!mEvent.isInEvent("LithographyStandAppear")) {
                    mEvent.advanceNext();
                } else {
                    field_0x7C8 = 0x1E;
                    mVec3_c vec = mVec3_c::Ez * 125.f;
                    vec.rotY(mRotation.y);
                    vec += mPosition;
                    s16 target = mRotation.y - 0x8000;
                    if (link != nullptr) {
                        cLib::addCalcPos(&vec, mLinkPos, 0.25f, 200.f, 0.f);
                        sLib::addCalcAngle(&mLinkRot.y.mVal, target, 4, 0x7fff, 0);
                        link->setPosRot(&mLinkPos, &mLinkRot, false, 0, 0);
                        mVec3_c vec2 = vec - link->mPosition;
                        if (vec2.squareMagXZ() < 50.f && labs(mRotation.y - target) < 0xb6) {
                            link->setPosRot(&mLinkPos, &mLinkRot, false, 0, 0);
                            // const fProfile::fBaseProfile_c *a = ;

                            updateExecutePriority((*fProfile::sProfileList)[200]->m_execute_order + 1);
                            mEvent.advanceNext();
                        }
                    } else {
                        mEvent.advanceNext();
                    }
                }
                break;
            }
            case 'act1': {
                if (mEvent.isInEvent("LithographyStandAppear")) {
                    fn_513_14D0();
                } else if (mPillarCSExitOffset != 0xFF) {
                    dScGame_c::GetInstance()->triggerExit(mRoomID, mPillarCSExitOffset + mTabletCSIndex);
                }
                break;
            }
            case 'act2': {
                a = true;
                if (!mInsertedTablet) {
                    mOpenedLightPillarState++;
                    link->getBodyMtx(&mTabletMatrix[mVisibleTabletState], 0xF);
                    mInsertedTablet = true;
                } else {
                    link->getBodyMtx(&mTabletMatrix[mVisibleTabletState], 0xF);
                    if (link->getCurrentAnimFrame() > 90.f) {
                        mAnm[mVisibleTabletState].setRate(1.f, 0);
                        mEvent.advanceNext();
                        updateExecutePriority((*fProfile::sProfileList[0x7a])->m_execute_order);
                    }
                }
                break;
            }
            case '????': {
                mEvent.advanceNext();
                break;
            }
        }
    }
    updateMatrix();
    mMtx_c mtx = mWorldMtx;
    mMtx_c mtx2;
    mtx2.transS(0.f, field_0x7A4, 0.f);
    mtx.concat(mtx2);
    mMdl.setLocalMtx(mtx);
    mMdl.calc(false);
    for (s32 i = 0; i < 3; i++) {
        mAnm[i].play();
        if (!a || i != mVisibleTabletState || mAnm[mVisibleTabletState].getRate(0) != 0.f) {
            mMdl.getNodeWorldMtx(mLocatorBones[i], mtx);
        }
        mTabletMdls[i].setLocalMtx(mtx);
    }
    return SUCCEEDED;
}

int dAcOStoneStand_c::draw() {
    drawModelType1(&mMdl);
    for (int i = 0; i < mOpenedLightPillarState; i++) {
        drawModelType1(&mTabletMdls[i]);
    }
    return SUCCEEDED;
}

void dAcOStoneStand_c::doInteraction(s32 param) {
    if (param == 5) {
        void *data = getOarcZev("LithographyStand");
        Event event("StoneStandSceneChange", data, 400, 0x100001, nullptr, nullptr);
        mEvent.scheduleEvent(event, 0);

        const dAcPy_c *link = dAcPy_c::GetLink();
        mLinkPos = link->mPosition;
        mLinkRot = link->mRotation;
    }
}

void dAcOStoneStand_c::initializeState_Wait() {}

void dAcOStoneStand_c::executeState_Wait() {
    if (mActivatedSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mActivatedSceneflag)) {
        void *data = getOarcZev("LithographyStand");
        Event event("LithographyStandAppear", data, 1, 0x100000, nullptr, nullptr);
        mEvent.scheduleEvent(event, 0);
    }
}

void dAcOStoneStand_c::finalizeState_Wait() {}

void dAcOStoneStand_c::initializeState_Shake() {
    field_0x7A8 = -5.f;
}

void dAcOStoneStand_c::executeState_Shake() {
    field_0x7A8 *= 0.85f;
    field_0x7A4 = field_0x7A8 * nw4r::math::SinIdx(field_0x7BE);
    field_0x7BE.mVal += 0x2000;
    if (fabsf(field_0x7A8) < 0.05f) {
        mStateMgr.changeState(StateID_OnSwitch);
    }
}

void dAcOStoneStand_c::finalizeState_Shake() {
    field_0x7A4 = 0.f;
    field_0x7A8 = 0.f;
}

void dAcOStoneStand_c::initializeState_OnSwitch() {}

void dAcOStoneStand_c::executeState_OnSwitch() {
    if (canInsertTablet()) {
        AttentionManager::GetInstance()->addTarget(*this, sInteraction1, 0, nullptr);
        AttentionManager::GetInstance()->addTarget(*this, sInteraction2, 0, nullptr);
    }
}

void dAcOStoneStand_c::finalizeState_OnSwitch() {}

void dAcOStoneStand_c::fn_513_14D0() {
    switch (field_0x7C9) {
        case 0: {
            setCrestPosRot(&mMdl);
            holdSound(SE_SndStn_UP_LV);
            if (sLib::chase(&field_0x78C.y, 2.5f, mPosition.y)) {
                startSound(SE_SndStn_UP_END);
                dRumble_c::start(dRumble_c::sRumblePreset3, 0x11);
                field_0x7A8 = -5.f;
                field_0x7C9 = 1;
            }
            break;
        }
        case 1: {
            field_0x7A8 *= 0.85f;
            field_0x7A4 = field_0x7A8 * nw4r::math::SinIdx(field_0x7BE);
            field_0x7BE.mVal += 0x2000;
            if (fabsf(field_0x7A8) < 0.05f) {
                field_0x7A4 = 0.f;
                field_0x7A8 = 0.f;
                field_0x7C9 = 2;
                mStateMgr.changeState(StateID_OnSwitch);
            }
            break;
        }
        case 2: {
            mEvent.advanceNext();
        }
    }
}

bool dAcOStoneStand_c::canInsertTablet() {
    if (mTabletCSIndex == 0 && !StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_FARON_PILLAR_OPENED)) {
        return true;
    }
    if (mTabletCSIndex == 1 && !StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_ELDIN_PILLAR_OPENED)) {
        return true;
    }
    if (mTabletCSIndex == 2 && !StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_LANAYRU_PILLAR_OPENED)) {
        return true;
    }
    return false;
}

void dAcOStoneStand_c::vt_0x88(f32 &param) {}
