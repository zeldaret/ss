#include "d/a/obj/d_a_obj_switch.h"

#include "d/a/d_a_player.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_w.h"
#include "d/d_linkage.h"
#include "d/d_rumble.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(OBJ_SW, dAcOsw_c, fProfile::OBJ_SW, 0x12B, 0, 0x1002);

static const char *SWITCH_TYPES[] = {"SwitchStepA", "SwitchStepB", "SwitchStepB"};

STATE_DEFINE(dAcOsw_c, OnWait);
STATE_DEFINE(dAcOsw_c, On);
STATE_DEFINE(dAcOsw_c, OffWait);
STATE_DEFINE(dAcOsw_c, Off);
STATE_DEFINE(dAcOsw_c, None);

void dAcOswMdlCallback_c::timingB(u32 nodeId, nw4r::g3d::WorldMtxManip *manip, nw4r::g3d::ResMdl mdl) {
    if (nodeId == mNodeId) {
        mMtx_c mtx1;
        mMtx_c mtx2;
        manip->GetMtx(mtx1);
        PSMTXTrans(mtx2, mElevation, 0.0f, 0.0f);
        PSMTXConcat(mtx1, mtx2, mtx1);
        manip->SetMtx(mtx1);
    }
}

static bool isPushableBlock(dAcBase_c *actor) {
    return actor->isActorPlayer() || actor->profile_name == fProfile::OBJ_PUSH_BLOCK ||
           actor->profile_name == fProfile::OBJ_KIBAKO;
}

void dAcOsw_c::rideCallback(dBgW *unknown, dAcObjBase_c *actor, dAcObjBase_c *interactor) {
    if (!isPushableBlock(interactor)) {
        return;
    }
    dAcPy_c *link = interactor->isActorPlayer() ? static_cast<dAcPy_c *>(interactor) : nullptr;
    dAcOsw_c *sw = static_cast<dAcOsw_c *>(actor);

    if (link != nullptr) {
        if (link->checkActionFlags(dAcPy_c::FLG0_IN_WATER)) {
            return;
        }
        if (link->checkFlags0x340(0x800000) &&
            !(link->getCurrentCarriedActor() && link->getCurrentCarriedActor()->GetLinkage().checkFlag(0x04))) {
            return;
        }
    }

    if (interactor->GetLinkage().checkState(dLinkage_c::STATE_ACTIVE)) {
        return;
    }

    if (!(link && link->checkActionFlags(0xC70852))) {
        if (!sw->someInteractCheck(link != nullptr)) {
            if (!sw->checkOnFlag() && sw->mObjRef.get() == nullptr && link != nullptr && sw->field_0x5F1 == 0 &&
                sw->mStateMgr.isState(StateID_On)) {
                link->onFlags_0x360(0x8000);
            }
        }
    }
    interactor->setObjectProperty(0x40);
}

bool dAcOsw_c::createHeap() {
    nw4r::g3d::ResFile resFile(getOarcResFile(SWITCH_TYPES[mSwitchType]));
    nw4r::g3d::ResMdl resMdl(resFile.GetResMdl(SWITCH_TYPES[mSwitchType]));
    TRY_CREATE(mModel.create(resMdl, &mAllocator, 0x20, 1, nullptr));

    field_0x5E8 =
        mScale.x * (resMdl.GetResNode("base").ref().volume_max.x - resMdl.GetResNode("base").ref().volume_min.x);
    cBgD_t *dbzData = (cBgD_t *)getOarcDZB(SWITCH_TYPES[mSwitchType], SWITCH_TYPES[mSwitchType]);
    PLC *plcData = (PLC *)getOarcPLC(SWITCH_TYPES[mSwitchType], SWITCH_TYPES[mSwitchType]);
    mScale.set(1.0f, 0.8f, 1.0f);
    updateMatrix();
    field_0x5B8.copyFrom(mWorldMtx);
    mModel.setLocalMtx(mWorldMtx);

    bool set = mCollision.Set(dbzData, plcData, cBgW::MOVE_BG_e, &field_0x5B8, &mScale);
    mCollision.SetCrrFunc(dBgS_MoveBGProc_TypicalRotY);
    return !set;
}

