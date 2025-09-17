#include "d/a/npc/d_a_npc.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/npc/d_a_npc tke.h"
#include "d/d_message.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "f/f_list_mg.h"
#include "f/f_profile_name.h"
#include "m/m3d/m3d.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/ut/ut_Color.h"
#include "rvl/MTX/mtx.h"
#include "s/s_Math.h"
#include "s/s_StateID.hpp"
#include "sized_string.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/event_manager.h"

fLiMgBa_c dAcNpc_c::NPC_ACTOR_LIST;

const char *dAcNpc_c::sHeadNodeName = "Head";
const char *dAcNpc_c::sNeckNodeName = "Neck";
const char *dAcNpc_c::sSpine2NodeName = "Spine2";

bool dFlowNpc_c::handleEventInternal(const MsbFlowInfo *element) {
    switch (element->param3) {
        case EVENT_SET_ITEM: {
            if (mpOwner->doGiveItem(element->param3, element->params1n2)) {
                setField0x3C();
            }
            return true;
        }
        case EVENT_EXIT: {
            u16 exitId, trial;
            extract2xU16Params(element, &exitId, &trial);
            if (field_0x10) {
                dAcBase_c *ac = EventManager::getMainActorInEvent();
                if (ac != nullptr && ac->profile_name == fProfile::NPC_TKE) {
                    static_cast<daNpcTke_c *>(ac)->setFinishFlags();
                } else if (mpOwner->profile_name == fProfile::NPC_TKE) {
                    static_cast<daNpcTke_c *>(mpOwner)->setFinishFlags();
                }
            }
            sExitId = exitId;
            if (trial == 1) {
                dScGame_c::GetInstance()->triggerExit(
                    dStage_c::GetInstance()->getCurrRoomId(), sExitId, SpawnInfo::RETAIN_TOD, SpawnInfo::TRIAL
                );
            } else {
                dScGame_c::GetInstance()->triggerExit(
                    dStage_c::GetInstance()->getCurrRoomId(), sExitId, SpawnInfo::RETAIN_TOD, SpawnInfo::RETAIN_TRIAL
                );
            }
            return false;
        }
        // TODO - more cases
        default: {
            return dFlow_c::handleEventInternal(element);
        }
    }
}

u16 dFlowNpc_c::getSwitchChoice(const MsbFlowInfo *element, u16 param) const {
    u16 result = 0;
    dAcNpc_c *npc = mpOwner;
    u32 arg = element->params1n2;
    switch (param) {
        case BRANCH_NPC_1:
            if (npc != nullptr) {
                result = npc->doFlowSwitch1(arg);
            }
            break;
        case BRANCH_NPC_2:
            if (npc != nullptr) {
                result = npc->doFlowSwitch2(arg);
            }
            break;
        case BRANCH_NPC_3:
            if (npc != nullptr) {
                result = npc->doFlowSwitch3(arg);
            }
            break;
        default: result = dFlow_c::getSwitchChoice(element, param); break;
    }

    return result;
}

void dFlowNpc_c::triggerEntryPoint(s32 labelPart1, s32 labelPart2) {
    setupActorRefs();
    dFlow_c::triggerEntryPoint(labelPart1, labelPart2);
}

void dFlowNpc_c::triggerEntryPoint(const char *label) {
    setupActorRefs();
    dFlow_c::triggerEntryPoint(label);
}

void dFlowNpc_c::setupActorRefs() {
    mObjRefs[0].link(mpOwner);
    for (int i = 1; i < (int)ARRAY_LENGTH(mObjRefs); i++) {
        mObjRefs[i].unlink();
    }

    for (int i = 0; i < ARRAY_LENGTH(field_0x128); i++) {
        field_0x128[i] = 0;
    }
    field_0x138 = 1;
    field_0x13C = 0;
}

STATE_VIRTUAL_DEFINE(dAcNpc_c, Wait);
STATE_VIRTUAL_DEFINE(dAcNpc_c, Demo);

static const nw4r::ut::Color sColors0[] = {
    nw4r::ut::Color(0xEF, 0x30, 0x27, 0xFF),
    nw4r::ut::Color(0x00, 0x00, 0x00, 0xFF),
    nw4r::ut::Color(0xEF, 0x30, 0x27, 0x00),
};
static const nw4r::ut::Color sColors1[] = {
    nw4r::ut::Color(0xFF, 0xFF, 0xFF, 0x00),
    nw4r::ut::Color(0xFE, 0xF8, 0x9D, 0xFF),
    nw4r::ut::Color(0x88, 0x63, 0x36, 0x80),
};
static const nw4r::ut::Color sColors2[] = {
    nw4r::ut::Color(0x4F, 0x46, 0x33, 0xFF),
    nw4r::ut::Color(0xD8, 0xC5, 0x8D, 0xFF),
    nw4r::ut::Color(0x95, 0x69, 0x3B, 0xFF),
};
static const nw4r::ut::Color sColors3[] = {
    nw4r::ut::Color(0x50, 0x46, 0x32, 0x00),
    nw4r::ut::Color(0xD8, 0xC5, 0x8D, 0x00),
    nw4r::ut::Color(0x96, 0x69, 0x3C, 0x00),
};

