#include "d/a/obj/d_a_obj_bomb.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/e/d_a_en_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_pc.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/c/c_m3d.h"
#include "d/col/c/c_m3d_g_pla.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_light_env.h"
#include "d/d_linkage.h"
#include "d/d_rumble.h"
#include "d/d_sc_game.h"
#include "d/d_stage_mgr.h"
#include "d/d_vec.h"
#include "d/snd/d_snd_wzsound.h"
#include "d/t/d_t_stream.h"
#include "f/f_base.h"
#include "f/f_profile.h"
#include "f/f_profile_name.h"
#include "m/m3d/m3d.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtx.h"
#include "s/s_Math.h"
#include "s/s_State.hpp"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/event_manager.h"

#include <cstdlib>

// For real??
template <typename T>
inline T MyClamp(T value, T min, T max) {
    return (value < min ? min : value > max ? max : value);
}

SPECIAL_ACTOR_PROFILE(BOMB, dAcBomb_c, fProfile::BOMB, 0x128, 0, 2);

static const Vec vec_fn_80259E80 = {0, 60, 0};

dCcD_SrcSph sSphSrc = {
    /* mObjInf */
    {/* mObjAt */ {AT_TYPE_BOMB, 0x1803E, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {~(AT_TYPE_BUGNET | AT_TYPE_GLITTERING_SPORES | AT_TYPE_0x8000 | AT_TYPE_0x10), 0x800111, {0, 0x0A, 0x40F}, 0, 0},
     /* mObjCo */ {0xE9}},
    /* mSphInf */
    {0.0f}
};

struct dAcBomc_HIO_c {
    static const f32 getBBoxX() {
        return 36.f;
    }
    static const f32 getBBoxY() {
        return 66.f;
    }
    static const f32 getBBoxZ() {
        return 6.f;
    }
    static const f32 getBombScale() {
        return 30.f;
    }
    static const mVec3_c getBBoxMin() {
        return mVec3_c(-bbox_x, 0.f, -bbox_x);
    }
    static const mVec3_c getBBoxMax() {
        return mVec3_c(bbox_x, bbox_y, bbox_x);
    }
    static const f32 bbox_x, bbox_y, bbox_z;
    static const f32 bbox_min_y;
};
static dAcBomc_HIO_c sHIO = {};

const f32 dAcBomc_HIO_c::bbox_x = sHIO.getBBoxX();
const f32 dAcBomc_HIO_c::bbox_y = sHIO.getBBoxY();
const f32 dAcBomc_HIO_c::bbox_z = sHIO.getBBoxZ();

STATE_DEFINE(dAcBomb_c, Wait);
STATE_DEFINE(dAcBomb_c, FlowerWait);
STATE_DEFINE(dAcBomb_c, Explode);
STATE_DEFINE(dAcBomb_c, Carry);
STATE_DEFINE(dAcBomb_c, WindCarry);

void dAcBomb_c::coHitCallback(cCcD_Obj *i_objInfB) {
    if (!i_objInfB->ChkCo_0x8000()) {
        return;
    }

    set0xA3C(FLAG_0x4);
    setActorProperty(AC_PROP_0x4);
}

bool dAcBomb_coHitCallback(dAcObjBase_c *i_actorA, cCcD_Obj *i_objInfA, dAcObjBase_c *i_actorB, cCcD_Obj *i_objInfB) {
    static_cast<dAcBomb_c *>(i_actorA)->coHitCallback(i_objInfB);
    return true;
}

void dAcBomb_c::tgHitCallback(dAcObjBase_c *i_actorB, cCcD_Obj *i_objInfA, cCcD_Obj *i_objInfB) {
    // Check if it is being blown
    if ((i_objInfB->ChkAtType(AT_TYPE_WIND | AT_TYPE_BELLOWS) && i_objInfB->GetAtSrcModifier() == AT_MOD_WIND) &&
        !(i_actorB &&
          (i_actorB->mProfileName == fProfile::OBJ_WIND || i_actorB->mProfileName == fProfile::OBJ_TORNADO))) {
        if (!getLinkage().checkState(dLinkage_c::STATE_ACTIVE) && !check0xA3C(FLAG_0x4)) {
            if (!(isState(StateID_FlowerWait) || isState(StateID_Explode))) {
                cLib::addCalcPosXZ(&field_0xA84, i_objInfA->GetTgAtHitDir() * 0.5f, 0.5f, 10.f, 1.f);
                set0xA3C(FLAG_0x8000);
            }
        }
    } else if (!i_objInfB->ChkAtType(AT_TYPE_0x40 | AT_TYPE_WIND | AT_TYPE_WHIP | AT_TYPE_BEETLE) ||
               i_objInfB->ChkAt_0x4000()) {
        if (i_objInfB->ChkAtType(AT_TYPE_0x20000)) {
            if (isState(StateID_FlowerWait)) {
                changeState(StateID_Wait);
                dAcPy_c::insertOwnedObject(this);
            }

            const dAcPy_c *pPlayer = dAcPy_c::GetLink2();

            mVec3_c target;
            setXYZCirclePoint(target, pPlayer->mRotation.y, 30.f, 0.f);
            setTarget(target);
        } else if (i_objInfB->ChkAtType(
                       AT_TYPE_SWORD | AT_TYPE_0x40 | AT_TYPE_SLINGSHOT | AT_TYPE_0x20000 | AT_TYPE_BEETLE
                   ) &&
                   check0xA3C(FLAG_0x1000000 | FLAG_0x400000)) {
            unset0xA3C(FLAG_0x1000000);
            set0xA3C(FLAG_0x400000);
            unsetActorProperty(AC_PROP_0x1);
            if (isState(StateID_FlowerWait)) {
                dAcPy_c::insertOwnedObject(this);
            }
            if (!check0xA3C(FLAG_0x100) && i_objInfB->ChkAtType(AT_TYPE_SWORD)) {
                set0xA3C(FLAG_0x100);
            }
        } else {
            set0xA3C(FLAG_0x4);
            setActorProperty(AC_PROP_0x4);
            if (i_objInfB->ChkAtType(AT_TYPE_BOMB)) {
                set0xA3C(FLAG_0x2);
            }
        }
    }
}

bool dAcBomb_tgHitCallback(dAcObjBase_c *i_actorA, cCcD_Obj *i_objInfA, dAcObjBase_c *i_actorB, cCcD_Obj *i_objInfB) {
    static_cast<dAcBomb_c *>(i_actorA)->tgHitCallback(i_actorB, i_objInfA, i_objInfB);
    return true;
}

bool dAcBomb_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("Alink"));

    nw4r::g3d::ResMdl mdl = mRes.GetResMdl("EquipBomb");
    bool result = mMdl.create(mdl, &mAllocator, 0x120);
    if (!result) {
        return false;
    }

    mMainMatID = m3d::getMatID(mdl, "m_Main");

    if (getBombType() == BOMB_2) {
        mMdl.setPriorityDraw(0x82, 0x7F);
    }
    return result;
}

