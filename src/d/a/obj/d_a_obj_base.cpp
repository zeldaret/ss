#include "d/a/obj/d_a_obj_base.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/d_base.h"
#include "d/d_bzs_types.h"
#include "d/d_linkage.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "d/d_vec.h"
#include "d/t/d_t_view_clip.h"
#include "f/f_base.h"
#include "f/f_list_mg.h"
#include "f/f_list_nd.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/math/math_geometry.h"
#include "toBeSorted/actor_info.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/event_manager.h"

void *dAcObjBase_c::getOarcFile(const char *oarcName, const char *fileName) {
    return OarcManager::GetInstance()->getData(oarcName, fileName);
}

void *dAcObjBase_c::getOarcSubEntry(const char *oarcName, const char *fileName) {
    return OarcManager::GetInstance()->getSubEntryData(oarcName, fileName);
}

void *dAcObjBase_c::getOarcResFile(const char *oarcName) {
    return OarcManager::GetInstance()->getMdlFromArc2(oarcName);
}

void *dAcObjBase_c::getOarcModelFile(const char *oarcName) {
    return OarcManager::GetInstance()->getMdlFromArc(oarcName);
}

void *dAcObjBase_c::getOarcZev(const char *oarcName) {
    return OarcManager::GetInstance()->getZevFromArc(oarcName);
}

void *dAcObjBase_c::getOarcDZB(const char *oarcName, const char *dzbName) {
    return OarcManager::GetInstance()->getDzbFromArc(oarcName, dzbName);
}

void *dAcObjBase_c::getOarcPLC(const char *oarcName, const char *plcName) {
    return OarcManager::GetInstance()->getPlcFromArc(oarcName, plcName);
}

dAcObjBase_c::dAcObjBase_c()
    : mTargetFiTextID(0xFFFF),
      mCullingDistance(8000.0f),
      field_0x1A8(400.0f),
      field_0x1B0(-1e9f),
      field_0x1B4(0.0f, 1.0f, 0.0f),
      mStts(this),
      mStartingPos(mPosition.x, mPosition.y, mPosition.z),
      mStartingRot(mRotation) {
    mLightingInfo.reset();
    mpSalvageIf = nullptr;
    if (s_Create_Rotation != nullptr) {
        mAngle = *s_Create_Rotation;
    }
    if (mpActorInfo != nullptr) {
        mTargetFiTextID = mpActorInfo->fiTextEntryId;
    }
    if (dStageMgr_c::GetInstance()->getStif() != nullptr) {
        mCullingDistance = dStageMgr_c::GetInstance()->getStif()->defaultCullingDistance;
    }
}

dAcObjBase_c::~dAcObjBase_c() {}

int dAcObjBase_c::preCreate() {
    if (fBase_c::preCreate() == NOT_READY) {
        return NOT_READY;
    }

    mOldPosition = mPosition;
    mPositionCopy2 = mPosition;
    mPositionCopy3 = mPosition;

    return SUCCEEDED;
}

int dAcObjBase_c::preExecute() {
    bool notOk = fBase_c::preExecute() == NOT_READY;

    mLinkage.fn_80051190(this);

    unsetObjectProperty(OBJ_PROP_0x4000 | OBJ_PROP_0x400 | OBJ_PROP_0x80 | OBJ_PROP_0x20);

    if (checkObjectProperty(OBJ_PROP_0x40)) {
        setObjectProperty(OBJ_PROP_0x80);
    } else if (checkObjectProperty(OBJ_PROP_0x1)) {
        if (checkActorProperty(AC_PROP_0x1) && checkActorProperty(AC_PROP_0x10000000)) {
            // Event Manger Check
        }
    }

    if (notOk) {
        return NOT_READY;
    }

    if (!checkObjectProperty(OBJ_PROP_0x1)) {
        unsetObjectProperty(OBJ_PROP_0x40);
    }

    return SUCCEEDED;
}

void dAcObjBase_c::postExecute(MAIN_STATE_e state) {
    if (state != CANCELED) {
        mOldPosition = mPosition;
        mStts.Move();
        if (!mLinkage.checkState(dLinkage_c::STATE_ACTIVE)) {
            mLinkage.setState(dLinkage_c::STATE_0);
        }
        unsetObjectProperty(OBJ_PROP_0x8000 | OBJ_PROP_0x2000);
    }

    return dAcBase_c::postExecute(state);
}

extern "C" bool fn_801BB750(f32 f, nw4r::math::AABB &);