int dAcOsw_c::actorCreate() {
    mSwitchType = mParams & 0xF;
    mOnSceneFlag = (mParams >> 0xE);
    CREATE_ALLOCATOR(dAcOsw_c);
    mCollision.SetRideCallback(rideCallback);
    mModel.setCallback(&mButtonCtrl);
    dBgS::GetInstance()->Regist(&mCollision, this);
    mOffSceneFlag = (mParams >> 0x4);
    mCanBeSeen = ((mParams >> 0xC) & 3) == 0;
    nw4r::g3d::ResMdl resMdl = mModel.getResMdl();
    nw4r::g3d::ResNode node = resMdl.GetResNode("button");
    mButtonCtrl.init(node.GetID());
    if (mOffSceneFlag >= 0xFF) {
        mStateMgr.changeState(StateID_None);
    } else if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mOffSceneFlag)) {
        mStateMgr.changeState(StateID_OffWait);
    } else {
        mStateMgr.changeState(StateID_OnWait);
    }
    boundingBox.Set(mVec3_c(-90.0f, -10.0f, -90.0f), mVec3_c(90.0f, 70.0f, 90.0f));

    return SUCCEEDED;
}

int dAcOsw_c::actorPostCreate() {
    if (mCanBeSeen) {
        field_0x5A0.check(mRoomID, mPosition, 0, 30.0f, 0.1f);
        if (field_0x5A0.field_0x00 <= 0.0f) {
            mHidden = true;
            mScale.set(0.0f, 0.0f, 0.0f);
        } else {
            mShown = true;
            mScale.set(1.0f, 1.0f, 1.0f);
        }
    }

    dAcBase_c *parent = nullptr;
    do {
        parent = static_cast<dAcBase_c *>(fManager_c::searchBaseByProfName(fProfile::OBJ_VSD, parent));
        if (parent != nullptr && nw4r::math::FAbs(parent->mPosition.y - mPosition.y) < 30.0f) {
            if (getSquareDistanceTo(parent->mPosition) < 900.0f) {
                mObjRef.link(static_cast<dAcOScatterSand_tmp *>(parent));
                break;
            }
        }
    } while (parent != nullptr);
    mModel.setScale(mScale);
    return SUCCEEDED;
}

int dAcOsw_c::doDelete() {
    dAcPy_c *link = dAcPy_c::LINK;
    if (mSwitchType != 1 && field_0x5F2 == 0) {
        if ((link == nullptr || link->checkFlags0x340(0x200)) && (link == nullptr || link->mRoomID == mRoomID)) {
            if (mOffSceneFlag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mOffSceneFlag)) {
                SceneflagManager::sInstance->unsetFlag(mRoomID, mOffSceneFlag);
            }
        }
    }
    return SUCCEEDED;
}

int dAcOsw_c::actorExecute() {
    mStateMgr.executeState();
    if (mCanBeSeen) {
        field_0x5A0.check(mRoomID, mPosition, 0, 30.0f, 0.1f);
        if (field_0x5A0.field_0x00 <= 0.0f) {
            if (mScale.x >= 1.0f) {
                if (!mHidden) {
                    dJEffManager_c::spawnEffect(
                        PARTICLE_RESOURCE_ID_MAPPING_754_, mPosition, &mRotation, nullptr, nullptr, nullptr, 0, 0
                    );
                    startSound(SE_TIMESLIP_TIMESLIP_REV);
                    mHidden = true;
                }
            } else {
                mHidden = false;
            }
            sLib::chase(&mScale.x, 0.0f, 0.07f);
            mScale.y = mScale.x;
            mScale.z = mScale.x;
        } else {
            if (mScale.x <= 0.0f) {
                if (!mShown) {
                    dJEffManager_c::spawnEffect(
                        PARTICLE_RESOURCE_ID_MAPPING_754_, mPosition, &mRotation, nullptr, nullptr, nullptr, 0, 0
                    );
                    startSound(SE_TIMESLIP_TIMESLIP);
                    mShown = true;
                }
            } else {
                mShown = false;
            }
            sLib::chase(&mScale.x, 1.0f, 0.07f);
            mScale.z = mScale.x;
        }
    }

    updateMatrix();
    field_0x5B8.copyFrom(mWorldMtx);
    mMtx_c tmp;
    PSMTXTrans(tmp, 0.0f, mButtonCtrl.mElevation, 0.0f);
    PSMTXConcat(field_0x5B8, tmp, field_0x5B8);
    mModel.setScale(mScale);
    mModel.setLocalMtx(mWorldMtx);
    mModel.calc(false);
    mCollision.Move();
    field_0x5F3 = 0;
    mActivatedByPlayer = 0;
    field_0x5F2 = field_0x5F1;
    return SUCCEEDED;
}