int dAcBomb_c::create() {
    CREATE_ALLOCATOR(dAcBomb_c);

    mAcch.Set(this, 1, &mAcchCir);
    mAcch.SetRoofCrrHeight(60.f);
    mAcch.SetGndChkOffset(60.f);
    mAcch.GetRoof().SetField_0x7C(1);
    mAcch.SetWaterCheckOffset(10000.f);
    mAcch.SetWtrChkMode(2);
    mAcch.ClrWaterNone();
    mAcch.ClrRoofNone();
    mAcch.OnLineCheck();

    mAcchCir.SetWall(30.f, 30.f);

    if (getBombType() == BOMB_2) {
        mStts.SetRank(4);
    } else {
        mStts.SetRank(5);
    }

    mSph.Set(sSphSrc);
    mSph.SetStts(mStts);
    mSph.SetTgCallback(dAcBomb_tgHitCallback);
    mSph.setCenter(mPosition);

    MTXIdentity(mMtx);

    mBoundingBox.Set(sHIO.getBBoxMin(), sHIO.getBBoxMax());

    mAcceleration = dAcPy_c::getBombAcceleration();
    mMaxSpeed = dAcPy_c::getBombMaxSpeed();

    mFuseTime = dAcPy_c::getBombExplodeTimer();

    mLinkage.set(0xc9c2, 25.f, 43.f, 36.f, nullptr);
    mLinkage.setField_0x8C_0x90(dAcPy_c::getBombUnknown0_0(), dAcPy_c::getBombUnknown0_1());
    mLinkage.setField_0x94_0x98(dAcPy_c::getBombUnknown1_0(), dAcPy_c::getBombUnknown1_1());
    field_0xA50 = 1.f;
    mVec3_c pos = (mPosition);
    mAcch.CrrPos(*dBgS::GetInstance());
    setRoomId();

    switch (getBombType()) {
        case BOMB_0:
        case BOMB_4:
        case BOMB_6: {
            changeState(StateID_Explode);
        } break;
        case BOMB_3: {
            set0xA3C(FLAG_0x200000);
            set0xA3C(FLAG_0x1);
            if (checkWaterIn()) {
                set0xA3C(FLAG_0x20);
                mAcceleration = dAcPy_c::getBombAcceleration_1();
                mMaxSpeed = dAcPy_c::getBombMaxSpeed_1();
            }
            changeState(StateID_Carry);
        } break;
        case BOMB_7: {
            set0xA3C(FLAG_0x8000000);
            changeState(StateID_Carry);
        } break;
        case BOMB_1:
        case BOMB_2:
        case BOMB_5: {
            mPosition = pos;
            changeState(StateID_FlowerWait);
        } break;
        default: {
            changeState(StateID_Wait);
        } break;
    }

    f32 yOffset = mScale.y * 25.f;
    mWorldMtx.transS(mPosition);
    mWorldMtx.ZXYrotM(mRotation);
    mMtx_c m0, m1;

    m0.transS(0, yOffset, 0);
    mWorldMtx.concat(m0);

    mWorldMtx.concat(mMtx);

    m1.transS(0, -yOffset, 0);
    mWorldMtx.concat(m1);

    mMdl.setLocalMtx(mWorldMtx);

    setBombColor();
    mMdl.calc(false);
    setSmokePos();
    field_0xA78 = mPosition;

    return SUCCEEDED;
}

