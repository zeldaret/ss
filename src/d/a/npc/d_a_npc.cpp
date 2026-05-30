#include "d/a/npc/d_a_npc.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/npc/d_a_npc tke.h"
#include "d/a/npc/d_a_npc_inv.h"
#include "d/d_message.h"
#include "d/d_room.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "f/f_list_mg.h"
#include "f/f_profile_name.h"
#include "m/m3d/m3d.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_fanm.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/ut/ut_Color.h"
#include "rvl/MTX/mtx.h"
#include "s/s_Math.h"
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
                if (ac != nullptr && ac->mProfileName == fProfile::NPC_TKE) {
                    static_cast<daNpcTke_c *>(ac)->setFinishFlags();
                } else if (mpOwner->mProfileName == fProfile::NPC_TKE) {
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

dAcObjBase_c *dFlowNpc_c::getActorForId(s32 flowNpcId) {
    if (flowNpcId == FLOW_LINK) {
        return dAcPy_c::GetLinkM();
    } else {
        return getObj(getIdx(flowNpcId));
    }
}

u16 dFlowNpc_c::flowNpcIdToProfile(s32 flowNpcId) {
    switch (flowNpcId) {
        case FLOW_NPC_ZLD:                return fProfile::NPC_ZLD;
        case FLOW_NPC_DSK:                return fProfile::NPC_DSK;
        case FLOW_NPC_TALK_KENSEI:        return fProfile::NPC_TALK_KENSEI;
        case FLOW_NPC_RVL:                return fProfile::NPC_RVL;
        case FLOW_NPC_KBN:                return fProfile::NPC_KBN;
        case FLOW_NPC_KBN2:               return fProfile::NPC_KBN2;
        case FLOW_NPC_SORAJIMA_MALE:      return fProfile::NPC_SORAJIMA_MALE;
        case FLOW_NPC_SORAJIMA_FEMALE:    return fProfile::NPC_SORAJIMA_FEMALE;
        case FLOW_NPC_SKN:                return fProfile::NPC_SKN;
        case FLOW_NPC_SKN2:               return fProfile::NPC_SKN2;
        case FLOW_NPC_KNIGHT_LEADER:      return fProfile::NPC_KNIGHT_LEADER;
        case FLOW_NPC_CE_FRIEND:          return fProfile::NPC_CE_FRIEND;
        case FLOW_NPC_GHM:                return fProfile::NPC_GHM;
        case FLOW_NPC_OIM:                return fProfile::NPC_OIM;
        case FLOW_NPC_YIM:                return fProfile::NPC_YIM;
        case FLOW_NPC_MOLE_NORMAL:        return fProfile::NPC_MOLE_NORMAL;
        case FLOW_NPC_MOLE_NORMAL2:       return fProfile::NPC_MOLE_NORMAL2;
        case FLOW_NPC_SENPAI:             return fProfile::NPC_SENPAI;
        case FLOW_NPC_DRB:                return fProfile::NPC_DRB;
        case FLOW_NPC_SENPAI_B:           return fProfile::NPC_SENPAI_B;
        case FLOW_NPC_SORAJIMA_MOTHER:    return fProfile::NPC_SORAJIMA_MOTHER;
        case FLOW_NPC_SLRP:               return fProfile::NPC_SLRP;
        case FLOW_NPC_SLB:                return fProfile::NPC_SLB;
        case FLOW_NPC_GRA:                return fProfile::NPC_GRA;
        case FLOW_NPC_MOLE_TACKLE2:       return fProfile::NPC_MOLE_TACKLE2;
        case FLOW_NPC_PDU:                return fProfile::NPC_PDU;
        case FLOW_NPC_SMA2:               return fProfile::NPC_SMA2;
        case FLOW_NPC_MED_HUS_NIGHT:      return fProfile::NPC_MED_HUS_NIGHT;
        case FLOW_NPC_MED_WIFE_NIGHT:     return fProfile::NPC_MED_WIFE_NIGHT;
        case FLOW_NPC_DOUGUYA_NIGHT:      return fProfile::NPC_DOUGUYA_NIGHT;
        case FLOW_NPC_DOUGUYA_MOTHER:     return fProfile::NPC_DOUGUYA_MOTHER;
        case FLOW_NPC_AZUKARIYA_NIGHT:    return fProfile::NPC_AZUKARIYA_NIGHT;
        case FLOW_NPC_AZUKARIYA_FATHER:   return fProfile::NPC_AZUKARIYA_FATHER;
        case FLOW_NPC_SENPAIA_MOTHER:     return fProfile::NPC_SENPAIA_MOTHER;
        case FLOW_NPC_AKU_HUMAN:          return fProfile::NPC_AKU_HUMAN;
        case FLOW_NPC_SALBAGE_MORRY:      return fProfile::NPC_SALBAGE_MORRY;
        case FLOW_NPC_DOUGUYA_MOTHER_LOD: return fProfile::NPC_DOUGUYA_MOTHER_LOD;
        case FLOW_NPC_JUNK_MOTHER_LOD:    return fProfile::NPC_JUNK_MOTHER_LOD;
        case FLOW_NPC_SENPAIA_MOTHER_LOD: return fProfile::NPC_SENPAIA_MOTHER_LOD;
        case FLOW_NPC_BBRVL:              return fProfile::NPC_BBRVL;
        case FLOW_NPC_SORAJIMA_MAN_D:     return fProfile::NPC_SORAJIMA_MAN_D;
        case FLOW_NPC_SORAJIMA_MAN_E:     return fProfile::NPC_SORAJIMA_MAN_E;
        case FLOW_NPC_DAISHINKAN_N:       return fProfile::NPC_DAISHINKAN_N;
        case FLOW_NPC_PMA:                return fProfile::NPC_PMA;
        case FLOW_NPC_DRBC:               return fProfile::NPC_DRBC;
        case FLOW_NPC_MOLE_MG:            return fProfile::NPC_MOLE_MG;
        case FLOW_NPC_GRD:                return fProfile::NPC_GRD;
        case FLOW_NPC_GRC:                return fProfile::NPC_GRC;
        case FLOW_NPC_SLB2:               return fProfile::NPC_SLB2;
        case FLOW_NPC_CE_LADY:            return fProfile::NPC_CE_LADY;
        case FLOW_NPC_MOLE:               return fProfile::NPC_MOLE;
    }
    if (flowNpcId >= FLOW_NPC_INV_OFFSET) {
        return fProfile::NPC_INV;
    }
    return fProfile::PROFILE_MAX;
}

dAcObjBase_c *dFlowNpc_c::getObj(s32 idx) {
    if (0 <= idx && idx < field_0x138) {
        return mObjRefs[idx].get();
    }
    return nullptr;
}

s32 dFlowNpc_c::getIdx(s32 flowNpcId) {
    if (flowNpcId == FLOW_NPC_SELF) {
        return 0;
    }
    u16 profile = flowNpcIdToProfile(flowNpcId);
    if (profile == mpOwner->mProfileName) {
        return 0;
    }

    for (int i = 1; i < field_0x138; i++) {
        if (profile == mObjRefs[i].get()->mProfileName) {
            if (profile == fProfile::NPC_INV) {
                if (flowNpcId - FLOW_NPC_INV_OFFSET ==
                    static_cast<dAcNpcInv_c *>(mObjRefs[i].get())->getActualFlowNpcId()) {
                    return i;
                }
            } else {
                return i;
            }
        }
    }

    return -1;
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

void dAcNpc_c::loadMainPos(f32 offsetY1, f32 offsetY2) {
    mPositionCopy3 = mPosition;
    mPositionCopy3.y += offsetY1;
    mPositionCopy2 = mPosition;
    mPositionCopy2.y += offsetY2;
}

void dAcNpc_c::loadMainAndHeadPos(f32 posYOffset, f32 headYOffset, d3d::AnmMdlWrapper *mdl) {
    mPositionCopy3 = mPosition;
    mPositionCopy3.y += posYOffset;
    mMtx_c mtx;
    mdl->getModel().getNodeWorldMtx(mdl->getModel().getResMdl().GetResNode(sHeadNodeName).GetID(), mtx);
    mtx.getTranslation(mPositionCopy2);
    mPositionCopy2.y += headYOffset;
}

void dAcNpc_c::loadMainAndHeadPos(const mVec3_c *posOffset, const mVec3_c *headOffset, d3d::AnmMdlWrapper *mdl) {
    loadMainPos(posOffset);
    mMtx_c mtx;
    mdl->getModel().getNodeWorldMtx(mdl->getModel().getResMdl().GetResNode(sHeadNodeName).GetID(), mtx);
    mtx.getTranslation(mPositionCopy2);
    mPositionCopy2 += *headOffset;
}

dAcNpc_c::dAcNpc_c() : mSoundInfo(this), mFlow(this), mStateMgr(*this), mNpcListEntry(this) {
    addToNpcList();
}

dAcNpc_c::dAcNpc_c(dFlowMgrBase_c *flowMgr) : mSoundInfo(this), mFlow(this), mStateMgr(*this), mNpcListEntry(this) {
    addToNpcList();
}

void dAcNpc_c::loadMainPos(const mVec3_c *offset) {
    mVec3_c v = *offset;
    v.rotY(mRotation.y);
    mPositionCopy3 = mPosition + v;
}

dAcNpc_c::~dAcNpc_c() {
    NPC_ACTOR_LIST.remove(&mNpcListEntry);
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

bool dAcNpc_c::initHeadNeckSpineCallbacks(
    d3d::AnmMdlWrapper *mdl, dNpcMdlCallbackMulti_c *multi, dNpcMdlCallbackAng_c *head, dNpcMdlCallbackAng_c *neck,
    dNpcMdlCallbackAng_c *spine
) {
    if (!addHeadNeckSpineCallbacks(mdl, multi, head, neck, spine)) {
        return false;
    }
    mAng max = acNpc_vt_0x198();
    mAng min = acNpc_vt_0x19C();
    mAng maxX = acNpc_vt_0x190();
    mAng minX = acNpc_vt_0x194();

    head->setMinMaxStepSize(max, min);
    head->setMinMaxStepSizeX(maxX, minX);
    neck->setMinMaxStepSize(max, min);
    neck->setMinMaxStepSizeX(maxX, minX);
    spine->setMinMaxStepSize(max, min);
    spine->setMinMaxStepSizeX(maxX, minX);

    return true;
}

m3d::anmTexSrt_c *
dAcNpc_c::newAnmTexSrt(d3d::AnmMdlWrapper *mdl, mAllocator_c *allocator, const char *mdlName, const char *anmName) {
    m3d::anmTexSrt_c *ret = new m3d::anmTexSrt_c();
    if (!ret) {
        return nullptr;
    }
    if (!ret->create(
            mdl->getMdlFile().GetResMdl(mdlName), mdl->getMdlFile().GetResAnmTexSrt(anmName), allocator, nullptr, 1
        )) {
        delete ret;
        return nullptr;
    }
    ret->setFrame(0.0f, 0);
    mdl->getModel().setAnm(*ret);
    return ret;
}

void dAcNpc_c::setAnmTexSrt(d3d::AnmMdlWrapper *mdl, m3d::anmTexSrt_c *texSrt, const char *anmName) {
    texSrt->setAnm(mdl->getModel(), mdl->getMdlFile().GetResAnmTexSrt(anmName), 0, m3d::PLAY_MODE_4);
    texSrt->setFrame(0.0f, 0);
}

bool dAcNpc_c::createAnmChr(
    d3d::AnmMdlWrapper *mdl, const char *mdlName, const char *anmName, mAllocator_c *allocator, m3d::anmChr_c *anm
) {
    // TODO - what?
    return !!anm->create2(mdl->getMdlFile().GetResMdl(mdlName), mdl->getMdlFile().GetResAnmChr(anmName), allocator);
}

s32 dAcNpc_c::getPathIndexForId(s32 pathId, s32 roomId, bool spth) {
    dRoom_c *room = dStage_c::GetInstance()->getRoom(roomId);
    int ret = -1;
    if (spth) {
        for (int i = 0; i < room->getSpthCount(); i++) {
            if (room->getSpth(i)->pathId == pathId) {
                ret = i;
                break;
            }
        }
    } else {
        for (int i = 0; i < room->getPathCount(); i++) {
            if (room->getPath(i)->pathId == pathId) {
                ret = i;
                break;
            }
        }
    }
    return ret;
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

/**
 * @brief Clamps an angle such that bounds[1] <= result <= bounds[0] and result + return = value
 *
 * @param value the input value
 * @param bounds the max, min bounds
 * @param result the clamped value
 * @return mAng the excess
 */
extern mAng clampAngle(mAng &value, mAng *bounds, mAng &result) {
    // TODO instruction order
    mAng ret = value;

    if (bounds[1] <= ret && ret <= bounds[0]) {
        result = ret;
        ret = 0;
    } else if (ret > 0) {
        result = bounds[0];
        ret = ret - bounds[0];
    } else {
        result = bounds[1];
        ret = ret - bounds[1];
    }

    return ret;
}

extern void clampAngleXY(mAng3_c &value, mAng *boundsX, mAng *boundsY, mAng3_c &result) {
    clampAngle(value.x, boundsX, result.x);
    clampAngle(value.y, boundsY, result.y);
}