int dAcOsw_c::actorExecuteInEvent() {
    return SUCCEEDED;
}

int dAcOsw_c::draw() {
    drawModelType1(&mModel);

    return SUCCEEDED;
}

bool dAcOsw_c::someInteractCheck(bool activatedByPlayer) {
    mActivatedByPlayer = activatedByPlayer;
    field_0x5F3 = 1;
    return SceneflagManager::sInstance->checkBoolFlag(mRoomID, mOffSceneFlag);
}

void dAcOsw_c::initializeState_OnWait() {
    if (mOffSceneFlag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mOffSceneFlag)) {
        SceneflagManager::sInstance->unsetFlag(mRoomID, mOffSceneFlag);
    }
}
void dAcOsw_c::executeState_OnWait() {
    sLib::chase(&mScale.y, 0.8f, 0.12f);
    if (field_0x5F1 != 0) {
        field_0x5F3 = 1;
    }

    dAcOScatterSand_tmp *ac = mObjRef.get();

    if (ac != nullptr) {
        if (ac != nullptr && (ac->getSomeScale() * 100.0f) <= 40.0f && !ac->field_0x6E4) {
            ac->field_0x6E4 = true;
            ac->field_0x6DC = 10;
        }
    } else {
        if (field_0x5F3 != 0) {
            bool doIt = false;
            if ((mOnSceneFlag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mOnSceneFlag))) {
                doIt = true;
            }
            if (!doIt) {
                mStateMgr.changeState(StateID_On);
            }
        }
    }
}
void dAcOsw_c::finalizeState_OnWait() {}

void dAcOsw_c::initializeState_On() {
    startSound(SE_Sw_SLIDE);
}
void dAcOsw_c::executeState_On() {
    if (sLib::chase(&mButtonCtrl.mElevation, -20.0f, 2.0f)) {
        mStateMgr.changeState(StateID_OffWait);
    }
}

void dAcOsw_c::finalizeState_On() {
    startSound(SE_Sw_ON);
    dRumble_c::start(dRumble_c::sRumblePreset2, 0x1);
}

void dAcOsw_c::initializeState_OffWait() {
    if (mOffSceneFlag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mOffSceneFlag)) {
        SceneflagManager::sInstance->setFlag(mRoomID, mOffSceneFlag);
    }
    field_0x5EC = 30;
    mButtonCtrl.mElevation = -20.0f;
}
void dAcOsw_c::executeState_OffWait() {
    if (field_0x5F1 != 0) {
        field_0x5F3 = 1;
    }
    if (mSwitchType != 1 && sLib::calcTimer(&field_0x5EC) == 0 && field_0x5F3 == 0) {
        mStateMgr.changeState(StateID_Off);
    }
}
void dAcOsw_c::finalizeState_OffWait() {}

void dAcOsw_c::initializeState_Off() {
    startSound(SE_Sw_SLIDE);
}
void dAcOsw_c::executeState_Off() {
    if (sLib::chase(&mButtonCtrl.mElevation, 0.0f, 2.0f)) {
        mStateMgr.changeState(StateID_OnWait);
    }
}
void dAcOsw_c::finalizeState_Off() {}

void dAcOsw_c::initializeState_None() {
    mScale.y = 0.8f;
    mButtonCtrl.mElevation = -20.0f;
}
void dAcOsw_c::executeState_None() {}
void dAcOsw_c::finalizeState_None() {}
