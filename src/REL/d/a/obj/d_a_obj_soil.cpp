#include "d/a/obj/d_a_obj_soil.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_tbox.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_camera.h"
#include "d/d_sc_game.h"
#include "d/d_stage_mgr.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/t/d_t_insect.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_smdl.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/math/math_triangular.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtx.h"
#include "rvl/MTX/vec.h"
#include "s/s_Math.h"
#include "toBeSorted/dowsing_target.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_SOIL, dAcOsoil_c, fProfile::OBJ_SOIL, 0x268, 0, 7);

STATE_DEFINE(dAcOsoil_c, Soil);
STATE_DEFINE(dAcOsoil_c, Hole);
STATE_DEFINE(dAcOsoil_c, Ready);

dCcD_SrcCyl dAcOsoil_c::sCylSrc = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {0, 0x111, {0, 0, 0x407}, 0, 0},
     /* mObjCo */ {0xC9}},
    /* mCylInf */
    {10.0f, 15.0f}
};

s32 useUnused() {
    static const f32 a1[] = {
        240,
        240,
    };
    static const s32 a2[] = {
        1,
        0x000F0000,
        0x459C4000,
        0x006E6950,
        0
    };
    return (s32)a1 + (s32)a2;
}


bool dAcOsoil_c::createHeap() {
    void* data = getOarcResFile("MoundShovel");
    if (data == nullptr) {
        return false;
    }
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("MoundShovel");
    TRY_CREATE(mModels[MODEL_COVERED].create(mdl, &mAllocator, 0x120, 1, nullptr));
    mModels[MODEL_COVERED].setPriorityDraw(0x22, field_0x793 ? 0x82 : 9);
    setModelColorFromParams(&mColors[MODEL_COVERED], mModels[MODEL_COVERED]);
    if (isSubtypeVentOrDrop()) {
        static const char* oarcResForType[4] = {
            "HoleShovel",
            "HoleShovel",
            "MudShovel",
            "MudShovel",
        };
        static const char* mdlForType[4] = {
            "HoleShovel",
            "HoleShovel",
            "MudShovel",
            "MudShovel",
        };
        void* subtypeData = getOarcResFile(oarcResForType[getSubtype()]);
        if (subtypeData == nullptr) {
            return false;
        }
        nw4r::g3d::ResFile subFile = nw4r::g3d::ResFile(subtypeData);
        nw4r::g3d::ResMdl subMdl = subFile.GetResMdl(mdlForType[getSubtype()]);
        TRY_CREATE(mModels[MODEL_DUG_UP].create(subMdl, &mAllocator, 0x120, 1, nullptr));
        mModels[MODEL_DUG_UP].setPriorityDraw(0x22, field_0x793 ? 0x82 : 9);
        setModelColorFromParams(&mColors[MODEL_DUG_UP], mModels[MODEL_DUG_UP]);
    }
    return true;
}