SizedString<128> sNpcStr1;
SizedString<128> sNpcStr2;
SizedString<128> sNpcStr3;
SizedString<128> sNpcStr4;

dAcNpc_c::dAcNpc_c() : mFlow(this), mStateMgr(*this, sStateID::null), mActorListEntry(this) {}

dAcNpc_c::~dAcNpc_c() {
    // TODO
}

u16 dAcNpc_c::doFlowSwitch1(u32 arg) {
    return eventFlowSwitch1(arg);
}

u16 dAcNpc_c::doFlowSwitch2(u32 arg) {
    return eventFlowSwitch2(arg);
}

u16 dAcNpc_c::doFlowSwitch3(u32 arg) {
    return eventFlowSwitch3(arg);
}

bool dAcNpc_c::addHeadNeckSpineCallbacks(
    d3d::AnmMdlWrapper *mdl, dNpcMdlCallbackMulti_c *multi, dNpcMdlCallbackBase_c *head, dNpcMdlCallbackBase_c *neck,
    dNpcMdlCallbackBase_c *spine
) {
    if (!multi->addCallback(head)) {
        return false;
    }
    head->loadNodeId(mdl, sHeadNodeName);

    if (!multi->addCallback(neck)) {
        return false;
    }
    neck->loadNodeId(mdl, sNeckNodeName);

    if (!multi->addCallback(spine)) {
        return false;
    }
    spine->loadNodeId(mdl, sSpine2NodeName);

    return true;
}

static u32 findNodeId(d3d::AnmMdlWrapper *mdl, const char *nodeName) {
    return m3d::getNodeID(mdl->getModel().getResMdl(), nodeName);
}

void dNpcMdlCallbackBase_c::loadNodeId(d3d::AnmMdlWrapper *mdl, const char *nodeName) {
    mNodeId = findNodeId(mdl, nodeName);
}

void dNpcMdlCallbackBase_c::timingA(nw4r::g3d::ChrAnmResult *result) {
    // no-op
}

void dNpcMdlCallbackBase_c::timingB(mMtx_c *result) {
    mVec3_c v1(0.0f, 0.0f, 1.0f);
    mVec3_c v2(0.0f, 0.0f, 1.0f);
    MTXMultVecSR(*result, v2, v1);
    field_0x04 = cM::atan2s(-v1.y, v1.absXZ());
    field_0x06 = cM::atan2s(v1.x, v1.z);
    if (mForceCalc || !isDone()) {
        apply(result);
    }
}

dNpcMdlCallbackAng_c::dNpcMdlCallbackAng_c()
    : mTarget(0, 0, 0),
      mNumSteps(2),
      mMaxStepSizeYZ(768),
      mMinStepSizeYZ(205),
      mMaxStepSizeX(252),
      mMinStepSizeX(1),
      mCurrent(0, 0, 0) {
    mScaleZ = 1;
    mScaleX = 1;
    mScaleY = 1;
}

void dNpcMdlCallbackAng_c::setMinMaxStepSize(const mAng &max, const mAng &min) {
    mMaxStepSizeYZ = max;
    mMinStepSizeYZ = min;
    mMaxStepSizeX = max;
    mMinStepSizeX = min;
    if (max < min) {
        mMaxStepSizeYZ = min;
        mMaxStepSizeX = min;
    }
}

void dNpcMdlCallbackAng_c::setMinMaxStepSizeX(const mAng &max, const mAng &min) {
    mMaxStepSizeX = max;
    mMinStepSizeX = min;
    if (max < min) {
        mMaxStepSizeX = min;
    }
}

void dNpcMdlCallbackAng_c::reset() {
    s32 zero = 0;
    mTarget.set(zero, zero, zero);
    mCurrent.set(zero, zero, zero);
    mForceCalc = false;
}

void dNpcMdlCallbackAng_c::resetTarget() {
    s32 zero = 0;
    mTarget.set(zero, zero, zero);
    mForceCalc = false;
}

void dNpcMdlCallbackAng_c::setTarget(const mAng3_c &target) {
    mTarget = target;
    mForceCalc = true;
}

void dNpcMdlCallbackAng_c::setTargetNow(const mAng3_c &target) {
    mTarget = target;
    mCurrent = target;
    mForceCalc = true;
}

void dNpcMdlCallbackAng_c::finishTarget() {
    mCurrent = mTarget;
    mForceCalc = true;
}

void dNpcMdlCallbackAng_c::calc() {
    mAng3_c ang = mCurrent;

    ang.x.step(mTarget.x, mNumSteps, mMaxStepSizeX, mMinStepSizeX);
    ang.y.step(mTarget.y, mNumSteps, mMaxStepSizeYZ, mMinStepSizeYZ);
    ang.z.step(mTarget.z, mNumSteps, mMaxStepSizeYZ, mMinStepSizeYZ);

    mCurrent = ang;
}