void dAcBomb_c::unkVirtFunc_0x5C() {
    if (isState(StateID_Wait)) {
        deleteRequest();
        mEffect1.remove(true);
        mEffect2.remove(true);
    }
}

int dAcBomb_c::doDelete() {
    mLinkage.forceRemove(this);
    return SUCCEEDED;
}

// NONMATCHING
bool dAcBomb_c::checkExplode() {
    if (field_0xA46 != 0) {
        field_0xA46--;
    }

    if (mFuseTime > 0) {
        bool b = !check0xA3C(FLAG_0x1000000 | FLAG_0x200000 | FLAG_0x800 | FLAG_0x200) && !isState(StateID_Explode);
        if (b) {
            mFuseTime--;
            if (check0xA3C(FLAG_0x8000000 | FLAG_0x800000)) {
                s16 explodeTimer = dAcPy_c::getBombExplodeTimer();
                if (mFuseTime == (explodeTimer >> 1) || mFuseTime == (explodeTimer >> 2) ||
                    mFuseTime == (explodeTimer >> 3) || mFuseTime == (explodeTimer >> 4)) {
                    mFuseTime <<= 1;
                }
            }
        }
    }

    if (check0xA3C(FLAG_0x800000 | FLAG_0x800 | FLAG_0x200)) {
        return false;
    }

    if (mFuseTime == 0 || check0xA3C(FLAG_0x4) ||
        (mSph.ChkCoHit() && mSph.GetCoActor()->isActorPlayer() &&
         static_cast<dAcPy_c *>(mSph.GetCoActor())->isBurning()) ||
        (check0xA3C(FLAG_0x2000000) && field_0xA46 == 0) &&
            ( // Check Beetle or Chu grab
                (mSph.ChkCoHit() && mSph.GetCoActor()->mProfileName != fProfile::BOOMERANG &&
                 mSph.GetCoActor()->mProfileName != fProfile::E_SM) ||
                !mAcch.fn_80340ca0(mUnkInfo)
            )) {
        changeState(StateID_Explode);
        return true;
    }

    if (mAcch.ChkGndHit()) {
        const dBgS_GndChk &gnd = mAcch.GetGnd();
        if (dBgS::GetInstance()->GetSpecialCode(gnd) == POLY_ATTR_LAVA ||
            dBgS::GetInstance()->GetGroundCode(gnd) == 4 /* FIXME*/) {
            mUnkInfo.SetPolyInfo(mAcch.GetGnd());
            changeState(StateID_Explode);
            return true;
        }
    }

    if (mAcch.ChkWallHit(nullptr)) {
        if (dBgS::GetInstance()->GetSpecialCode(mAcchCir) == POLY_ATTR_LAVA) {
            mUnkInfo.SetPolyInfo(mAcchCir);
            changeState(StateID_Explode);
            return true;
        }
    }

    return false;
}

void dAcBomb_c::setRoomId() {
    if (mAcch.GetGroundH() != -1e9f) {
        mRoomID = dBgS::GetInstance()->GetRoomId(mAcch.GetGnd());
    } else if (mRoomID == -1) {
        mRoomID = dAcPy_c::GetLink2()->getRoomId();
    }
}

void dAcBomb_c::setSmokePos() {
    mVec3_c in = vec_fn_80259E80;
    mWorldMtx.multVec(in, field_0xA54);
    field_0xA6C = (field_0xA54 - field_0xA60) * 0.5f;
}

