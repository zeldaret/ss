#include "d/a/obj/d_a_obj_base.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/col/bg/d_bg_pc.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "d/col/c/c_m3d_g_pla.h"
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
#include "f/f_profile.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/math/math_geometry.h"
#include "s/s_Math.h"
#include "toBeSorted/actor_info.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/time_area_mgr.h"

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
      mGndNormal(0.0f, 1.0f, 0.0f),
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
    bool notOk = false;
    if (dAcBase_c::preExecute() == NOT_READY) {
        notOk = true;
    }

    mLinkage.fn_80051190(this);

    unsetObjectProperty(OBJ_PROP_0x4000 | OBJ_PROP_0x400 | OBJ_PROP_0x80 | OBJ_PROP_0x20);

    if (checkObjectProperty(OBJ_PROP_0x40)) {
        setObjectProperty(OBJ_PROP_0x80);
    } else if (checkObjectProperty(OBJ_PROP_0x1)) {
        if (checkActorProperty(AC_PROP_0x1) && checkActorProperty(AC_PROP_0x10000000)) {
            if (!(EventManager::checkFlag(0x10) || EventManager::FUN_800a0570(this) ||
                  (checkActorProperty(AC_PROP_0x1000) && dTimeAreaMgr_c::fn_800BBE00()))) {
                if (!dTimeAreaMgr_c::fn_800BBD40() || !dTgViewClip_c::checkForceOn(mViewClipIdx)) {
                    return false;
                }
            }
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
        cull = FLT_MAX;
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
        (mLinkage.checkState(dLinkage_c::STATE_ACTIVE) && mLinkage.getControllingActor()->isPlayer() &&
         (mLinkage.getControllingActor() && mLinkage.getControllingActor()->checkObjectProperty(OBJ_PROP_0x200)))) {
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
    setTempCreateParams(pPos, pRot, pScale, roomID, params2, nullptr, 0, 0xFFFF, 0xFF, nullptr);
    return static_cast<dAcObjBase_c *>(createBase(profile, dStage_c::getParentForRoom(roomID), params, ACTOR));
}

dAcObjBase_c *dAcObjBase_c::create(
    const char *name, u32 roomID, u32 params, const mVec3_c *pPos, const mAng3_c *pRot, const mVec3_c *pScale,
    u32 params2, u16 id, s8 viewclipId
) {
    const ActorInfo *pInfo = getActorInfoByName(name);
    setTempCreateParams(pPos, pRot, pScale, roomID, params2, nullptr, pInfo->subtype, id, viewclipId, pInfo);
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
    mVec3_c v = pos - mPosition;

    mVelocity.x = v.x / speed;
    mVelocity.z = v.z / speed;
    mVelocity.y = mGravity * speed * 0.5f;
    mVelocity.y += v.y / speed;
}

void dAcObjBase_c::calcVelocity() {
    f32 f = nw4r::ut::Max(mVelocity.y + mGravity, mMaxFallSpeed);
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

void dAcObjBase_c::setSinkDepths(f32 f0, f32 f1, f32 f2, f32 f3, f32 f4, dBgS_Acch *acch) {
    acch->SetGroundSink();
    mDeepSinkTarget = f0;
    mShallowSinkTarget = f1;
    mDeepSinkRate = f2;
    mShallowSinkRate = f3;
    mDefaultSinkRate = f4;
}

void dAcObjBase_c::calcSinkDepth(const cBgS_PolyInfo &gnd) {
    f32 target = 0.0f;
    f32 step = mDefaultSinkRate;

    if (!mLinkage.checkState(dLinkage_c::STATE_ACTIVE)) {
        int attr = dBgS::GetInstance()->GetSpecialCode(gnd);

        if (attr == POLY_ATTR_SAND_DEEP_INSTANT || attr == POLY_ATTR_LAVA || attr == POLY_ATTR_SAND_DEEP_SLOW ||
            attr == POLY_ATTR_SAND_MED || attr == POLY_ATTR_CURSED_WATER) {
            target = mDeepSinkTarget;
            step = mDeepSinkRate;
        } else if (attr == POLY_ATTR_SAND_SHALLOW || attr == POLY_ATTR_SAND_SHALLOW2) {
            target = mShallowSinkTarget;
            step = mShallowSinkRate;
        }
    }
    sLib::chase(&mSinkDepth, target, step);
}

void dAcObjBase_c::setGndNormal(const cBgS_PolyInfo &gnd) {
    cM3dGPla plane;
    dBgS::GetInstance()->GetTriPla(gnd, &plane);
    mGndNormal = plane.GetN();
}

bool dAcObjBase_c::putInODesert(f32 depth, mVec3_c *pos) {}

void dAcObjBase_c::drawModel(m3d::smdl_c *pMdl) {
    d3d::setLight(pMdl, mLightingInfo.mLightSetIdx >= 0 ? mLightingInfo.mLightSetIdx : -1, &mLightingInfo);
    pMdl->entry();
}

void dAcObjBase_c::drawModel(m3d::smdl_c *pMdl, int lightSetIdx) {
    d3d::setLight(pMdl, lightSetIdx, &mLightingInfo);
    pMdl->entry();
}

bool dAcObjBase_c::drawShadowAlone(m3d::smdl_c &mdl) {
    bool check;
    if (dStageMgr_c::GetInstance()->checkFlag0x88A0(0x2)) {
        check = false;
    } else {
        if (mLinkage.fn_800513C0()) {
            if (mLinkage.getControllingActor()->isActorPlayer()) {
                static_cast<dAcPy_c *>(mLinkage.getControllingActor())->updateShadow(mdl);
            }
            check = false;
        } else {
            check = true;
        }
    }
    return check;
}

bool dAcObjBase_c::drawShadow(
    dShadowCircle_c &shadow, m3d::smdl_c &mdl, const mSphere_c &sph, s32 priority, s32 alpha, f32 height
) {
    return drawShadowAlone(mdl) ? shadow.drawMdl(mdl, sph, mLightingInfo, priority, alpha, this, height) : false;
}

bool dAcObjBase_c::drawShadow(
    dShadowCircle_c &shadow, const GXTexObj *pTex, const mMtx_c &mtx, const mSphere_c &sph, s32 priority, s32 alpha,
    s32 red, s32 green, s32 blue, f32 height
) {
    bool check;
    if (dStageMgr_c::GetInstance()->checkFlag0x88A0(0x2)) {
        check = false;
    } else {
        if (mLinkage.fn_800513C0()) {
            if (mLinkage.getControllingActor()->isActorPlayer()) {
                static_cast<dAcPy_c *>(mLinkage.getControllingActor())->updateShadow(sph, mtx);
            }
            check = false;
        } else {
            check = true;
        }
    }

    return check ? shadow.drawTexObj(pTex, mtx, sph, mLightingInfo, priority, alpha, red, green, blue, this, height) :
                   false;
}

dAcObjBase_c *dAcObjBase_c::createChildAttached(
    ProfileName actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2, u32 roomId
) {
    return static_cast<dAcObjBase_c *>(createActor(actorId, params1, pos, rot, scale, params2, roomId, this));
}
dAcObjBase_c *dAcObjBase_c::createChildDetached(
    ProfileName actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2, u32 roomId
) {
    return static_cast<dAcObjBase_c *>(createActor(actorId, params1, pos, rot, scale, params2, roomId, nullptr));
}

void dAcObjBase_c::fn_8002efa0() {
    if (isRoomFlags_0x6_Set()) {
        setObjectProperty(OBJ_PROP_0x200);
    } else {
        unsetObjectProperty(OBJ_PROP_0x200);
    }
}
bool dAcObjBase_c::fn_8002eff0(
    const f32 &radius, const s16 &rollRotRange, cCcD_Obj *pCc, const f32 &heightMax, const f32 &heightMin
) {
    if (pCc == nullptr || pCc->ChkCoHit()) {
        dAcPy_c *pPlayer = dAcPy_c::GetLinkM();

        f32 rad = (radius + 40.0f);
        f32 heightDiffToPlayer = dAcPy_c::GetLinkM()->mPosition.y - mPosition.y;

        bool b = heightDiffToPlayer > heightMin;
        if (b) {
            b = heightDiffToPlayer < heightMax;
        }
        if (b) {
            b = pPlayer->checkCurrentAction(12 /* ROLL */);
        }
        if (b) {
            mVec3_c v = pPlayer->mPosition - mPosition;
            b = v.squareMagXZ() < rad * rad;
        }
        if (b) {
            // TODO: Find a better solution for ang bullsh*t
            s32 a = getPostionDifference(*pPlayer).atan2sX_Z();
            mAng b = (mAng)a - pPlayer->mAngle.y;
            if (mAng(b).abs() < rollRotRange) {
                pPlayer->setBonkRelatedAnimFlag(false);
                return true;
            }
        }
    }
    return false;
}

// These may not belong here
dAcObjBase_c *dAcObjBase_c::createInsectActor(
    dAcBase_c *parent, ProfileName actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2,
    s32 roomId
) {
    return static_cast<dAcObjBase_c *>(parent->createActor(actorId, params1, pos, rot, scale, params2, roomId, parent));
}

dAcObjBase_c *dAcObjBase_c::createStageActor(
    ProfileName profile, u32 roomID, u32 params, mVec3_c *pPos, mAng3_c *pRot, mVec3_c *pScale, u32 params2
) {
    setTempCreateParams(pPos, pRot, pScale, roomID, params2, nullptr, 0, 0xFFFF, 0xFF, nullptr);
    return static_cast<dAcObjBase_c *>(createBase(profile, dStage_c::getParentForRoom(roomID), params, STAGE));
}

dAcObjBase_c *dAcObjBase_c::createStageActor(
    const char *name, u32 roomID, u32 params, const mVec3_c *pPos, const mAng3_c *pRot, const mVec3_c *pScale,
    u32 params2, u16 id, s8 viewclipId
) {
    const ActorInfo *pInfo = getActorInfoByName(name);
    setTempCreateParams(pPos, pRot, pScale, roomID, params2, nullptr, pInfo->subtype, id, viewclipId, pInfo);
    return static_cast<dAcObjBase_c *>(createBase(pInfo->profileId, dStage_c::getParentForRoom(roomID), params, STAGE));
}
