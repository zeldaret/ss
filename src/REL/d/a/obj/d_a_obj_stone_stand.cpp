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
#include "nw4r/math/math_arithmetic.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/event.h"

SPECIAL_ACTOR_PROFILE(OBJ_STONE_STAND, dAcOStoneStand_c, fProfile::OBJ_STONE_STAND, 0x24B, 0, 3);

STATE_DEFINE(dAcOStoneStand_c, Wait);
STATE_DEFINE(dAcOStoneStand_c, Shake);
STATE_DEFINE(dAcOStoneStand_c, OnSwitch);

char *dAcOStoneStand_c::sResMdlAnmNames[3] = {"SekibanMapADemo", "SekibanMapBDemo", "SekibanMapCDemo"};

const u32 dAcOStoneStand_c::pad[2] = {0, 0};

const InteractionTargetDef dAcOStoneStand_c::sInteractions[2] = {
    {0, 3, 0, EXAMINE_TALK, 0, 250.f, 60.f, 60.f, -50.f, 150.f, 50.f, 1.f},
    {1, 3, 0, EXAMINE_TALK, 0, 250.f, 60.f, 60.f, -50.f, 150.f, 50.f, 1.f},
};

bool dAcOStoneStand_c::createHeap() {
    void *data = getOarcResFile("LithographyStand");
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("LithographyStand");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x120));
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
    mLocatorBones[0] = mMdl.getResMdl().GetResNode("locator_A").GetID();
    mLocatorBones[1] = mMdl.getResMdl().GetResNode("locator_B").GetID();
    mLocatorBones[2] = mMdl.getResMdl().GetResNode("locator_C").GetID();
    for (s32 i = 0; i < 3; i++) {
        mMdl.getNodeWorldMtx(mLocatorBones[i], mTabletMatrix[i]);
    }
    setCrestAtBone("SetGS", &mMdl, &mActivatedSceneflag);
    mMdl.setPriorityDraw(0x1C, 9);
    mAcceleration = 0.f;
    mMaxSpeed = -40.f;
    if (mActivatedSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mActivatedSceneflag)) {
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
    bool act2 = false;
    if (mEvent.isThisActorInEvent()) {
        switch (mEvent.getCurrentEventCommand()) {
            case 'act0': {
                if (mEvent.isInEvent("LithographyStandAppear")) {
                    mEvent.advanceNext();
                } else {
                    field_0x7C8 = 0x1E;
                    mVec3_c targetPos = mVec3_c(mVec3_c::Ez * 125.f);
                    targetPos.rotY(mRotation.y);
                    targetPos += mPosition;
                    s32 targetAng = mRotation.y - 0x8000;
                    if (link != nullptr) {
                        cLib::addCalcPos(&mLinkPos, targetPos, 0.25f, 200.f, 0.f);
                        sLib::addCalcAngle(mLinkRot.y.ref(), targetAng, 4, 0x7fff, 0);
                        link->setPosRot(&mLinkPos, &mLinkRot, false, 0, 0);
                        if (targetPos.squareDistanceToXZ(link->mPosition) < 50.f &&
                            mAng(mRotation.y - mAng(targetAng)).abs() < 0xB6) {
                            link->setPosRot(&mLinkPos, &mLinkRot, false, 0, 0);
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
                    dScGame_c::GetInstance()->triggerExit(mRoomID, u8(mPillarCSExitOffset + mTabletCSIndex));
                }
                break;
            }
            case 'act2': {
                act2 = true;
                if (!mInsertedTablet) {
                    mOpenedLightPillarState++;
                    link->getBodyMtx(&mTabletMatrix[mVisibleTabletState], 0xF);
                    mInsertedTablet = true;
                } else {
                    link->getBodyMtx(&mTabletMatrix[mVisibleTabletState], 0xF);
                    if (link->getCurrentAnimFrame() > 90.f) {
                        mAnm[mVisibleTabletState].setRate(1.f, 0);
                        mEvent.advanceNext();
                        updateExecutePriority((*fProfile::sProfileList)[0x7A]->m_execute_order);
                    }
                }
                break;
            }
            case '????': break;
            default:     mEvent.advanceNext();
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
        if (!act2 || i != mVisibleTabletState || mAnm[mVisibleTabletState].getRate(0) != 0.f) {
            mMdl.getNodeWorldMtx(mLocatorBones[i], mTabletMatrix[i]);
        }
        mTabletMdls[i].setLocalMtx(mTabletMatrix[i]);
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
        {
            void *data = getOarcZev("LithographyStand");
            Event event("StoneStandSceneChange", data, 400, 0x100001, nullptr, nullptr);
            mEvent.scheduleEvent(event, 0);
        }

        const dAcPy_c *link = dAcPy_c::GetLink();
        mLinkPos = link->mPosition;
        mLinkRot = link->mRotation;
    }
}

void dAcOStoneStand_c::initializeState_Wait() {}

volatile u32 FLAGS_1 = 0x00000001;
u32 FLAGS_2 = 0x00100001;

void dAcOStoneStand_c::executeState_Wait() {
    if (mActivatedSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mActivatedSceneflag)) {
        u32 f1 = ~FLAGS_1;
        void *data = getOarcZev("LithographyStand");
        u32 f2 = FLAGS_2;
        Event event("LithographyStandAppear", data, 1, f2 & f1, nullptr, nullptr);
        mEvent.scheduleEvent(event, 0);
    }
}

void dAcOStoneStand_c::finalizeState_Wait() {}

void dAcOStoneStand_c::initializeState_Shake() {
    field_0x7A8 = -5.f;
}

const s16 dAcOStoneStand_c::lbl_513_rodata_A8 = 0x2000;

void dAcOStoneStand_c::executeState_Shake() {
    field_0x7A8 *= 0.85f;
    field_0x7A4 = field_0x7A8 * field_0x7BE.sin();
    field_0x7BE += mAng(lbl_513_rodata_A8);
    if (nw4r::math::FAbs(field_0x7A8) < 0.05f) {
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
        AttentionManager::GetInstance()->addTarget(*this, sInteractions[0], 0, nullptr);
        AttentionManager::GetInstance()->addTarget(*this, sInteractions[1], 0, nullptr);
    }
}

void dAcOStoneStand_c::finalizeState_OnSwitch() {}

void dAcOStoneStand_c::fn_513_14D0() {
    switch (field_0x7C9) {
        case 0: {
            setCrestPosRot(&mMdl);
            holdSound(SE_SndStn_UP_LV);
            if (sLib::chase(&mPosition.y, field_0x78C.y, 2.5f)) {
                startSound(SE_SndStn_UP_END);
                dRumble_c::start(dRumble_c::sRumblePreset3, 0x11);
                field_0x7A8 = -5.f;
                field_0x7C9 = 1;
            }
            break;
        }
        case 1: {
            field_0x7A8 *= 0.85f;
            field_0x7A4 = field_0x7A8 * field_0x7BE.sin();
            field_0x7BE += mAng(lbl_513_rodata_A8);
            if (nw4r::math::FAbs(field_0x7A8) < 0.05f) {
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
    return (mTabletCSIndex == 0 && !StoryflagManager::sInstance->getFlag(STORYFLAG_FARON_PILLAR_OPENED)) ||
           (mTabletCSIndex == 1 && !StoryflagManager::sInstance->getFlag(STORYFLAG_ELDIN_PILLAR_OPENED)) ||
           (mTabletCSIndex == 2 && !StoryflagManager::sInstance->getFlag(STORYFLAG_LANAYRU_PILLAR_OPENED));
}

void dAcOStoneStand_c::vt_0x88(f32 &param) {}