bool dAcBomb_c::checkWaterIn() {
    return check0xA3C(FLAG_0x20000000) ||
           (mAcch.ChkWaterHit() && (mPosition.y + mScale.x * sHIO.getBombScale() < mAcch.GetWtrGroundH()));
}

void dAcBomb_c::setTransformFromFlower(const mMtx_c &m) {
    if (mLinkage.checkState(dLinkage_c::STATE_ACTIVE)) {
        return;
    }
    mMtx = m;

    mMtx_c &m0 = mMtx;
    mVec3_c out;
    m0.setTranslation(mVec3_c::Zero);
    m0.multVecSR(mVec3_c::Ey, out);

    f32 f = field_0xA50 * (25.f * mScale.y);
    m.getTranslation(field_0xA78);

    mPosition = field_0xA78 + out * f;
    mPosition.y -= f;
}

void dAcBomb_c::setPickupTarget() {
    if (check0xA3C(FLAG_0x800)) {
        return;
    }

    AttentionManager::GetInstance()->addPickUpTarget(*this, 120.f);
}

void dAcBomb_c::lookAt() {
    dAcPy_c::GetLink2()->lookTowardItem(mPositionCopy2, 4, 0x5555, 300, 400, -400);
}

void dAcBomb_c::bounceWall(mAng angle) {
    mSpeed *= dAcPy_c::getBombBounceSpeed();
    mBombSpeed *= dAcPy_c::getBombBounceSpeed();

    mAngle.y = angle - mAng((mAngle.y + 0x8000) - angle);
    mBombRotationY = angle - mAng((mBombRotationY + 0x8000) - angle);
}

void dAcBomb_c::fn_8025A1F0(mVec3_c &out) {
    setXYZCirclePoint(out, mAngle.y, mSpeed, mVelocity.y);

    if (field_0xA40 != 0) {
        f32 f = MyClamp(field_0xA40 * 0.02f, 0.f, 1.f);

        mVec3_c v;
        setXYZCirclePoint(v, mBombRotationY, mBombSpeed, 0.f);

        out = out * (1.f - f) + v * f;
    }
}

