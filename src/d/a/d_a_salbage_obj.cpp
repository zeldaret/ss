#include "d/a/d_a_salbage_obj.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/npc/d_a_npc_salbage_robot.h"
#include "d/a/npc/d_a_npc_talk_kensei.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/cc/d_cc_s.h"
#include "f/f_profile_name.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/dowsing_target.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/salvage_mgr.h"

STATE_VIRTUAL_DEFINE(dAcSalbageObj_c, Wait);
STATE_VIRTUAL_DEFINE(dAcSalbageObj_c, Carried);
STATE_VIRTUAL_DEFINE(dAcSalbageObj_c, Demo);
STATE_VIRTUAL_DEFINE(dAcSalbageObj_c, DemoThrow);
STATE_VIRTUAL_DEFINE(dAcSalbageObj_c, Fly);
STATE_VIRTUAL_DEFINE(dAcSalbageObj_c, After);
STATE_DEFINE(dAcSalbageObj_c, Kill);

static dCcD_SrcCyl sCylSrc = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {~0, 0x111, {0, 0x02, 0x407}, 0, 0},
     /* mObjCo */ {0xE9}},
    /* mCylInf */
    {0.0f, 0.0f}
};

static const InteractionTargetDef sInteractionTargetDefs[] = {
    {0, 0, 1, EXAMINE_TALK, 0, 250.0f, 60.0f, 60.0f,  -50.0f, 150.0f, 50.0f, 1.0f},
    {0, 0, 1, EXAMINE_TALK, 0, 250.0f, 60.0f,  0.0f,  -50.0f, 150.0f, 50.0f, 1.0f},
    {0, 0, 1, EXAMINE_TALK, 0, 250.0f, 60.0f,  0.0f, -750.0f, 500.0f, 50.0f, 1.0f},
    {0, 0, 1, EXAMINE_TALK, 0, 250.0f, 60.0f,  0.0f,  -50.0f, 150.0f, 50.0f, 1.0f},
    {0, 0, 1, EXAMINE_TALK, 0, 250.0f, 60.0f,  0.0f,  -50.0f, 150.0f, 50.0f, 1.0f},
    {0, 2, 1, EXAMINE_TALK, 0, 250.0f, 45.0f,  0.0f,  -50.0f, 150.0f, 50.0f, 1.0f},
    {0, 0, 1, EXAMINE_TALK, 0, 250.0f, 60.0f,  0.0f,  -50.0f, 150.0f, 50.0f, 1.0f},
};

void SalbageRelated::doDemoThrow() {
    mpObj->mStateMgr.changeState(dAcSalbageObj_c::StateID_DemoThrow);
}

int dAcSalbageObj_c::actorExecute() {
    executeInternal();
    calcVelocity();
    position += velocity;
    position += mStts.GetCcMove();
    if (isInStateWait()) {
        updateCc();
    }
    if (isInStateWaitOrDemoThrow()) {
        updateBgAcchCir();
    }
    updateMdl();
    return SUCCEEDED;
}

int dAcSalbageObj_c::preExecute() {
    // TODO - this matches, but so does `bool` conversion...
    int result = dAcObjBase_c::preExecute() == NOT_READY ? NOT_READY : SUCCEEDED;
    if (dSalvageMgr_c::sInstance->checkDeliveredStoryflag(mSalbageRelated.getSalvageObjId())) {
        s32 id = mSalbageRelated.getSalvageObjId();
        if (id != SALVAGE_OBJ_DIVINER_CRYSTAL && id != SALVAGE_OBJ_POT) {
            if (!EventManager::isInEvent()) {
                deleteRequest();
            }
            return NOT_READY;
        }
    }

    if (mBehavior == BEHAVIOR_STATIONARY) {
        if (dSalvageMgr_c::sInstance->getCurrentSalvageObjId() == mSalbageRelated.getSalvageObjId()) {
            if (!EventManager::isInEvent()) {
                deleteRequest();
            }
            return NOT_READY;
        }
    }

    if (shouldBeActiveDowsingTarget()) {
        if (mIsDowsingRegistered == false) {
            mDowsingTarget.doRegister();
            mIsDowsingRegistered = true;
        }
    } else {
        if (mIsDowsingRegistered == true) {
            mDowsingTarget.doUnregister();
            mIsDowsingRegistered = false;
        }
    }

    getDowsingUnusedF32();
    field_0x95C.set(0.0f, getDowsingTargetYOffset(), 0.0f);
    mDowsingTarget.setOffset(field_0x95C);

    return result;
}