int dAcObjBase_c::preDraw() {
    bool notOk = false;
    if (dBase_c::preDraw() == NOT_READY) {
        notOk = true;
    }

    unsetObjectProperty(OBJ_PROP_0x2 | OBJ_PROP_0x1);

    mMtx_c m = mWorldMtx;
    m.scaleM(mScale);
    nw4r::math::AABB aabb;
    // UHHHH
    aabb.Set((nw4r::math::AABB *)(&mBoundingBox), m);

    f32 cull = dTgViewClip_c::getCull(mViewClipIdx, mCullingDistance);
    if (EventManager::checkFlag(0x20)) {
        cull = 3.402923e30f;
    }
    if (cull < 0.0f) {
        if (checkActorProperty(AC_PROP_0x1) && checkActorProperty(AC_PROP_0x2)) {
            setObjectProperty(OBJ_PROP_0x1 | OBJ_PROP_0x2);
            return NOT_READY;
        }
        cull = mCullingDistance;
    }
    if (fn_801BB750(cull, aabb)) {
        setObjectProperty(OBJ_PROP_0x2);
    }

    aabb.min.x -= field_0x1A8;
    aabb.min.y -= field_0x1A8;
    aabb.min.z -= field_0x1A8;

    aabb.max.x += field_0x1A8;
    aabb.max.y += field_0x1A8;
    aabb.max.z += field_0x1A8;

    if (fn_801BB750(cull, aabb)) {
        setObjectProperty(OBJ_PROP_0x1);
    }

    if (checkObjectProperty(OBJ_PROP_0x2) && checkActorProperty(AC_PROP_0x2)) {
        return NOT_READY;
    }

    if ((notOk || checkObjectProperty(OBJ_PROP_0x200)) ||
        (mLinkage.checkState(dLinkage_c::STATE_ACTIVE) &&
         ((dAcObjBase_c *)mLinkage.getControllingActor())->isPlayer() && mLinkage.getControllingActor() &&
         ((dAcObjBase_c *)mLinkage.getControllingActor())->checkObjectProperty(OBJ_PROP_0x200))) {
        return NOT_READY;
    }

    return SUCCEEDED;
}

fLiNdBa_c *dAcObjBase_c::getObjectListEntry() {
    return nullptr;
}

bool dAcObjBase_c::restorePosRotFromCopy() {
    if (!dAcBase_c::restorePosRotFromCopy()) {
        return false;
    }

    mOldPosition = mPosition;
    mAngle = mRotation;
    return true;
}

dAcObjBase_c *dAcObjBase_c::create(
    ProfileName profile, u32 roomID, u32 params, const mVec3_c *pPos, const mAng3_c *pRot, const mVec3_c *pScale,
    u32 params2
) {
    setTempCreateParams(
        (mVec3_c *)pPos, (mAng3_c *)pRot, (mVec3_c *)pScale, roomID, params2, nullptr, 0, 0xFFFF, 0xFF, nullptr
    );
    return static_cast<dAcObjBase_c *>(createBase(profile, dStage_c::getParentForRoom(roomID), params, ACTOR));
}

dAcObjBase_c *dAcObjBase_c::create(
    const char *name, u32 roomID, u32 params, const mVec3_c *pPos, const mAng3_c *pRot, const mVec3_c *pScale,
    u32 params2, u16 id, s8 viewclipId
) {
    const ActorInfo *pInfo = getActorInfoByName(name);
    setTempCreateParams(
        (mVec3_c *)pPos, (mAng3_c *)pRot, (mVec3_c *)pScale, roomID, params2, nullptr, pInfo->subtype, id, viewclipId,
        pInfo
    );
    return static_cast<dAcObjBase_c *>(createBase(pInfo->profileId, dStage_c::getParentForRoom(roomID), params, ACTOR));
}

dAcObjBase_c *dAcObjBase_c::findObject(ProfileName actorId, fLiMgBa_c *refList, dAcObjBase_c *parent) {
    fLiNdBa_c *pNode = parent != nullptr ? parent->getObjectListEntry() : nullptr;
    pNode = (fLiNdBa_c *)refList->searchNodeByProfName(actorId, pNode);
    return pNode != nullptr ? (dAcObjBase_c *)pNode->p_owner : nullptr;
}

dAcObjBase_c *dAcObjBase_c::getNextObject(fLiMgBa_c *ref, dAcObjBase_c *parent) {
    fLiNdBa_c *pNode = parent != nullptr ? parent->getObjectListEntry()->getNext() : ref->getFirst();

    return pNode != nullptr ? (dAcObjBase_c *)pNode->p_owner : nullptr;
}

bool dAcObjBase_c::isPlayer() {
    return this == dAcPy_c::GetLink();
}

void dAcObjBase_c::calcVelocity(const mVec3_c &pos, f32 speed) {
    // FIXME
    mVelocity.set((pos.x - mPosition.x) / speed, (pos.y - mPosition.y) / speed, (pos.z - mPosition.z) / speed);
    mVelocity.y += mAcceleration * speed / 2.0f;
}

void dAcObjBase_c::calcVelocity() {
    f32 f = nw4r::ut::Max(mVelocity.y + mAcceleration, mMaxSpeed);
    setXYZCirclePoint(mVelocity, mAngle.y, mSpeed, f);
}

void dAcObjBase_c::updateMatrix() {
    mWorldMtx.transS(mPosition.x, mPosition.y + getYOffset(), mPosition.z);
    mWorldMtx.ZXYrotM(mRotation);
}

void dAcObjBase_c::setDefaultCullDistance() {
    mCullingDistance = 8000.0f;
}

bool dAcObjBase_c::canBeLinkedToWoodTag() {
    return false;
}

bool dAcObjBase_c::drop() {
    return true;
}

void dAcObjBase_c::fn_8002EA30(f32 f0, f32 f1, f32 f2, f32 f3, f32 f4, dBgS_Acch *acch) {
    acch->OnWallSort();
}