void dAcBomb_c::initializeState_Explode() {
    setActorProperty(AC_PROP_0x4);

    dAcPy_c *pPlayer = dAcPy_c::GetLink2();

    dAcObjBase_c *pObj = pPlayer->getBeetleHeldActor();
    if (pObj == this) {
        mLinkage.forceRemove(this);
        pPlayer->setBeetleReleasedObject(this);
    }

    mRotation.y = cM::rndF(65536);
    field_0xA6C = mVec3_c::Zero;
    mVec3_c out;
    if (getOldStateID() == StateID_FlowerWait || dBgS::GetInstance()->ChkPolySafe(mUnkInfo)) {
        if (getOldStateID() == StateID_FlowerWait) {
            mWorldMtx.multVecSR(mVec3_c::Ey, out);
        } else {
            cM3dGPla plane;
            dBgS::GetInstance()->GetTriPla(mUnkInfo, &plane);
            out = plane.GetN();
        }
        (void)out.atan2sX_Z();
        (void)cM::atan2s(out.absXZ(), out.y);

        mPosition.y += 25.f;

        mPosition -= out * 25.f;

    } else if (mAcch.ChkGndHit()) {
        cM3dGPla plane;
        dBgS::GetInstance()->GetTriPla(mAcch.GetGnd(), &plane);

        (void)plane.GetN().atan2sX_Z();
        (void)cM::atan2s(plane.GetN().absXZ(), plane.GetN().y);
    }

    dCamera_c *pCam = dScGame_c::getCamera();
    mVec3_c v = pCam->getTarget() - pCam->getPosition();
    if (v.getSquareMag() > 0.01f) {
        v.normalize();
    } else {
        v = mVec3_c::Ez;
    }

    cM3dGPla plane;
    plane.SetupNP(v, pCam->getPosition());

    f32 d = cM3d_SignedLenPlaAndPos(&plane, mPosition);

    mVec3_c temp(dAcPy_c::getBombExplodeScale(), dAcPy_c::getBombExplodeScale(), dAcPy_c::getBombExplodeScale());
    if (d < dAcPy_c::getBombDistanceUnknown0()) {
        dEmitterBase_c *pEmitter = dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_322_, mPosition, nullptr, &temp, nullptr, nullptr, 0, 0
        );
        if (pEmitter != nullptr) {
            pEmitter->setRate(10.f);
        }
        (void)dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_323_, mPosition, nullptr, &temp, nullptr, nullptr, 0, 0
        );
        pEmitter = dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_324_, mPosition, nullptr, &temp, nullptr, nullptr, 0, 0
        );
        if (pEmitter != nullptr) {
            pEmitter->setRate(5.f);
        }
    } else if (check0xA3C(FLAG_0x2)) {
        (void)dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_321_, mPosition, nullptr, &temp, nullptr, nullptr, 0, 0
        );
    } else {
        (void)dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_322_, mPosition, nullptr, &temp, nullptr, nullptr, 0, 0
        );
        (void)dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_323_, mPosition, nullptr, &temp, nullptr, nullptr, 0, 0
        );
        (void)dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_324_, mPosition, nullptr, &temp, nullptr, nullptr, 0, 0
        );
        (void)dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_325_, mPosition, nullptr, &temp, nullptr, nullptr, 0, 0
        );
    }

    if (dStageMgr_c::GetInstance()->isInLastBoss() && mAcch.ChkGndHit()) {
        (void)dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_849_, mPosition, nullptr, nullptr, nullptr, nullptr, 0, 0
        );
    }
    dLightEnv_c::GetPInstance()->SordFlush_set(&mPosition, 5);

    mSpeed = 0.f;
    mVelocity = mVec3_c::Zero;
    mAcceleration = 0.f;

    mSph.ClrTgSet();
    mSph.ClrCoSet();
    if (getBombType() == BOMB_6) {
        mSph.ClrAtSet();
    } else {
        mSph.OnAtSet();
    }

    if (getBombType() == BOMB_2) {
        mSph.SetR(200.f);
    } else {
        mSph.SetR(dAcPy_c::getBombExplodeScale());
    }

    mSph.SetC(mPosition);
    mSph.SetAtType(AT_TYPE_BOMB);
    mSph.SetAtVec(mVec3_c::Zero);
    dCcS::GetInstance()->Set(&mSph);
    dCcS::GetInstance()->SetMass(&mSph, 1);

    startSound(SE_BM_EXPLODE);
    mLinkage.forceRemove(this);
    mFuseTime = 0;
    unsetObjectProperty(OBJ_PROP_0x1000);

    f32 dist = mPosition.squareDistance(pPlayer->mPosition);
    if (dist < 160000.f) {
        dRumble_c::start(dRumble_c::sRumblePreset3, dRumble_c::FLAG_SLOT0 | dRumble_c::FLAG_ACTIVE);
    } else if (dist < 640000.f) {
        dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0 | dRumble_c::FLAG_ACTIVE);
    }

    dLightEnv_c::GetPInstance()->setBPM8_Type10_2(&mPosition);
    dAcEnBase_c::fn_800307E0(&mPosition, 0);
}
void dAcBomb_c::executeState_Explode() {
    if (--mFuseTime >= -3) {
        dCcS::GetInstance()->Set(&mSph);
        dCcS::GetInstance()->SetMass(&mSph, 1);
    } else {
        deleteRequest();
    }
}
void dAcBomb_c::finalizeState_Explode() {}

void dAcBomb_c::initializeState_Carry() {
    unset0xA3C(FLAG_0x2000000 | FLAG_0x80000 | FLAG_0x40000 | FLAG_0x10000);
    field_0xA84 = mVec3_c::Zero;
    if (mLinkage.checkConnection(dLinkage_c::CONNECTION_5)) {
        set0xA3C(FLAG_0x2000000);
        field_0xA46 = 5;
        mLinkage.offFlag(0x20000000);
    } else {
        dAcBase_c *pObj = mLinkage.getControllingActor();
        if (pObj != nullptr && pObj->isActorPlayer()) {
            dAcPy_c::GetLink2()->getOwnedObjects().insert(this);
        }
        mSph.ClrCoSet();
    }
    dAcBase_c *pObj = mLinkage.getControllingActor();
    if (pObj != nullptr && pObj->mProfileName == fProfile::E_SM) {
        onSoundSourceFlag(1);
    }

    mSpeed = 0.f;
    mVelocity = mVec3_c::Zero;

    mLinkage.bushTpFunc(mAcch);
    setRoomId();
}
void dAcBomb_c::executeState_Carry() {
    dAcPy_c *pPlayer = dAcPy_c::GetLink2();

    if (checkExplode()) {
        unset0xA3C(FLAG_0x8000000 | FLAG_0x1000000 | FLAG_0x200000);
    } else {
        if (!mLinkage.checkState(dLinkage_c::STATE_ACTIVE)) {
            if (check0xA3C(FLAG_0x100000)) {
                unset0xA3C(FLAG_0x100000);
                mMtx.ZrotS(-mRotation.z);
                mMtx.XrotM(-mRotation.x);
                mMtx.YrotM(-mRotation.y);
                mMtx.concat(mWorldMtx);
                mMtx.m[0][3] = 0.f;
                mMtx.m[1][3] = 0.f;
                mMtx.m[2][3] = 0.f;
            }
            if (mLinkage.checkState(dLinkage_c::STATE_3)) {
                set0xA3C(FLAG_0x80000 | FLAG_0x40000);
                mBombRotationY = pPlayer->mRotation.y;
                mBombSpeed = mSpeed;
                field_0xA40 = 60;
            }
            unset0xA3C(FLAG_0x8000000 | FLAG_0x1000000 | FLAG_0x200000);
            changeState(StateID_Wait);
        } else if (!mLinkage.checkConnection(dLinkage_c::CONNECTION_5)) {
            unset0xA3C(FLAG_0x2000000 | FLAG_0x1000000);
        }
        mLinkage.bushTpFunc(mAcch);
        setRoomId();
        if (isState(StateID_Carry)) {
            mSpeed = 0.f;
            mVelocity = mVec3_c::Zero;
        }
    }
}
void dAcBomb_c::finalizeState_Carry() {
    if (getNewStateID() != StateID_Explode) {
        offSoundSourceFlag(1);
    }
}