int dAcSalbageObj_c::preDraw() {
    if (dAcObjBase_c::preDraw() == NOT_READY) {
        return NOT_READY;
    } else {
        return mSalbageRelated.isHidden() ? NOT_READY : SUCCEEDED;
    }
}

int dAcSalbageObj_c::draw() {
    drawModelType1(mpMdl);
    fn_8002edb0(mShadow, *mpMdl, &mShadowRot, -1, -1, field_0x900);
    return SUCCEEDED;
}

void dAcSalbageObj_c::executeInternal() {
    if (!EventManager::isInEvent()) {
        if (mIsDemoState == true) {
            mIsDemoState = false;
            if (mpNextStateId != nullptr) {
                mStateMgr.changeState(*mpNextStateId);
            } else {
                mStateMgr.changeState(StateID_Wait);
            }
        }
    } else {
        if (mIsDemoState == false) {
            mIsDemoState = true;
            mStateMgr.changeState(StateID_Demo);
        }
    }
    mStateMgr.executeState();
}

void dAcSalbageObj_c::initSalbageObj() {
    mShadowRot.v = mVec3_c(0.0f, 90.0f, 0.0f);
    mShadowRot.w = 180.0f;
    mScale.set(1.0f, 1.0f, 1.0f);
    mBehavior = getFromParams(4, 0xF);
    mpSalbageRelated = &mSalbageRelated;
    initCcAndBg();
    if (dSalvageMgr_c::sInstance->checkDeliveredStoryflag(mSalbageRelated.getSalvageObjId())) {
        if (mBehavior == BEHAVIOR_CARRY && vt_0x80()) {
            mStateMgr.changeState(StateID_After);
        } else {
            mStateMgr.changeState(StateID_Kill);
        }
    } else {
        switch (mBehavior) {
            case BEHAVIOR_CARRY: {
                forwardSpeed = 0.0f;
                velocity.set(0.0f, 0.0f, 0.0f);
                forwardAccel = 0.0f;
                mSalbageRelated.setHidden();
                if (dSalvageMgr_c::sInstance->getCurrentSalvageObjId() != mSalbageRelated.getSalvageObjId()) {
                    mStateMgr.changeState(StateID_Kill);
                    return;
                } else {
                    mSalbageRelated.setCarried();
                    mStateMgr.changeState(StateID_Carried);
                }
                break;
            }
            case BEHAVIOR_FLY: {
                forwardSpeed = 0.0f;
                velocity.set(0.0f, 0.0f, 0.0f);
                forwardAccel = 0.0f;
                mStateMgr.changeState(StateID_Fly);
                break;
            }
            default: {
                if (dSalvageMgr_c::sInstance->getCurrentSalvageObjId() == mSalbageRelated.getSalvageObjId()) {
                    mStateMgr.changeState(StateID_Kill);
                    return;
                } else {
                    mBehavior = BEHAVIOR_STATIONARY;
                    forwardAccel = -1.0f;
                    forwardMaxSpeed = -40.0f;
                    initCcAndBg();
                    forwardSpeed = 0.0f;
                    forwardAccel = 0.0f;
                    velocity.set(0.0f, 0.0f, 0.0f);
                    mStateMgr.changeState(StateID_Wait);
                }
                break;
            }
        }
        dSalvageMgr_c::sInstance->mObjRef.link(this);
    }
}