void dNpcMdlCallbackYXZ_c::apply(mMtx_c *result) const {
    result->YrotM(mCurrent.y * getScaleY());
    result->XrotM(mCurrent.x * getScaleX());
    result->ZrotM(mCurrent.z * getScaleZ());
}

void dNpcMdlCallbackYXZFixed_c::apply(mMtx_c *result) const {
    mMtx_c tmp;
    mVec3_c trans;

    result->getTranslation(trans);

    // Yes, this + is needed and sort of makes sense with
    // the symmetry below...
    tmp.YrotS(+mActorRotation.y);
    tmp.YrotM(mCurrent.y * getScaleY());
    tmp.XrotM(mCurrent.x * getScaleX());
    tmp.ZrotM(mCurrent.z * getScaleZ());
    tmp.YrotM(-mActorRotation.y);

    MTXConcat(tmp, *result, *result);

    result->setTranslation(trans);
}

void dNpcMdlCallbackYXZFixed_c::vt_0x20(const dAcBase_c *ac) {
    mActorRotation = ac->mRotation;
}

void dNpcMdlCallbackYZX_c::apply(mMtx_c *result) const {
    result->YrotM(mCurrent.y * getScaleY());
    result->ZrotM(mCurrent.x * getScaleX());
    result->XrotM(mCurrent.z * getScaleZ());
}

void dNpcMdlCallbackXZY_c::apply(mMtx_c *result) const {
    result->XrotM(mCurrent.y * getScaleY());
    result->ZrotM(mCurrent.x * getScaleX());
    result->YrotM(mCurrent.z * getScaleZ());
}

void dNpcMdlCallbackQuat_c::calc() {
    if (mSlerpFactor < 1.0f) {
        sLib::addCalc(&mSlerpFactor, 1.0f, mRatio, mMaxStepSize, mMinStepSize);
    } else {
        mSlerpFactor = 1.0f;
    }
}

void dNpcMdlCallbackQuat_c::apply(mMtx_c *result) const {
    mMtx_c quatMtx;
    mQuat_c resultQuat;
    mVec3_c trans;
    mMtx_c tmp;

    mStart.slerpTo(mTarget, mSlerpFactor, resultQuat);
    quatMtx.fromQuat(resultQuat);

    result->getTranslation(trans);

    tmp.YrotS(*mActorRotY);
    MTXConcat(tmp, quatMtx, tmp);
    tmp.YrotM(-*mActorRotY);

    MTXConcat(tmp, *result, *result);

    result->setTranslation(trans);
}

dNpcMdlCallbackNode *dNpcMdlCallbackMulti_c::newCallbackNode() {
    dNpcMdlCallbackNode *node = new dNpcMdlCallbackNode;
    if (node == nullptr) {
        return nullptr;
    }
    node->mpNext = nullptr;
    return node;
}

bool dNpcMdlCallbackMulti_c::createCallbackNode(dNpcMdlCallbackBase_c *cb) {
    dNpcMdlCallbackNode *node = newCallbackNode();
    if (node == nullptr) {
        return false;
    }
    mpTail = node;
    node->mpCallback = cb;
    return true;
}

void dNpcMdlCallbackMulti_c::clearList() {
    dNpcMdlCallbackNode *node = mpHead;
    dNpcMdlCallbackNode *next;
    if (node != nullptr) {
        while (node != nullptr) {
            next = node->mpNext;
            delete node;
            node = next;
        }
    }
}

bool dNpcMdlCallbackMulti_c::addCallback(dNpcMdlCallbackBase_c *callback) {
    if (mpHead == nullptr) {
        if (!createCallbackNode(callback)) {
            return false;
        }
        mpHead = mpTail;
    } else {
        dNpcMdlCallbackNode *prev = mpTail;
        if (!createCallbackNode(callback)) {
            return false;
        }
        prev->mpNext = mpTail;
    }
    mNumNodes++;
    return true;
}

void dNpcMdlCallbackMulti_c::timingA(u32 nodeId, nw4r::g3d::ChrAnmResult *result, nw4r::g3d::ResMdl mdl) {
    if (mpHead == nullptr) {
        return;
    }
    dNpcMdlCallbackNode *node = mpHead;
    while (node != nullptr) {
        if (nodeId == node->mpCallback->getNodeId()) {
            node->mpCallback->timingA(result);
            return;
        }
        node = node->mpNext;
    }
}

void dNpcMdlCallbackMulti_c::timingB(u32 nodeId, nw4r::g3d::WorldMtxManip *manip, nw4r::g3d::ResMdl mdl) {
    if (mpHead == nullptr) {
        return;
    }
    dNpcMdlCallbackNode *node = mpHead;
    while (node != nullptr) {
        if (nodeId == node->mpCallback->getNodeId()) {
            node->mpCallback->calc();
            mMtx_c mtx;
            manip->GetMtx(mtx);
            node->mpCallback->timingB(&mtx);
            manip->SetMtx(mtx);
            return;
        }
        node = node->mpNext;
    }
}