void dAcBomb_c::initializeState_Wait() {
    mSph.OnCoSet();
    mSph.SetCoCallback(dAcBomb_coHitCallback);
    field_0xA42 = 5;
    if (check0xA3C(FLAG_0x4000000)) {
        unset0xA3C(FLAG_0x4000000);
        set0xA3C(FLAG_0x2000000);
    }
}
void dAcBomb_c::executeState_Wait() {
    if (checkExplode()) {
        return;
    }

    if (mLinkage.checkState(dLinkage_c::STATE_ACTIVE)) {
        changeState(StateID_Carry);
        return;
    }

    if (mUnkRef.fn_80051780(mSph)) {
        changeState(StateID_WindCarry);
        return;
    }

    if (field_0xA42 != 0) {
        field_0xA42--;
    }

    mVec3_c out;
    fn_8025A1F0(out);

    s16 s = out.atan2sX_Z();
    f32 d = out.absXZ();

    s16 out_s;

    if (mAcch.ChkGndHit()) {
        mAng a = mAcch.GetGnd().GetAngle(s);
        mVelocity.y -= d * std::abs(a.sin());

        f32 cos = a.cos();
        out.x *= cos;
        out.z *= cos;
        if (field_0xA40 != 0) {
            field_0xA40--;
        }
        out_s = a;

    } else {
        out_s = 0;
    }

    f32 acc = mAcceleration;
    f32 vely = mVelocity.y;
    out.y += mVelocity.y + mAcceleration;
    if (out.y < mMaxSpeed) {
        out.y = mMaxSpeed;
    }
    mVelocity.y = out.y;

    mPosition += out + mStts.GetCcMove();

    if (check0xA3C(FLAG_0x10000) && check0xA3C(FLAG_0x20) && mAcch.ChkWaterHit()) {
        if (mAcch.GetWtrGroundH() <= mPosition.y + (mScale.x * 30.f) + 1.f) {
            mVelocity.y = 0.f;
            mPosition.y = mAcch.GetWtrGroundH() - (mScale.x * 30.f) - 1.f;
        }
    }
    mAcceleration = acc;
    f32 f = 1.f;
    if (mAcch.ChkGndHit() && field_0xA84.squareMagXY() > 1.f) {
        cM3dGPla plane;
        dBgS::GetInstance()->GetTriPla(mAcch.GetGnd(), &plane);

        f = plane.GetAngle(field_0xA84.atan2sX_Z()).cos();
        f *= f;
    }

    mPosition += (field_0xA84 * f) + field_0xA90;
    mAcch.CrrPos(*dBgS::GetInstance());
    f32 f1 = 0.f;
    f32 f2 = 4.f;
    mVec3_c force = mVec3_c::Zero;
    f32 offs = mYOffset;
    if (mAcch.ChkGndHit() && !mLinkage.checkState(dLinkage_c::STATE_ACTIVE)) {
        s32 attr = dBgS::GetInstance()->GetSpecialCode(mAcch.GetGnd());
        if (attr == POLY_ATTR_CURSED_WATER) {
            f1 = -60.f;
            if (mYOffset >= 0.f) {
                vely *= 0.5f;
                mSpeed *= 0.5f;
            }
            f2 = nw4r::math::FAbs(vely);
            mVelocity.y = vely;
            mAcceleration = dAcPy_c::getBombAcceleration_1();
            mMaxSpeed = dAcPy_c::getBombMaxSpeed_1();
        } else if (attr == POLY_ATTR_SAND_DEEP_INSTANT || attr == POLY_ATTR_SAND_DEEP_SLOW ||
                   attr == POLY_ATTR_SAND_MED) {
            f1 = -10.f;
            f2 = 1.f;
        }
        if (attr == POLY_ATTR_SAND_DEEP_SLOW || attr == POLY_ATTR_SAND_MED) {
            dTgStream_c::getForce(mPosition, force);
        }

        cLib::addCalcPosXZ(&field_0xA90, force, 0.5f, 10.f, 3.f);
        sLib::chase(&offs, f1, f2);
        mYOffset = offs;
        if (offs <= -60.f) {
            set0xA3C(FLAG_0x1000);
        }
        setRoomId();

        if (mAcch.ChkGndHit()) {
            cM3dGPla plane;
            dBgS::GetInstance()->GetTriPla(mAcch.GetGnd(), &plane);
        }
        // ...

        if (mAcch.ChkGroundLanding()) {
            if (vely < -5.f) {
                startSoundWithFloatParam(SE_BM_BOUND, -1.f);
                if (dBgS::GetInstance()->GetPolyAtt0(mAcch.GetGnd()) != POLY_ATT_0_LAVA) {
                    dJEffManager_c::spawnGroundEffect(
                        mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 1, 0.8f, field_0x1B0
                    );
                }
            }

            // ...

            f32 f3 = dAcPy_c::getBombUnkFloat_0();
            s32 attr = dBgS::GetInstance()->GetSpecialCode(mAcch.GetGnd());

            if (check0xA3C(FLAG_0x40000) ||
                ((dAcPy_c::getBombUnkFloat_1() <= f3 && attr != POLY_ATTR_SAND_DEEP_INSTANT) &&
                 attr != POLY_ATTR_CURSED_WATER && attr != POLY_ATTR_SAND_DEEP_SLOW && attr != POLY_ATTR_SAND_MED)) {
                mSpeed *= 0.9f;
                mBombSpeed *= 0.9f;
            }
        }
    }

    lookAt();
}
void dAcBomb_c::finalizeState_Wait() {
    mSph.SetCoCallback(dAcBomb_coHitCallback);
    mSph.OnCoSet();
}