void dAcSalbageObj_c::doInteraction(s32 arg) {
    if (arg == 4 || arg == 5 || arg == 12) {
        if (mStateMgr.isState(StateID_Wait) && dSalvageMgr_c::sInstance->hasObjAndSlb()) {
            dAcNpcTalkKensei_c::GetInstance()->triggerSalvageQuestion(this);
        }
    }
}

void dAcSalbageObj_c::addAttentionTargetIfNeeded() {
    switch (mSalbageRelated.getSalvageObjId()) {
        case SALVAGE_OBJ_WATER_JAR:
        case SALVAGE_OBJ_POT:       return;
        default:                    addAttentionTarget(); break;
    }
}

void dAcSalbageObj_c::addAttentionTarget() {
    AttentionManager::sInstance->addTarget(
        *this, sInteractionTargetDefs[mSalbageRelated.getSalvageObjId()], 0, nullptr
    );

    InteractionTargetDef copy = sInteractionTargetDefs[mSalbageRelated.getSalvageObjId()];
    copy.field_0x00 = 1;
    AttentionManager::sInstance->addTarget(*this, copy, 0, nullptr);
}

void dAcSalbageObj_c::initCcAndBg() {
    mBgObjAcch.Set(this, 1, &mBgAcchCir);
    mBgObjAcch.SetWaterNone();
    mBgObjAcch.SetRoofNone();
    mBgObjAcch.SetGndThinCellingOff();
    mBgAcchCir.SetWall(0.0f, 100.0f);
    mStts.SetDefaultRank();
    mCcCyl.Set(sCylSrc);
    mCcCyl.SetStts(mStts);
}

bool dAcSalbageObj_c::shouldBeActiveDowsingTarget() const {
    if (mBehavior != BEHAVIOR_STATIONARY) {
        return false;
    }

    if (dSalvageMgr_c::sInstance->getCurrentSalvageObjId() == mSalbageRelated.getSalvageObjId()) {
        return false;
    }

    if (dSalvageMgr_c::sInstance->hasDowsingForSalvageObj(this) == false) {
        return false;
    } else {
        return mSalbageRelated.getSalvageObjId() != SALVAGE_OBJ_POT;
    }
}

void dAcSalbageObj_c::updateCc() {
    mCcCyl.SetC(position);
    mCcCyl.SetR(getCcRadius());
    mCcCyl.SetH(getCcHeight());
    dCcS::GetInstance()->Set(&mCcCyl);
}

void dAcSalbageObj_c::updateBgAcchCir() {
    mBgAcchCir.SetWall(getBgAcchCirHeight(), getBgAcchCirRadius());
    mBgObjAcch.CrrPos(*dBgS::GetInstance());
}

void dAcSalbageObj_c::calcMtxFromSalbageNpc(mMtx_c &ret) {
    dAcNpcSlb_c *npc = dSalvageMgr_c::sInstance->mSlbRef.get();
    if (npc == nullptr) {
        ret.transS(position.x, position.y, position.z);
        ret.ZXYrotM(rotation);
    } else {
        ret = npc->getCarriedObjMtx();
        mMtx_c rotMtx = dSalvageMgr_c::sInstance->getCarryRotMtx2(mSalbageRelated.getSalvageObjId());
        MTXConcat(ret, rotMtx, ret);
        ret.getTranslation(position);
    }
}

mMtx_c dAcSalbageObj_c::calcWorldMtx() {
    mMtx_c ret;
    if (mSalbageRelated.isCarried()) {
        calcMtxFromSalbageNpc(ret);
    } else {
        ret.transS(position);
        ret.ZXYrotM(rotation);
    }

    return ret;
}

void dAcSalbageObj_c::updateMdl() {
    mWorldMtx = calcWorldMtx();
    mpMdl->setLocalMtx(mWorldMtx);
    mpMdl->setScale(mScale);
    mpMdl->calc(false);
    field_0x900 = 0.0f;

    mVec3_c translate;
    mWorldMtx.getTranslation(translate);
    if (dBgS_ObjGndChk::CheckPos(translate)) {
        f32 diff = translate.y - dBgS_ObjGndChk::GetGroundHeight();
        if (diff >= 0.0f && diff <= 850.0f) {
            field_0x900 = diff;
        }
    }

    f32 scale = mScale.x;
    poscopy3 = position;
    poscopy3.y += getPoscopy3YOffset() * scale;
    poscopy2 = position;
    poscopy2.y += getPoscopy2YOffset() * scale;
}