int dAcOsoil_c::actorCreate() {
    mSubtype = getFromParams(0, 0xF);
    if (mSubtype > 4) {
        mSubtype = SUBTYPE_VENT;
    }
    if (mSubtype == SUBTYPE_KEY_PIECE) {
        mSubtype = SUBTYPE_DROP;
        mItemSubtype = ITEMTYPE_KEY_PIECE;
    } else {
        mItemSubtype = ITEMTYPE_DROP;
    }
    mSceneflag = getFromParams(4, 0xFF);
    mSpawnSceneflag = getParams2Lower() & 0xFF;
    if (mSpawnSceneflag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSpawnSceneflag)) {
        mIsHidden = true;
        mInteractionDisabled = true;
    }
    if (getFromParams(0x13, 1) == 0 && mItemSubtype == ITEMTYPE_DROP && mSubtype != SUBTYPE_TBOX) {
        mForInsects = true;
    } 
    if (!mForInsects) {
        mCountInParams = getFromParams(0xC, 0xF);
        if (mCountInParams == 0) {
            mCountInParams = 0xF;
        }
        if (getFromParams(0x10, 3) == 1) {
            if (mItemSubtype == ITEMTYPE_DROP) {
                mItemSubtype = ITEMTYPE_DIRECT_GIVE;
            }
        }
    } else {
        mCountInParams = 1;
    }
    mRotation.x = mAng(0);
    mRotation.z = mAng(0);
    if (getFromParams(0x12, 1) == 0) {
        mDowsingStateRelated = 2;
        mAlwaysLoaded = true;
    }
    if (getFromParams(0x19, 1) == 0) {
        field_0x793 = true;
    }
    CREATE_ALLOCATOR(dAcOsoil_c);
    mStts.SetRank(0);
    mCollider.Set(sCylSrc);
    mCollider.SetStts(mStts);
    mAcceleration = 0;
    mMaxSpeed = -40;
    updateMatrix();
    if (isSubtypeVentOrDrop()) {
        mModels[MODEL_DUG_UP].setLocalMtx(mWorldMtx);
        mModels[MODEL_COVERED].setLocalMtx(mWorldMtx);
    } else if (mModelIndex == MODEL_COVERED) {
        mModels[MODEL_COVERED].setLocalMtx(mWorldMtx);
    }
    if (dScGame_c::currentSpawnInfo.stageName == "F200") {
        static const mVec3_c posOnSkyloft(-7301, 3768, -9309);
        if (mPosition.squareDistance(posOnSkyloft) < 360000.f) {
            mHideWhenCameraNear = true;
        }
    }
    mBoundingBox.Set(mVec3_c(-100, -10, -100), mVec3_c(100, 50, 100));
    if (mAlwaysLoaded) {
        s32 roomId = mRoomID;
        if (addActorToRoom(-1) != 0) {
            mRoomID = roomId;
            changeLoadedEntitiesWithSet();
        }
    }
    return SUCCEEDED;
}
int dAcOsoil_c::actorPostCreate() {
    if (mSubtype == SUBTYPE_TBOX) {
        mStateMgr.changeState(StateID_Soil);
    } else {
        if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag) || getFromParams(0x18, 1) == 0) {
            mDowsingStateRelated = 0;
            if (mSubtype == SUBTYPE_DROP || mSubtype == SUBTYPE_MOGMA) {
                mInteractionDisabled = true;
            }
            mStateMgr.changeState(StateID_Hole);
        } else {
            mStateMgr.changeState(StateID_Soil);
        }
    }
    if (mDowsingStateRelated == 2) {
        mDowsingTarget.initialize(DowsingTarget::SLOT_QUEST, 0, nullptr, 5000);
        if (mItemSubtype == ITEMTYPE_KEY_PIECE && !DowsingTarget::hasKeyPieceDowsing()) {
            mDowsingStateRelated = 1;
            unsetActorProperty(AC_PROP_0x1);
        } else {
            mDowsingTarget.doRegister();
        }
    }
    if (mForInsects) {
        dTgInsect_c* tgInsect = nullptr;
        do {
            tgInsect = static_cast<dTgInsect_c*>(fManager_c::searchBaseByProfName(fProfile::TAG_INSECT, tgInsect));
            if (tgInsect != nullptr) {
                f32 dist = tgInsect->mPosition.squareDistanceToXZ(mPosition);
                if (dist < 10000) {
                    mInsectLink.link(tgInsect);
                    break;
                }
            }
        } while(tgInsect != nullptr);
    }
    if (mSceneflag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag) && dScGame_c::currentSpawnInfo.stageName == "F401") {
        mColors[MODEL_DUG_UP].a = 0xFF;
        mModelIndex = MODEL_DUG_UP;
        mModels[MODEL_DUG_UP].setTevColorAll(GX_TEVREG1, mColors[MODEL_DUG_UP], false);
        mLightingInfo.setTev1Color(mColors[MODEL_DUG_UP]);
        unsetActorProperty(AC_PROP_0x1);
        mHideCover = true;
    }
    if (mItemSubtype == ITEMTYPE_KEY_PIECE || mItemSubtype == ITEMTYPE_DIRECT_GIVE) {
        unsetActorProperty(AC_PROP_0x1);
    }
    mPositionCopy2 = mPosition;
    mPositionCopy2.y += 5;
    mPositionCopy3 = mPositionCopy2;
    return SUCCEEDED;
}