void dAcBomb_c::initializeState_FlowerWait() {
    set0xA3C(FLAG_0x1000000);
    mSph.ClrTgSet();
    mScale.set(0.1f, 0.1f, 0.1f);
    mStts.SetRank(0xD);
    mAcch.OffLineCheck();
    mMdl.setScale(mScale);
}
void dAcBomb_c::executeState_FlowerWait() {
    dAcPy_c *pPlayer = dAcPy_c::GetLink2();

    AttentionManager::GetInstance()->addUnk3Target(*this, 1, 250.f, -200.f, 200.f);
    lookAt();
    mSph.OnTgSet();
    mVec3_c scale = mScale * field_0xA50;
    mMdl.setScale(scale);

    if (checkExplode()) {
        return;
    }

    if (mLinkage.checkState(dLinkage_c::STATE_ACTIVE)) {
        changeState(StateID_Carry);
        return;
    }

    setPickupTarget();
}
void dAcBomb_c::finalizeState_FlowerWait() {
    if (!(mLinkage.checkConnection(dLinkage_c::CONNECTION_5) && check0xA3C(FLAG_0x1000000))) {
        unset0xA3C(FLAG_0x1000000);
    }

    if (getBombType() == BOMB_2) {
        mStts.SetRank(4);
    } else {
        mStts.SetRank(5);
    }

    unsetActorProperty(AC_PROP_0x1);
    mAcch.OnLineCheck();
}

void dAcBomb_c::initializeState_WindCarry() {
    unset0xA3C(FLAG_0x2000000 | FLAG_0x80000 | FLAG_0x40000 | FLAG_0x10000);
    field_0xA84 = mVec3_c::Zero;
    mSpeed = 0.f;
    mVelocity = mVec3_c::Zero;
    mBombSpeed = 0.f;
    mLinkage.bushTpFunc(mAcch);
    setRoomId();
}
void dAcBomb_c::executeState_WindCarry() {
    if (checkExplode()) {
        return;
    }

    if (mLinkage.checkState(dLinkage_c::STATE_ACTIVE)) {
        changeState(StateID_Carry);
        return;
    }

    if (mSph.ChkCoHit()) {
        mPosition += mStts.GetCcMove();
        mUnkRef.fn_800051630();
    }

    if (!mUnkRef.fn_80051780(mSph)) {
        changeState(StateID_Wait);
        return;
    }

    mLinkage.bushTpFunc(mAcch);
    setRoomId();
    mSpeed = 0.f;
    mVelocity = mVec3_c::Zero;
}
void dAcBomb_c::finalizeState_WindCarry() {}