void dAcSalbageObj_c::loadBehaviorFromParams() {
    mBehavior = getFromParams(4, 0xF);
}

void dAcSalbageObj_c::initializeState_Wait() {
    mDowsingTarget.initialize(getDowsingSlot(), 0, &field_0x95C, getDowsingUnusedF32());
    mDowsingTarget.doRegister();
    mIsDowsingRegistered = true;
}
void dAcSalbageObj_c::executeState_Wait() {
    if (dSalvageMgr_c::sInstance->getCurrentSalvageObjId() != mSalbageRelated.getSalvageObjId()) {
        if (!dSalvageMgr_c::sInstance->mSlbRef.isLinked()) {
            mVec3_c pos(position.x, position.y + 100000.0f, position.z);
            dAcObjBase_c::create(fProfile::NPC_SLB, roomid, 0xFFFFFD01, &pos, nullptr, nullptr, -1);
        }

        if (dSalvageMgr_c::sInstance->startedQuestForSalvageObj(this)) {
            addAttentionTargetIfNeeded();
        }
    }
}
void dAcSalbageObj_c::finalizeState_Wait() {}

void dAcSalbageObj_c::initializeState_Carried() {
    // Return to "carried" after demo ends
    mpNextStateId = &StateID_Carried;
}
void dAcSalbageObj_c::executeState_Carried() {}
void dAcSalbageObj_c::finalizeState_Carried() {}

void dAcSalbageObj_c::initializeState_Fly() {}
void dAcSalbageObj_c::executeState_Fly() {}
void dAcSalbageObj_c::finalizeState_Fly() {}

void dAcSalbageObj_c::initializeState_After() {
    dSalvageMgr_c::sInstance->deleteObjAndSlb();
}
void dAcSalbageObj_c::executeState_After() {
    dSalvageMgr_c::sInstance->deleteObjAndSlb();
}
void dAcSalbageObj_c::finalizeState_After() {}

void dAcSalbageObj_c::initializeState_Demo() {}
void dAcSalbageObj_c::executeState_Demo() {}
void dAcSalbageObj_c::finalizeState_Demo() {}

void dAcSalbageObj_c::initializeState_DemoThrow() {
    mWorldSRMtx = mWorldMtx;
    mWorldSRMtx.m[0][3] = 0.0f;
    mWorldSRMtx.m[1][3] = 0.0f;
    mWorldSRMtx.m[2][3] = 0.0f;

    mWorldMtx.getTranslation(position);
    field_0x938 = 0.0f;

    mVec3_c result;
    MTXMultVecSR(mWorldSRMtx, mVec3_c::Ez, result);
    rotation.y = result.atan2sX_Z();
    rotation.x = -cM::atan2s(result.y, result.absXZ());
    rotation.z = field_0x904;
    angle.x = 0;
    angle.y = rotation.y;
    angle.z = 0;
    forwardMaxSpeed = -40.0f;
    mBgObjAcch.Set(this, 1, &mBgAcchCir);
    mBgObjAcch.SetWaterNone();
    mBgObjAcch.SetRoofNone();
    mBgObjAcch.SetGndThinCellingOff();
    mBgAcchCir.SetWall(0.0f, 100.0f);
    mSalbageRelated.setNotCarried();
}
void dAcSalbageObj_c::executeState_DemoThrow() {}
void dAcSalbageObj_c::finalizeState_DemoThrow() {}

void dAcSalbageObj_c::initializeState_Kill() {
    deleteRequest();
}
void dAcSalbageObj_c::executeState_Kill() {
    deleteRequest();
}
void dAcSalbageObj_c::finalizeState_Kill() {}