int dAcOsoil_c::doDelete() {
    if (mAlwaysLoaded) {
        changeLoadedEntitiesNoSet();
    }
    return SUCCEEDED;
}

extern "C" dAcBase_c *getCurrentEventActor();
extern "C" bool AcItem__isKeyPiece2(dAcItem_c*);
int dAcOsoil_c::actorExecute() {
    if (mShouldSetFlagInEvent) {
        if (EventManager::isInEvent()) {
            if (EventManager::isCurrentEvent("DefaultGetItem") || EventManager::isCurrentEvent("ItemGetGorgeous")) {
                if (mItemSubtype == ITEMTYPE_KEY_PIECE) {
                    if (AcItem__isKeyPiece2(static_cast<dAcItem_c*>(getCurrentEventActor()))) {
                        mShouldSetFlagInEvent = false;
                        SceneflagManager::sInstance->setFlag(mRoomID, mSceneflag);
                    }
                } else if (mItemSubtype == ITEMTYPE_DIRECT_GIVE) {
                    mShouldSetFlagInEvent = false;
                    SceneflagManager::sInstance->setFlag(mRoomID, mSceneflag);
                }
            }
        }
    }
    if (mDowsingStateRelated == 1 && DowsingTarget::hasKeyPieceDowsing()) {
        mDowsingStateRelated = 2;
        mDowsingTarget.doRegister();
    }
    if (mIsHidden && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSpawnSceneflag)) {
        mIsHidden = false;
        mInteractionDisabled = false;
    }
    tryLinkTbox();
    fn_255_1D80();
    handleModelsOpacities();
    mStateMgr.executeState();
    fn_255_1A50();
    if (!mInteractionDisabled) {
        bool hit = false;
        checkCoHit(&hit);
        if (!hit && mStateMgr.isState(StateID_Soil)) {
            dAcPy_c::addDigInteractionTarget(this, 240);
        }
    }
    mCollider.SetC(mPosition);
    dCcS::GetInstance()->Set(&mCollider);
    return SUCCEEDED;
}

int dAcOsoil_c::draw() {
    if (mIsHidden) {
        return SUCCEEDED;
    }
    if (isSubtypeVentOrDrop()) {
        if (mStateMgr.isState(StateID_Ready)) {
            if (mHideCover) {
                drawModelType1(&mModels[MODEL_DUG_UP]);
            } else {
                drawModelType1(&mModels[MODEL_COVERED]);
                drawModelType1(&mModels[MODEL_DUG_UP]);
            }
        } else {
            drawModelType1(&mModels[mModelIndex]);
        }
    } else if(mModelIndex == MODEL_COVERED) {
        drawModelType1(&mModels[MODEL_COVERED]);
    }
    return SUCCEEDED;
}

