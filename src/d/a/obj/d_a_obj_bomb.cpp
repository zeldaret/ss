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
#include "m/m_mtx.h"
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
#include "toBeSorted/minigame_mgr.h"

// For real??
template <typename T>
inline T MyClamp(T value, T min, T max) {
    return (value < min ? min : value > max ? max : value);
}

SPECIAL_ACTOR_PROFILE(BOMB, dAcBomb_c, fProfile::BOMB, 0x128, 0, 2);

static const Vec sSmokeOffset = {0, 60, 0};

struct dAcBomc_HIO_c {
    static const f32 getBBoxX() {
        return 36.0f;
    }
    static const f32 getBBoxY() {
        return 66.0f;
    }
    static const f32 getBBoxZ() {
        return 5.0f;
    }
    static const f32 getBombScale() {
        return 30.0f;
    }
    static const s32 getSomething() {
        return 40;
    }
    static const mVec3_c getBBoxMin() {
        return mVec3_c(-bbox_x, 0.0f, -bbox_x);
    }
    static const mVec3_c getBBoxMax() {
        return mVec3_c(bbox_x, bbox_y, bbox_x);
    }
    static const f32 bbox_x, bbox_y, bbox_z;
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

dCcD_SrcSph sSphSrc = {
    /* mObjInf */
    {/* mObjAt */ {AT_TYPE_BOMB, 0x1803E, {0, 0, 0}, sHIO.getSomething(), 0, 0, 0, 0, 0},
     /* mObjTg */
     {~(AT_TYPE_BUGNET | AT_TYPE_GLITTERING_SPORES | AT_TYPE_0x8000 | AT_TYPE_0x10), 0x800111, {0, 0x0A, 0x40F}, 0, 0},
     /* mObjCo */ {0xE9}},
    /* mSphInf */
    {sHIO.getBombScale()}
};

void dAcBomb_c::coHitCallback(cCcD_Obj *i_objInfB) {
    if (!i_objInfB->ChkCo_0x8000()) {
        return;
    }

    setFlag(dAcBomb_c::FLAG_BOMB_HIT);
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
        if (!getLinkage().checkState(dLinkage_c::STATE_ACTIVE) && !checkFlag(dAcBomb_c::FLAG_BOMB_HIT)) {
            if (!(isState(StateID_FlowerWait) || isState(StateID_Explode))) {
                cLib::addCalcPosXZ(&mHitForce, i_objInfA->GetTgAtHitDir() * 0.5f, 0.5f, 10.0f, 1.0f);
                setFlag(dAcBomb_c::FLAG_0x8000);
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
            setXYZCirclePoint(target, pPlayer->mRotation.y, 30.0f, 0.0f);
            rollTo(target);
        } else if (i_objInfB->ChkAtType(
                       AT_TYPE_SWORD | AT_TYPE_0x40 | AT_TYPE_SLINGSHOT | AT_TYPE_0x20000 | AT_TYPE_BEETLE
                   ) &&
                   checkFlag(dAcBomb_c::FLAG_FLOWER | dAcBomb_c::FLAG_0x400000)) {
            unsetFlag(dAcBomb_c::FLAG_FLOWER);
            setFlag(dAcBomb_c::FLAG_0x400000);
            unsetActorProperty(AC_PROP_0x1);
            if (isState(StateID_FlowerWait)) {
                dAcPy_c::insertOwnedObject(this);
            }
            if (!checkFlag(dAcBomb_c::FLAG_0x100) && i_objInfB->ChkAtType(AT_TYPE_SWORD)) {
                setFlag(dAcBomb_c::FLAG_0x100);
            }
        } else {
            setFlag(dAcBomb_c::FLAG_BOMB_HIT);
            setActorProperty(AC_PROP_0x4);
            if (i_objInfB->ChkAtType(AT_TYPE_BOMB)) {
                setFlag(dAcBomb_c::FLAG_CHAIN_BOMB);
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

    if (getBombType() == BOMBF_CRAWLSPACE) {
        mMdl.setPriorityDraw(0x82, 0x7F);
    }
    return result;
}

int dAcBomb_c::create() {
    CREATE_ALLOCATOR(dAcBomb_c);

    mAcch.Set(this, 1, &mAcchCir);
    mAcch.SetRoofCrrHeight(60.0f);
    mAcch.SetGndChkOffset(60.0f);
    mAcch.GetRoof().SetField_0x7C(1);
    mAcch.SetWaterCheckOffset(10000.0f);
    mAcch.SetWtrChkMode(2);
    mAcch.ClrWaterNone();
    mAcch.ClrRoofNone();
    mAcch.OnLineCheck();

    mAcchCir.SetWall(30.0f, 30.0f);

    if (getBombType() == BOMBF_CRAWLSPACE) {
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

    mLinkage.set(0xc9c2, 25.0f, 43.0f, 36.0f, nullptr);
    mLinkage.setField_0x8C_0x90(dAcPy_c::getBombUnknown0_0(), dAcPy_c::getBombUnknown0_1());
    mLinkage.setField_0x94_0x98(dAcPy_c::getBombUnknown1_0(), dAcPy_c::getBombUnknown1_1());
    field_0xA50 = 1.0f;
    mVec3_c pos = (mPosition);
    mAcch.CrrPos(*dBgS::GetInstance());
    setRoomId();

    switch (getBombType()) {
        case BOMB_0:
        case BOMB_4:
        case BOMB_EXPLODE_NO_DAMAGE: {
            changeState(StateID_Explode);
        } break;
        case BOMB_PLAYER_CARRY: {
            setFlag(dAcBomb_c::FLAG_0x200000);
            setFlag(dAcBomb_c::FLAG_PLAYER_MAKE);
            if (checkWaterIn()) {
                setFlag(dAcBomb_c::FLAG_UNDERWATER);
                mAcceleration = dAcPy_c::getBombAcceleration_1();
                mMaxSpeed = dAcPy_c::getBombMaxSpeed_1();
            }
            changeState(StateID_Carry);
        } break;
        case BOMB_BOCO_CARRY: {
            setFlag(dAcBomb_c::FLAG_BOKO_CARRY);
            changeState(StateID_Carry);
        } break;
        case BOMBF:
        case BOMBF_CRAWLSPACE:
        case BOMB_5:           {
            mPosition = pos;
            changeState(StateID_FlowerWait);
        } break;
        default: {
            changeState(StateID_Wait);
        } break;
    }

    f32 yOffset = mScale.y * 25.0f;
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
    mExternalTranslation = mPosition;

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
    if (mCarryTimer != 0) {
        mCarryTimer--;
    }

    if (mFuseTime > 0) {
        bool b = !checkFlag(
                     dAcBomb_c::FLAG_FLOWER | dAcBomb_c::FLAG_0x200000 | dAcBomb_c::FLAG_FALL_IN_WATER |
                     dAcBomb_c::FLAG_NEAR_TIMEDOOR
                 ) &&
                 !isState(StateID_Explode);
        if (b) {
            mFuseTime--;
            if (checkFlag(dAcBomb_c::FLAG_BOKO_CARRY | dAcBomb_c::FLAG_FUSE_PAUSED)) {
                s16 explodeTimer = dAcPy_c::getBombExplodeTimer();
                if (mFuseTime == (explodeTimer >> 1) || mFuseTime == (explodeTimer >> 2) ||
                    mFuseTime == (explodeTimer >> 3) || mFuseTime == (explodeTimer >> 4)) {
                    mFuseTime <<= 1;
                }
            }
        }
    }

    if (checkFlag(dAcBomb_c::FLAG_FUSE_PAUSED | dAcBomb_c::FLAG_FALL_IN_WATER | dAcBomb_c::FLAG_NEAR_TIMEDOOR)) {
        return false;
    }

    if (mFuseTime == 0 || checkFlag(dAcBomb_c::FLAG_BOMB_HIT) ||
        (mSph.ChkCoHit() && mSph.GetCoActor()->isActorPlayer() &&
         static_cast<dAcPy_c *>(mSph.GetCoActor())->isBurning()) ||
        (checkFlag(dAcBomb_c::FLAG_BEETLE_CARRY) && mCarryTimer == 0) &&
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
    mVec3_c in = sSmokeOffset;
    mWorldMtx.multVec(in, mSmokePosNext);
    mSmokePos = (mSmokePosNext - mSmokePosPrev) * 0.5f;
}

bool dAcBomb_c::checkWaterIn() {
    return checkFlag(dAcBomb_c::FLAG_0x20000000) ||
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

    f32 f = field_0xA50 * (25.0f * mScale.y);
    m.getTranslation(mExternalTranslation);

    mPosition = mExternalTranslation + out * f;
    mPosition.y -= f;
}

void dAcBomb_c::setPickupTarget() {
    if (checkFlag(dAcBomb_c::FLAG_FALL_IN_WATER)) {
        return;
    }

    AttentionManager::GetInstance()->addPickUpTarget(*this, 120.0f);
}

void dAcBomb_c::lookAt() {
    dAcPy_c::GetLink2()->lookTowardItem(mPositionCopy2, 4, 0x5555, 300, 400, -400);
}

void dAcBomb_c::bounceWall(mAng angle) {
    mSpeed *= dAcPy_c::getBombBounceSpeed();
    mRollSpeed *= dAcPy_c::getBombBounceSpeed();

    mAngle.y = angle - mAng((mAngle.y + 0x8000) - angle);
    mRollRotationY = angle - mAng((mRollRotationY + 0x8000) - angle);
}

void dAcBomb_c::getVelocity(mVec3_c &out) {
    // out = Bomb Speed (Standard Parameters)
    setXYZCirclePoint(out, mAngle.y, mSpeed, mVelocity.y);

    if (mRollFrameTimer != 0) {
        f32 f = MyClamp(mRollFrameTimer * 0.02f, 0.0f, 1.0f);

        mVec3_c v;
        setXYZCirclePoint(v, mRollRotationY, mRollSpeed, 0.0f);

        out = out * (1.0f - f) + v * f;
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
    mSmokePos = mVec3_c::Zero;
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

        mPosition.y += 25.0f;

        mPosition -= out * 25.0f;

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

    mVec3_c temp(dAcPy_c::getBombParticleScale(), dAcPy_c::getBombParticleScale(), dAcPy_c::getBombParticleScale());
    if (d < dAcPy_c::getBombDistanceUnknown0()) {
        dEmitterBase_c *pEmitter = dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_322_, mPosition, nullptr, &temp, nullptr, nullptr, 0, 0
        );
        if (pEmitter != nullptr) {
            pEmitter->setRate(10.0f);
        }
        (void)dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_323_, mPosition, nullptr, &temp, nullptr, nullptr, 0, 0
        );
        pEmitter = dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_324_, mPosition, nullptr, &temp, nullptr, nullptr, 0, 0
        );
        if (pEmitter != nullptr) {
            pEmitter->setRate(5.0f);
        }
    } else if (checkFlag(dAcBomb_c::FLAG_CHAIN_BOMB)) {
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

    mSpeed = 0.0f;
    mVelocity = mVec3_c::Zero;
    mAcceleration = 0.0f;

    mSph.ClrTgSet();
    mSph.ClrCoSet();
    if (getBombType() == BOMB_EXPLODE_NO_DAMAGE) {
        mSph.ClrAtSet();
    } else {
        mSph.OnAtSet();
    }

    if (getBombType() == BOMBF_CRAWLSPACE) {
        mSph.SetR(200.0f);
    } else {
        mSph.SetR(dAcPy_c::getBombExplodeRadius());
    }

    mSph.SetC(mPosition);
    mSph.SetAtType(AT_TYPE_BOMB);
    mSph.SetAtVec(mVec3_c::Zero);
    dCcS::GetInstance()->Set(&mSph);
    dCcS::GetInstance()->SetMass(&mSph, 1);

    startSound(SE_BM_EXPLODE);
    mLinkage.forceRemove(this);
    unsetObjectProperty(OBJ_PROP_0x1000);
    mFuseTime = 0;

    f32 dist = mPosition.squareDistance(pPlayer->mPosition);
    if (dist < 160000.0f) {
        dRumble_c::start(dRumble_c::sRumblePreset3, dRumble_c::FLAG_SLOT0 | dRumble_c::FLAG_ACTIVE);
    } else if (dist < 640000.0f) {
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
    unsetFlag(dAcBomb_c::FLAG_BEETLE_CARRY | dAcBomb_c::FLAG_0x80000 | dAcBomb_c::FLAG_ROLL | dAcBomb_c::FLAG_0x10000);
    mHitForce = mVec3_c::Zero;
    if (mLinkage.checkConnection(dLinkage_c::CONNECTION_5)) {
        setFlag(dAcBomb_c::FLAG_BEETLE_CARRY);
        mCarryTimer = 5;
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

    mSpeed = 0.0f;
    mVelocity = mVec3_c::Zero;

    mLinkage.bushTpFunc(mAcch);
    setRoomId();
}
void dAcBomb_c::executeState_Carry() {
    dAcPy_c *pPlayer = dAcPy_c::GetLink2();

    if (checkExplode()) {
        unsetFlag(dAcBomb_c::FLAG_BOKO_CARRY | dAcBomb_c::FLAG_FLOWER | dAcBomb_c::FLAG_0x200000);
    } else {
        if (!mLinkage.checkState(dLinkage_c::STATE_ACTIVE)) {
            if (checkFlag(dAcBomb_c::FLAG_0x100000)) {
                unsetFlag(dAcBomb_c::FLAG_0x100000);
                mMtx.ZrotS(-mRotation.z);
                mMtx.XrotM(-mRotation.x);
                mMtx.YrotM(-mRotation.y);
                mMtx.concat(mWorldMtx);
                mMtx.m[0][3] = 0.0f;
                mMtx.m[1][3] = 0.0f;
                mMtx.m[2][3] = 0.0f;
            }
            if (mLinkage.checkState(dLinkage_c::STATE_3)) {
                setFlag(dAcBomb_c::FLAG_0x80000 | dAcBomb_c::FLAG_ROLL);
                mRollRotationY = pPlayer->mRotation.y;
                mRollSpeed = mSpeed;
                mRollFrameTimer = 60;
            }
            unsetFlag(dAcBomb_c::FLAG_BOKO_CARRY | dAcBomb_c::FLAG_FLOWER | dAcBomb_c::FLAG_0x200000);
            changeState(StateID_Wait);
        } else if (!mLinkage.checkConnection(dLinkage_c::CONNECTION_5)) {
            unsetFlag(dAcBomb_c::FLAG_BEETLE_CARRY | dAcBomb_c::FLAG_FLOWER);
        }
        mLinkage.bushTpFunc(mAcch);
        setRoomId();
        if (isState(StateID_Carry)) {
            mSpeed = 0.0f;
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
    if (checkFlag(dAcBomb_c::FLAG_0x4000000)) {
        unsetFlag(dAcBomb_c::FLAG_0x4000000);
        setFlag(dAcBomb_c::FLAG_BEETLE_CARRY);
    }
}
// NONMATCHING
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

    mVec3_c bombVelocity;
    getVelocity(bombVelocity);

    s16 bombAngleY = bombVelocity.atan2sX_Z();
    f32 bombSpeed = bombVelocity.absXZ();

    mAng gndAngle;

    if (mAcch.ChkGndHit()) {
        gndAngle = mAcch.GetGnd().GetAngle(bombAngleY);
        mVelocity.y -= bombSpeed * nw4r::math::FAbs(gndAngle.sin());

        bombVelocity *= gndAngle.cos();

        if (mRollFrameTimer != 0) {
            mRollFrameTimer--;
        }
    } else {
        gndAngle = 0;
    }

    f32 prevAccel = mAcceleration;
    mVelocity.y = nw4r::ut::Max(mVelocity.y + mAcceleration, mMaxSpeed);
    bombVelocity.y = mVelocity.y;
    mPosition += bombVelocity;
    mPosition += mStts.GetCcMove();

    if (checkFlag(dAcBomb_c::FLAG_0x10000) && checkFlag(dAcBomb_c::FLAG_UNDERWATER) && mAcch.ChkWaterHit()) {
        if (mPosition.y + (mScale.x * sHIO.getBombScale()) + 1.0f >= mAcch.GetWtrGroundH()) {
            mPosition.y = mAcch.GetWtrGroundH() - (mScale.x * sHIO.getBombScale()) - 1.0f;
            mVelocity.y = 0.0f;
        }
    }
    mAcceleration = prevAccel;
    f32 prevVelocityY = mVelocity.y;
    s16 a0 = 0;
    s16 a1 = 0;
    f32 f = 1.0f;
    if (mAcch.ChkGndHit() && mHitForce.squareMagXZ() > 1.0f) {
        cM3dGPla plane;
        dBgS::GetInstance()->GetTriPla(mAcch.GetGnd(), &plane);

        f = plane.GetAngle(mHitForce.atan2sX_Z()).cos();
        f *= f;
    }

    mPosition += (mHitForce * f) + mStreamForce;
    mAcch.CrrPos(*dBgS::GetInstance());
    f32 sinkTarget = 0.0f;
    f32 sinkRate = 4.0f;
    mVec3_c force = mVec3_c::Zero;
    f32 sinkValue = mYOffset;
    if (mAcch.ChkGndHit() && !mLinkage.checkState(dLinkage_c::STATE_ACTIVE)) {
        s32 attr = dBgS::GetInstance()->GetSpecialCode(mAcch.mGnd);
        if (attr == POLY_ATTR_CURSED_WATER) {
            sinkTarget = -60.0f;
            if (sinkValue >= 0.0f) {
                prevVelocityY *= 0.5f;
                mSpeed *= 0.5f;
            }
            sinkRate = nw4r::math::FAbs(prevVelocityY);
            mVelocity.y = prevVelocityY;
            mAcceleration = dAcPy_c::getBombAcceleration_1();
            mMaxSpeed = dAcPy_c::getBombMaxSpeed_1();
        } else if (attr == POLY_ATTR_SAND_DEEP_INSTANT || attr == POLY_ATTR_SAND_DEEP_SLOW ||
                   attr == POLY_ATTR_SAND_MED) {
            sinkTarget = -10.0f;
            sinkRate = 1.0f;
        }
        if (attr == POLY_ATTR_SAND_DEEP_SLOW || attr == POLY_ATTR_SAND_MED) {
            dTgStream_c::getForce(mPosition, force, dTgStream_c::sSandStreamList);
        }
    }
    cLib::addCalcPosXZ(&mStreamForce, force, 0.5f, 10.0f, 3.0f);
    sLib::chase(&sinkValue, sinkTarget, sinkRate);
    mYOffset = sinkValue;
    if (sinkValue <= -60.0f) {
        setFlag(dAcBomb_c::FLAG_DELETE_BOMB);
    }
    setRoomId();

    if (mAcch.ChkGndHit()) {
        cM3dGPla plane;
        dBgS::GetInstance()->GetTriPla(mAcch.GetGnd(), &plane);
        mGndAngle = cM::atan2s(plane.GetN().absXZ(), plane.GetN().y);

        a1 = plane.GetN().atan2sX_Z();
        a0 = a1 - bombAngleY;
    }
    bool hit = false;
    f32 outXZ = bombVelocity.absXZ();
    if (mAcch.ChkWallHit(nullptr) && mAng::abs(mAcchCir.GetWallAngleY() - bombAngleY) > 0x4400 && outXZ > 1.0f) {
        bounceWall(mAcchCir.GetWallAngleY());
        hit = true;
        startSoundWithFloatParam(SE_BM_BOUND, outXZ);
    } else if (mAcch.ChkRoofHit() && mVelocity.y > 0.0f) {
        startSoundWithFloatParam(SE_BM_BOUND, mVelocity.y);
        mVelocity.y = 0.0f;
    } else if (mAcch.ChkGroundLanding() && mSpeed > 5.0f && mGndAngle >= mAng::fromDeg(50.0f) &&
               mAng::abs(a0) >= 0x4000) {
        bounceWall(a1);
        mVelocity.y = prevVelocityY;
        mAcch.ClrGroundLanding();
        mAcch.i_ClrGroundHit();
        hit = true;
    } else if (mSph.ChkCoHit() && field_0xA42 == 0 && mStts.GetCcMove().absXZ() >= 1.0f) {
        bounceWall(mStts.GetCcMove().atan2sX_Z());
        field_0xA42 = 5;
        hit = true;
    }

    mAng hitGndAngle = gndAngle;
    if (hit) {
        getVelocity(bombVelocity);
        s16 sVar15 = bombVelocity.atan2sX_Z();
        if (mAcch.ChkGndHit()) {
            hitGndAngle = mAcch.GetGnd().GetAngle(sVar15);
        }
    }

    if (mAcch.ChkGroundLanding()) {
        if (prevVelocityY < -5.0f) {
            startSoundWithFloatParam(SE_BM_BOUND, prevVelocityY * -1.0f);
            if (dBgS::GetInstance()->GetPolyAtt0(mAcch.GetGnd()) != POLY_ATT_0_LAVA) {
                dJEffManager_c::spawnGroundEffect(mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 1, 0.8f, field_0x1B0);
            }
        }

        prevVelocityY *= -dAcPy_c::getBombBounceSpeed();
        s32 attr = dBgS::GetInstance()->GetSpecialCode(mAcch.mGnd);

        if (!checkFlag(dAcBomb_c::FLAG_ROLL) &&
            (prevVelocityY < dAcPy_c::getBombStopSpeedY() ||
             (attr == POLY_ATTR_SAND_DEEP_INSTANT ||
              (attr == POLY_ATTR_CURSED_WATER || attr == POLY_ATTR_SAND_DEEP_SLOW || attr == POLY_ATTR_SAND_MED)))) {
            sLib::chase(&mSpeed, 0.0f, 7.0f);
        } else {
            mSpeed *= 0.9f;
            mRollSpeed *= 0.9f;

            if (hitGndAngle < 0) {
                mSpeed *= hitGndAngle.cos();
                mRollSpeed *= hitGndAngle.cos();
            }

            if (checkFlag(dAcBomb_c::FLAG_0x80000)) {
                unsetFlag(dAcBomb_c::FLAG_0x80000);
                mVelocity.y = 0.0f;
            } else if (prevVelocityY > dAcPy_c::getBombMaxSpeedY()) {
                mVelocity.y = dAcPy_c::getBombMaxSpeedY();
            } else {
                mVelocity.y = prevVelocityY;
            }
        }
    } else if (mAcch.ChkGndHit()) {
        cM3dGPla plane;
        dBgS::GetInstance()->GetTriPla(mAcch.mGnd, &plane);
        int code = dBgS::GetInstance()->GetSpecialCode(mAcch.mGnd);

        f32 step;
        if (checkFlag(dAcBomb_c::FLAG_ROLL)) {
            if (code == POLY_ATTR_SAND_DEEP_SLOW || code == POLY_ATTR_SAND_MED) {
                step = 0.2f;
            } else {
                step = 0.1f;
            }
        } else {
            step = 0.75f;
        }
        if (code != POLY_ATTR_CURSED_WATER) {
            if (mGndAngle >= mAng::fromDeg(20.0f)) {
                mVec3_c v1 = 2.0f * plane.GetN() * mGndAngle.sin();
                v1.y = 0.0f;

                mAng yRot = v1.atan2sX_Z();
                f32 compare = mHitForce.squareMagXZ();
                if (compare > 1.0f) {
                    s32 a = yRot - mAng::fromVec(mHitForce);
                    if (mAng::abs(a) >= 0x4000) {
                        mHitForce.rotY(-yRot);
                        mHitForce.z *= 0.99f;
                        sLib::chase(&mHitForce.z, 0.0f, 1.0f);
                        mHitForce.rotY(yRot);
                    }
                }

                if (mAng::abs(yRot - mAngle.y) >= 0x4000) {
                    sLib::chase(&mSpeed, 0.0f, step);
                }
                mSpeed *= 0.99f;
                rollTo(v1);
                mSpeed = nw4r::ut::Min(mSpeed, 30.0f);

                if (mAng::abs(yRot - mRollRotationY) >= 0x4000) {
                    sLib::chase(&mRollSpeed, 0.0f, step);
                }
                mRollSpeed *= 0.99f;
                rollToInternal(v1);
                mRollSpeed = nw4r::ut::Min(mRollSpeed, 30.0f);
            } else {
                sLib::chase(&mRollSpeed, 0.0f, step);
                if (sLib::chase(&mSpeed, 0.0f, step)) {
                    if (mHitForce.getSquareMag() < 1.0f && mStreamForce.getSquareMag() < 1.0f) {
                        setPickupTarget();
                    }
                    unsetFlag(dAcBomb_c::FLAG_ROLL);
                    mSph.ClrAtSet();
                }
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
    setFlag(dAcBomb_c::FLAG_FLOWER);
    mSph.ClrTgSet();
    mScale.set(0.1f, 0.1f, 0.1f);
    mStts.SetRank(0xD);
    mAcch.OffLineCheck();
    mMdl.setScale(mScale);
}
void dAcBomb_c::executeState_FlowerWait() {
    dAcPy_c *pPlayer = dAcPy_c::GetLink2();

    if (mScale.x < 1.0f || field_0xA50 < 1.0f) {
        if (mScale.x < 0.15f && !MinigameManager::GetInstance()->checkInThrillDigger() &&
            ((getBombType() == BOMBF_CRAWLSPACE && pPlayer->checkActionFlagsCont(dAcPy_c::ActFLG1_INCRAWLSPACE)) ||
             (getBombType() != BOMBF_CRAWLSPACE && !pPlayer->checkActionFlagsCont(dAcPy_c::ActFLG1_INCRAWLSPACE)))) {
            startSound(SE_O_BOMB_FLOWER_SPROUT);
        }
        sLib::chase(&mScale.x, 1.0f, 0.1f);
        mScale.z = mScale.y = mScale.x;
        mVec3_c scale = mScale * field_0xA50;
        mMdl.setScale(scale);

        mMtx_c m = mMtx;
        m.setTranslation(mExternalTranslation);
        setTransformFromFlower(m);
        mSph.ClrTgSet();
        return;
    }

    AttentionManager::GetInstance()->addUnk3Target(*this, 1, 250.0f, -200.0f, 200.0f);
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
    if (!(mLinkage.checkConnection(dLinkage_c::CONNECTION_5) && checkFlag(dAcBomb_c::FLAG_FLOWER))) {
        unsetFlag(dAcBomb_c::FLAG_FLOWER);
    }

    if (getBombType() == BOMBF_CRAWLSPACE) {
        mStts.SetRank(4);
    } else {
        mStts.SetRank(5);
    }

    unsetActorProperty(AC_PROP_0x1);
    mAcch.OnLineCheck();
}

void dAcBomb_c::initializeState_WindCarry() {
    unsetFlag(dAcBomb_c::FLAG_BEETLE_CARRY | dAcBomb_c::FLAG_0x80000 | dAcBomb_c::FLAG_ROLL | dAcBomb_c::FLAG_0x10000);
    mHitForce = mVec3_c::Zero;
    mSpeed = 0.0f;
    mVelocity = mVec3_c::Zero;
    mRollSpeed = 0.0f;
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
    mSpeed = 0.0f;
    mVelocity = mVec3_c::Zero;
}
void dAcBomb_c::finalizeState_WindCarry() {}

void dAcBomb_c::setBombColor() {
    mColor clr(0, 0, 0, 0xFF);
    if (!checkFlag(dAcBomb_c::FLAG_FALL_IN_WATER)) {
        bool b = !checkFlag(
                     dAcBomb_c::FLAG_FLOWER | dAcBomb_c::FLAG_0x200000 | dAcBomb_c::FLAG_FALL_IN_WATER |
                     dAcBomb_c::FLAG_NEAR_TIMEDOOR
                 ) &&
                 !isState(StateID_Explode);
        if (b) {
            mEffect1.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_974_, mWorldMtx, nullptr, nullptr);
            if (!checkFlag(dAcBomb_c::FLAG_BOKO_CARRY)) {
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
    if (mRollFrameTimer == 0) {
        return;
    }
    mVec3_c v;
    setXYZCirclePoint(v, mRollRotationY, mRollSpeed, 0.0f);
    v += dir;

    v.y = 0.0f;

    mRollRotationY = v.atan2sX_Z();
    mRollSpeed = v.absXZ();
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

// NONMATCHING
int dAcBomb_c::actorExecute() {
    unsetFlag(dAcBomb_c::FLAG_0x400000 | dAcBomb_c::FLAG_0x100);
    mVec3_c v;
    getVelocity(v);

    f32 f = mHitForce.getSquareMag();
    if (!checkFlag(dAcBomb_c::FLAG_0x8000)) {
        if (f > 1.0f) {
            v += mHitForce;
            rollTo(mHitForce);
            rollToInternal(mHitForce);
        }
        f = 0.0f;
        mHitForce = mVec3_c::Zero;
    } else {
        unsetFlag(dAcBomb_c::FLAG_0x8000);
    }

    mGndAngle = 0;
    mSmokePosPrev = mSmokePosNext;

    executeState();

    if (checkFlag(dAcBomb_c::FLAG_DELETE_BOMB)) {
        deleteRequest();
        return SUCCEEDED;
    }

    if (!isState(StateID_Explode)) {
        if (checkFlag(dAcBomb_c::FLAG_FALL_IN_WATER)) {
            if (sLib::chase(&mScale.x, 0.0f, 0.0875f)) {
                deleteRequest();
                return SUCCEEDED;
            }

            mScale.y = mScale.z = mScale.x;
            mMdl.setScale(mScale);
        } else if (mFuseTime < 4) {
            f32 scale = nw4r::ut::Min((4 - mFuseTime) * (1.0f / 7.5f), 0.4f) + 1.0f;
            mScale.set(scale, scale, scale);
            mMdl.setScale(mScale);
        }

        if (checkWaterIn()) {
            if (!checkFlag(dAcBomb_c::FLAG_FALL_IN_WATER | dAcBomb_c::FLAG_UNDERWATER)) {
                if (!checkFlag(dAcBomb_c::FLAG_0x20000000)) {
                    mVec3_c pos;
                    // !!! FPR alloc here is weird I guess
                    f32 diff0 = nw4r::math::FAbs(mOldPosition.y - mPosition.y);
                    f32 diff1 = nw4r::math::FAbs(mAcch.GetWtrGroundH() - mPosition.y);
                    if (diff0 < 1.0f) {
                        pos.x = mPosition.x;
                        pos.y = mAcch.GetWtrGroundH();
                        pos.z = mPosition.z;
                    } else {
                        f32 f = diff1 / diff0;
                        if (f > 1.0f) {
                            f = 1.0f;
                        }
                        pos = f * mOldPosition + (1.0f - f) * mPosition;
                    }
                    dAcPy_c::fn_801E2FC0(pos, mAcch.GetWtr(), 0.8f);
                }
                startSound(SE_BM_FALL_WATER);
                if (mLinkage.checkState(dLinkage_c::STATE_ACTIVE)) {
                    mLinkage.forceRemove(this);
                }
                setFlag(dAcBomb_c::FLAG_FALL_IN_WATER);

                mSmokePos = mVec3_c::Zero;
                mSpeed *= 0.5f;
                mVelocity.y *= 0.5f;

                mAcceleration = dAcPy_c::getBombAcceleration_1();
                mMaxSpeed = dAcPy_c::getBombMaxSpeed_1();
            }

            if (checkFlag(dAcBomb_c::FLAG_DELETE_BOMB)) {
                deleteRequest();
                mSmokePos = mVec3_c::Zero;
                return SUCCEEDED;
            }
        } else {
            unsetFlag(dAcBomb_c::FLAG_UNDERWATER);
            mAcceleration = dAcPy_c::getBombAcceleration();
            mMaxSpeed = dAcPy_c::getBombMaxSpeed();
        }

        if (mSpeed > 1.0f || f > 1.0f) {
            mVec3_c v;
            setXYZCirclePoint(v, mAngle.y, mSpeed, 0.0f);
            bool gndNotHit = true;
            if (f > 1.0f) {
                v += mHitForce;
            } else if (mAcch.ChkGndHit()) {
                gndNotHit = false;
            }

            mMtx_c m;
            f32 f = v.absXZ();
            s32 atan0 = v.atan2sX_Z();
            mAng a1 = f / ((sHIO.getBombScale() * 2.0f * mScale.x) * M_PI) * 65536.0f;
            mAng a0 = mAng(atan0) - mRotation.y;
            m.YrotS(a0);
            if (gndNotHit) {
                a1.mVal >>= 1;
            }
            m.XrotM(a1);
            m.YrotM(-a0);
            MTXConcat(m, mMtx, mMtx);
        }

        f32 scale = mScale.y * 25.0f;
        f32 f = 0.0f;
        if (mGndAngle != 0 && mGndAngle < 0x4000) {
            /// !! Need to load mGndAngle again
            f32 f0 = mGndAngle.cos();
            f32 f_temp = ((1.0f / f0) - 1.0f);
            f = scale * nw4r::ut::Min(f_temp * 0.75f, 0.5f);
        }
        mLinkage.fn_800511E0(this);
        mUnkRef.modifyMtx();

        mMtx_c m2, m0, m1;
        m0.transS(0.0f, scale, 0.0f);
        mWorldMtx.concat(m0);
        mWorldMtx.concat(mMtx);

        m1.transS(0.0f, -scale, 0.0f);
        mWorldMtx.concat(m1);

        m2.transS(0.0f, f, 0.0f);
        MTXConcat(m2, mWorldMtx, mWorldMtx);

        mMdl.setLocalMtx(mWorldMtx);
        mVec3_c in(0.0f, mScale.y * sHIO.getBombScale(), 0.0f);
        mWorldMtx.multVec(in, mPositionCopy2);
        mPositionCopy3 = mPositionCopy2;
        mSph.moveCenter(mPositionCopy2);
        mSph.SetR(mScale.x * sHIO.getBombScale() * field_0xA50);
        if (checkFlag(dAcBomb_c::FLAG_FALL_IN_WATER)) {
            mSph.ClrTgSet();
            mSph.ClrTgHit();
        }
        if (mSph.GetR() > 0.01f) {
            dCcS::GetInstance()->Set(&mSph);
        } else {
            mSph.ClrCoHit();
            mSph.ClrTgHit();
        }

        if (!checkFlag(dAcBomb_c::FLAG_FALL_IN_WATER)) {
            setSmokePos();
        }

        mMdl.calc(false);
        setBombColor();
    }

    return SUCCEEDED;
}

int dAcBomb_c::draw() {
    if (isState(StateID_Explode) ||
        (getBombType() == BOMBF_CRAWLSPACE && !dAcPy_c::GetLink()->checkActionFlagsCont(0x400000))) {
        return SUCCEEDED;
    }

    drawModelType1(&mMdl);

    mQuat_c rot(0.0f, mScale.x * sHIO.getBombScale(), 0.0f, mScale.x * sHIO.getBombScale());

    if (!isState(StateID_FlowerWait)) {
        drawShadow(mShdw, nullptr, mWorldMtx, &rot, -1, -1, -1, -1, -1, mPosition.y - mAcch.GetGroundH());
    }
    return SUCCEEDED;
}