void dAcBomb_c::setBombColor() {
    mColor clr(0, 0, 0, 0xFF);
    if (!check0xA3C(FLAG_0x800)) {
        bool b = !check0xA3C(FLAG_0x1000000 | FLAG_0x200000 | FLAG_0x800 | FLAG_0x200) && !isState(StateID_Explode);
        if (b) {
            mEffect1.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_974_, mWorldMtx, nullptr, nullptr);
            if (!check0xA3C(FLAG_0x8000000)) {
                mEffect2.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_975_, mWorldMtx, nullptr, nullptr);
            }

            dAcObjBase_c *pObj;
            if ( 
            // Is controlled - and by player
            // And Controlling Actor (player) as objprop 0x200 set
               (mLinkage.checkState(dLinkage_c::STATE_ACTIVE) && static_cast<dAcObjBase_c *>(mLinkage.getControllingActor())->isPlayer()) 
               && ((pObj = static_cast<dAcObjBase_c *>(mLinkage.getControllingActor())) && pObj->checkObjectProperty(OBJ_PROP_0x200)) 
            // OOOOR this bomb has it set.
               || checkObjectProperty(OBJ_PROP_0x200)
            ) {
                mEffect1.stopDrawParticles();
                mEffect2.stopDrawParticles();
            } else {
                holdSound(SE_BM_IGNITION_LV);
                mEffect1.playDrawParticles();
                mEffect2.playDrawParticles();
            }

            s32 interval;
            s16 maxTime = dAcPy_c::getBombExplodeTimer();
            if (mFuseTime > maxTime) {
                interval = maxTime >> 1;
            } else if (mFuseTime > (maxTime >> 1)) {
                interval = maxTime >> 2;
            } else if (mFuseTime > (maxTime >> 2)) {
                interval = maxTime >> 3;
            } else {
                interval = maxTime >> 4;
            }

            s32 tmp1 = (mFuseTime / interval);
            tmp1 = nw4r::ut::Max<s32>(mFuseTime - (tmp1 * interval) - 5, 0);
            u8 colorVal = (1.0f / (interval - 5)) * tmp1 * 255.0f;

            clr.r = colorVal;
            clr.b = colorVal;
        }
    }

    mMdl.setTevKColor(mMainMatID, GX_KCOLOR3, clr, false);
}

void dAcBomb_c::rollTo(const mVec3_c &dir) {
    mVec3_c v;
    setXYZCirclePoint(v, mAngle.y, mSpeed, mVelocity.y);
    v += dir;

    mAngle.y = v.atan2sX_Z();
    mSpeed = v.absXZ();
    mVelocity.y = v.y;
}

void dAcBomb_c::rollToInternal(const mVec3_c &dir) {
    if (field_0xA40 == 0) {
        return;
    }
    mVec3_c v;
    setXYZCirclePoint(v, mBombRotationY, mBombSpeed, 0.f);
    v += dir;

    v.y = 0.f;

    mBombRotationY = v.atan2sX_Z();
    mBombSpeed = v.absXZ();
}

void dAcBomb_c::registerInEvent() {
    if (!isState(StateID_Wait)) {
        return;
    }

    if (!mAcch.ChkGndHit()) {
        return;
    }

    const dAcObjBase_c *pObj = dBgS::GetInstance()->GetActorPointer(mAcch.GetGnd());
    if (pObj != nullptr && pObj->mProfileName != fProfile::OBJ_CANNON) {
        if (pObj->checkActorProperty(AC_PROP_0x4) || EventManager::FUN_800a0570(pObj)) {
            setActorProperty(AC_PROP_0x4);
        }
    }
}

void dAcBomb_c::unkVirtFunc_0x6C() {
    if (isState(StateID_Wait)) {
        unsetActorProperty(AC_PROP_0x4);
    }
}

int dAcBomb_c::actorExecute() {}

int dAcBomb_c::draw() {
    if (isState(StateID_Explode) || (getBombType() == BOMB_2 && !dAcPy_c::GetLink()->checkActionFlagsCont(0x400000))) {
        return SUCCEEDED;
    }

    drawModelType1(&mMdl);

    mQuat_c rot(0.f, mScale.x * sHIO.getBombScale(), 0.f, mScale.x * sHIO.getBombScale());

    if (!isState(StateID_FlowerWait)) {
        drawShadow(mShdw, nullptr, mWorldMtx, &rot, -1, -1, -1, -1, -1, mPosition.y - mAcch.GetGroundH());
    }
    return SUCCEEDED;
}