void dAcOsoil_c::initializeState_Soil() {
    mModelIndex = MODEL_COVERED;
    if (mSubtype == SUBTYPE_TBOX || mItemSubtype == ITEMTYPE_KEY_PIECE || mItemSubtype == ITEMTYPE_DIRECT_GIVE) {
        mDigsLeft = 1;
    } else if (mCountInParams == 0xF) {
        mDigsLeft = 3 + cM::rndInt(3);
    } else {
        mDigsLeft = mCountInParams;
    }
}
void dAcOsoil_c::executeState_Soil() {
    if (!checkActorProperty(AC_PROP_0x20000000) && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag)) {
        return;
    }
    if (mSubtype == SUBTYPE_VENT) {
        bool tmp = false;
        if (checkCoHit(&tmp)) {
            mDigsLeft = 1;
        }
    }
    if (mDigsLeft && --mDigsLeft) {
        giveItemRepeated();
    } else {
        if (mItemSubtype == ITEMTYPE_KEY_PIECE || mItemSubtype == ITEMTYPE_DIRECT_GIVE) {
            mFinalDigTimer = 1;
        } else if (mSubtype == SUBTYPE_VENT) {
            if (mFinalDigTimer == 0) {
                mFinalDigTimer = 1;
            }
        } else if (mSubtype == SUBTYPE_TBOX) {
            if (!mInteractionDisabled) {
                mInteractionDisabled = true;
                mFinalDigTimer = 0xF;
            }
        } else if (mSubtype == SUBTYPE_DROP || mSubtype == SUBTYPE_MOGMA) {
            if (!mInteractionDisabled) {
                mInteractionDisabled = true;
                mFinalDigTimer = 1;
            }
        }
        if (mItemSubtype == ITEMTYPE_KEY_PIECE || mItemSubtype == ITEMTYPE_DIRECT_GIVE) {
            mShouldSetFlagInEvent = true;
        } else {
            SceneflagManager::sInstance->setFlag(mRoomID, mSceneflag);
        }
        mStateMgr.changeState(StateID_Ready);
    }
}
void dAcOsoil_c::finalizeState_Soil() {}
void dAcOsoil_c::initializeState_Hole() {
    mModelIndex = MODEL_DUG_UP;
    mCollider.ClrCoSet();
    if (mItemSubtype == ITEMTYPE_KEY_PIECE || mItemSubtype == ITEMTYPE_DIRECT_GIVE) {
        return;
    }
    setActorProperty(AC_PROP_0x1);
}
void dAcOsoil_c::executeState_Hole() {
    if (checkActorProperty(AC_PROP_0x20000000)) {
        if (mSceneflag < 0xFF) {
            SceneflagManager::sInstance->unsetFlag(mRoomID, mSceneflag);
        }
        mStateMgr.changeState(StateID_Ready);
    }
}
void dAcOsoil_c::finalizeState_Hole() {}
void dAcOsoil_c::initializeState_Ready() {
    mInteractionDisabled = true;
    if (isSubtypeVentOrDrop()) {
        mMtx_c tmpMtx = mWorldMtx;
        tmpMtx.transM(0, 0.2f, 0);
        mModels[MODEL_COVERED].setLocalMtx(tmpMtx);
    }
    if (mModelIndex == MODEL_COVERED) {
        mColors[MODEL_COVERED].a = 0xFF;
        mModels[MODEL_COVERED].setTevColorAll(GX_TEVREG1, mColors[MODEL_COVERED], false);
        mLightingInfo.setTev1Color(mColors[MODEL_COVERED]);
        if (isSubtypeVentOrDrop()) {
            mColors[MODEL_DUG_UP].a = 0;
            mModels[MODEL_DUG_UP].setTevColorAll(GX_TEVREG1, mColors[MODEL_DUG_UP], false);
            mLightingInfo.setTev1Color(mColors[MODEL_DUG_UP]);
        }
    }
}
void dAcOsoil_c::executeState_Ready() {
    if (mModelIndex == MODEL_COVERED) {
        bool bVar4 = true;
        bool bVar3 = true;
        bool bVar2 = true;
        if (isSubtypeVentOrDrop()) {
            bVar4 = sLib::chaseUC(&mColors[MODEL_DUG_UP].a, 0xFF, 0x1E);
            mModels[MODEL_DUG_UP].setTevColorAll(GX_TEVREG1, mColors[MODEL_DUG_UP], false);
            mLightingInfo.setTev1Color(mColors[MODEL_DUG_UP]);
            if (mColors[MODEL_DUG_UP].a < 50) {
                bVar3 = false;
            }
        }
        bVar2 = false;
        if (bVar3) {
            bVar2 = sLib::chaseUC(&mColors[MODEL_COVERED].a, 0, 0x14);
            mModels[MODEL_COVERED].setTevColorAll(GX_TEVREG1, mColors[MODEL_COVERED], false);
            mLightingInfo.setTev1Color(mColors[MODEL_COVERED]);
        }
        if (bVar2 && bVar4) {
            mStateMgr.changeState(StateID_Hole);
        }
    } else {
        mStateMgr.changeState(StateID_Hole);
    }
}
void dAcOsoil_c::finalizeState_Ready() {
    if (!isSubtypeVentOrDrop()) {
        return;
    }
    mInteractionDisabled = false;
    mModels[MODEL_COVERED].setLocalMtx(mWorldMtx);
}

void dAcOsoil_c::tryLinkTbox() {
    if (mSubtype == SUBTYPE_TBOX && !mHasTriedTboxLink) {
        mHasTriedTboxLink = true;
        dAcTbox_c* tbox = nullptr;
        f32 minDist = 100;
        do {
            tbox = static_cast<dAcTbox_c*>(fManager_c::searchBaseByProfName(fProfile::TBOX, tbox));
            if (tbox != nullptr) {
                f32 dist = tbox->mPosition.squareDistanceToXZ(mPosition);
                if (dist < minDist) {
                    minDist = dist;
                    mTboxLink.link(tbox);
                }
            }
        } while(tbox != nullptr);
        if (mTboxLink.isLinked()) {
            dAcTbox_c* tbox = mTboxLink.get();
            if (tbox->hasBeenOpened() == true) {
                mStateMgr.changeState(StateID_Hole);
                mInteractionDisabled = true;
            } else {
                tbox->setActorProperty(AC_PROP_0x40000000);
                tbox->setObjectProperty(OBJ_PROP_0x200);
            }
        }
    }
}

void dAcOsoil_c::giveItemRepeated() {
    if (!mForInsects) {
        if (mItemSubtype == ITEMTYPE_KEY_PIECE) {
            dAcItem_c::giveItem(ITEM_KEY_PIECE, -1, 0);
        } else {
            calcItemAndInsectSpawnPos();
            u32 param2NoLower = getParams2_ignoreLower();
            itemDroppingAndGivingRelated(&mItemAndInsectSpawnPos, mItemSubtype == ITEMTYPE_DIRECT_GIVE ? 4 : 1);
            setParams2Upper_ignoreLower(param2NoLower);
        }
    }
}

void dAcOsoil_c::giveItemFinal() {
    if (mDowsingStateRelated != 0) {
        if (mDowsingStateRelated == 2) {
            mDowsingTarget.doUnregister();
        }
        mDowsingStateRelated = 0;
    }
    if (!mForInsects) {
        if (mItemSubtype == ITEMTYPE_KEY_PIECE) {
            dAcItem_c::giveItem(ITEM_KEY_PIECE, -1, 0);
        } else {
            calcItemAndInsectSpawnPos();
            itemDroppingAndGivingRelated(&mItemAndInsectSpawnPos, mItemSubtype == ITEMTYPE_DIRECT_GIVE ? 4 : 1);
        }
    } else if (mInsectLink.isLinked()) {
        calcItemAndInsectSpawnPos();
        dTgInsect_c* tgInsect = mInsectLink.get();
        tgInsect->reveal(&mItemAndInsectSpawnPos);
    }
}

void dAcOsoil_c::calcItemAndInsectSpawnPos() {
    mItemAndInsectSpawnPos = mPosition;
    mItemAndInsectSpawnPos.offsetWithAngle(dAcPy_c::LINK->getRotation().y, 30);
}

void dAcOsoil_c::fn_255_1A50() {
    if (mFinalDigTimer != 0 && --mFinalDigTimer == 0) {
        if (mSubtype == SUBTYPE_TBOX) {
            if (!mTboxPrepared && mTboxLink.isLinked()) {
                dAcTbox_c* tbox = mTboxLink.get();
                tbox->unsetActorProperty(AC_PROP_0x40000000);
                tbox->unsetObjectProperty(OBJ_PROP_0x200);
                tbox->mRotation.y = tbox->getXZAngleToPlayer();
                mTboxPrepared = true;
            }
        } else {
            giveItemFinal();
        }
    }
    if (mTboxPrepared) {
        if (mTboxLink.isLinked())  {
            dAcTbox_c* tbox = mTboxLink.get();
            if (tbox->mScale.x >= 1) {
                mTboxPrepared = false;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_READ_RIDDLE_A);
            }
        } else {
            mTboxPrepared = false;
        }
    }
}

bool dAcOsoil_c::checkCoHit(bool* pOut) {
    bool hitByWind = false;
    *pOut = false;
    if (mCollider.ChkCoHit()) {
        dAcObjBase_c* actor = mCollider.GetCoActor();
        if (actor != nullptr && (actor->mProfileName == fProfile::OBJ_WIND || actor->mProfileName == fProfile::OBJ_WIND02 || actor->mProfileName == fProfile::OBJ_WIND03)) {
            hitByWind = true;
        } else {
            *pOut = true;
        }
    }
    return hitByWind;
}

void dAcOsoil_c::setModelColorFromParams(GXColor* pOutColor, m3d::smdl_c& mdl) {
    u32 param = getFromParams(0x14, 0xF);
    if (param >= 5) {
        param = 0;
    }
    static const mColor colors[5] = {
        mColor(0x6E, 0x69, 0x50, 0xFF),
        mColor(0xB4, 0x8C, 0, 0xFF),
        mColor(0xC8, 0x96, 0x50, 0xFF),
        mColor(0x8C, 0x6E, 0x46, 0xFF),
        mColor(0x8C, 0x82, 0x64, 0xFF),
    };
    *pOutColor = colors[param];
    mdl.setTevColorAll(GX_TEVREG1, *pOutColor, false);
    mLightingInfo.setTev1Color(*pOutColor);
}

void dAcOsoil_c::fn_255_1D80() {
    if (!field_0x793) {
        return;
    }
    if (dAcPy_c::GetLink()->checkActionFlagsCont(0x400000)) {
        mSomeCounter1 = 3;
        setModelAlphaToGlobalAlpha();
    } else if (mSomeCounter1 != 0) {
        mSomeCounter1--;
        setModelAlphaToGlobalAlpha();
    }
}

void dAcOsoil_c::setModelAlphaToGlobalAlpha() {
    u8 globalAlpha = dStageMgr_c::GetInstance()->getGlobalAlpha();
    if (globalAlpha != mColors[mModelIndex].a) {
        mColors[mModelIndex].a = globalAlpha;
        mModels[mModelIndex].setTevColorAll(GX_TEVREG1, mColors[mModelIndex], false);
        mLightingInfo.setTev1Color(mColors[mModelIndex]);
    }
}

// hide itself when camera is near
void dAcOsoil_c::handleModelsOpacities() {
    if (!mHideWhenCameraNear) {
        return;
    }
    if (mStateMgr.isState(StateID_Ready)) {
        return;
    }
    if (dAcPy_c::LINK->mPosition.squareDistance(mPosition) < 1000000.f) {
        dCamera_c* camera = dScGame_c::getCamera();
        if (camera != nullptr) {
            f32 dist = (camera->getPositionMaybe() - mPosition).y * (1.f / 80.f);
            if (dist < 0.1f) {
                dist = 0;
            } else if (dist > 1) {
                dist = 1;
            }
            mColors[mModelIndex].a = 255 * dist;
            mModels[mModelIndex].setTevColorAll(GX_TEVREG1, mColors[mModelIndex], false);
            mLightingInfo.setTev1Color(mColors[mModelIndex]);
        }
    } else {
        if (mColors[mModelIndex].a != 0xFF) {
            sLib::chaseUC(&mColors[mModelIndex].a, 0xFF, 0x32);
            mModels[mModelIndex].setTevColorAll(GX_TEVREG1, mColors[mModelIndex], false);
            mLightingInfo.setTev1Color(mColors[mModelIndex]);
        }
    }
}

s32 moreUnusedd() {
    static const s32 stuff[] = {
        0x49742400,
        0x42a00000,
        0x3c4ccccd,
        0x41f00000,
        0x42c80000,
        0x0000001e,
        0x00000014,
        0x00000032,
        0x461c4000,
        0x40a00000,
        0x48afc800
    };
    return (s32) stuff;
}
