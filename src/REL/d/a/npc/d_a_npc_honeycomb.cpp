#include "d/a/npc/d_a_npc_honeycomb.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_boomerang.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/c/c_cc_d.h"
#include "d/d_light_env.h"
#include "d/d_linkage.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m_allocator.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/math/math_triangular.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(NPC_HONEYCOMB, dAcNpcHc_c, fProfile::NPC_HONEYCOMB, 0x124, 0, 2);

static dCcD_SrcSph srcSph = {
    /* mObjInf */
    {/* mObjAt */ {},
     /* mObjTg */
     {~(AT_TYPE_BUGNET | AT_TYPE_0x2000000 | AT_TYPE_0x8000 | AT_TYPE_WIND | AT_TYPE_BEETLE |
        AT_TYPE_GLITTERING_SPORES),
      0x103,
      {0, 0, 0x40F},
      0,
      0},
     /* mObjCo */ {0x89}},
    /* mSphInf */
    {40.0f}
};

bool dAcNpcHc_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("Bee"));
    return mModel.create(mRes.GetResMdl("home"), &mAllocator, 0x20, 1, nullptr);
}

int dAcNpcHc_c::create() {
    mSceneflag = getFromParams(0x10, 0xFF);
    CREATE_ALLOCATOR(dAcNpcHc_c);
    mObjAcch.Set(this, 1, &mAcchCir);
    mAcchCir.SetWall(35.f, 35.f);
    mCollider.addCc(mSph, srcSph);
    mStts.SetRank(8);
    mCollider.SetStts(mStts);
    mAcceleration = -5.f;
    mMaxSpeed = -50.f;
    mBoundingBox.Set(mVec3_c(-200.f, -200.f, -200.f), mVec3_c(200.f, 200.f, 200.f));
    mRandomnessTimer = cM::rndF(65536.f);
    field_0x934 = mPosition;
    mHealth = 10000;
    field_0x918 = 1;
    mLinkage.set(0x80, 0.0f, 30.f, 70.f, nullptr);
    return SUCCEEDED;
}

int dAcNpcHc_c::doDelete() {
    return SUCCEEDED;
}

void dAcNpcHc_c::handleCollider() {
    if (sLib::calcTimer(&mNoCollisionTimer) == 0 && someEnemyDamageCollisionStuffMaybe(mCollider, nullptr)) {
        mNoCollisionTimer = 5;
        cCcD_Obj *found = mCollider.find(&cCcD_Obj::ChkTgHit);
        if (found != nullptr) {
            mVec3_c local_58;
            if (found->ChkTgAtHitType(AT_TYPE_0x40) && dAcPy_c::hasBeetleVariantOrBetter(dAcBoomerang_c::HOOK_BEETLE)) {
                return;
            }
            if (found->ChkTgAtHitType(AT_TYPE_BUBBLE)) {
                field_0x918 = 1;
            } else if (found->ChkTgSkywardStrike()) {
                mDeathTimer = 10;
                mHealth = 0;
                return;
            }
            if (found->ChkTgAtHitType(AT_TYPE_BELLOWS)) {
                mIsBlown = true;
                local_58.x = 0.f;
                local_58.y = 0.f;
                local_58.z = -5.f;
                mBeeAngerTimer = 10;
                mNoCollisionTimer = 0;
                mGustBellowsTimer += 2;
                if (mGustBellowsTimer >= 60) {
                    mFallDownTimer = 1;
                    mNoCollisionTimer = 200;
                }
            } else {
                mBeeAngerTimer = 10;
                mIsBlown = false;
                local_58.x = 0.f;
                local_58.y = 0.f;
                local_58.z = -100.f;
                field_0x918--;
                // since the field is only ever set to 1, this will always succeed
                if (field_0x918 <= 0) {
                    mFallDownTimer = 0xf;
                    mNoCollisionTimer = 200;
                }
            }
            mMtx_c mtx;
            mtx.YrotS(found->GetTgAtHitDir().atan2sX_Z());
            mtx.multVec(local_58, local_58);
            field_0x920 += local_58.x;
            field_0x924 += local_58.z;
        }
    }
}

extern "C" bool fn_80030390(f32, dAcObjBase_c *);
extern "C" void spawnDrop(u16, s32, void *, void *);

// non matching
int dAcNpcHc_c::actorExecute() {
    if (mDeathTimer != 0) {
        if (--mDeathTimer == 0) {
            killWithFlag();
            if (mSceneflag != 0xFF && SceneflagManager::sInstance) {
                SceneflagManager::sInstance->setFlag(getRoomId(), mSceneflag);
            }
        }
        return SUCCEEDED;
    }
    mRandomnessTimer++;
    dAcPy_c *player = dAcPy_c::GetLinkM();
    if (mUnusedTimer1 != 0) {
        mUnusedTimer1--;
    }
    if (mUnusedTimer2 != 0) {
        mUnusedTimer2--;
    }
    if (mGustBellowsTimer != 0) {
        mGustBellowsTimer--;
    }
    if (player->checkFlags0x350(0x2000) && getSquareDistanceTo(player->mPosition) < 160000.f &&
        getHeightDifference(*player) < 800.f) {
        mBeeAngerTimer = 10;
        mIsBlown = false;
        mVec3_c local_11c(0.f, 0.f, -100.f);
        mFallDownTimer = 0xf;
        mNoCollisionTimer = 200;
        mMtx_c mtx;
        mtx.YrotS(player->mRotation.y);
        mtx.multVec(local_11c, local_11c);
        field_0x920 += local_11c.x;
        field_0x924 += local_11c.z;
    } else if (dLightEnv_c::GetPInstance()->check_BPM8(&mPosition, nullptr)) {
        mBeeAngerTimer = 5;
    } else if ((mRandomnessTimer & 0xF) == 0) {
        // check if there is something in the way between the hive and the player
        mPlayerObstructed = fn_80030390(0.f, this);
    }
    if (mBeeAngerTimer != 0) {
        mBeeAngerTimer--;
    }
    if (mLinkage.checkConnection(dLinkage_c::CONNECTION_5)) {
        setStartingPosition(mPosition);
        field_0x934 = mPosition;
        mFallDownTimer = 1;
        dAcObjBase_c *beetle = player->getBeetleInFlight();
        if (beetle != nullptr) {
            mRotation.y = beetle->mRotation.y;
            f32 fVar11 = nw4r::math::SinIdx((s16)(mRandomnessTimer * 2048));
            s32 speed = beetle->mSpeed;
            s32 temp = fVar11 * 1000.f;
            // regalloc
            s16 temp2 = speed * 0x96 + temp;
            sLib::addCalcAngle(mRotation.x.ref(), temp2 + beetle->mRotation.x, 8, 0x400);
            sLib::addCalcAngle(mRotation.z.ref(), (beetle->mRotation.z * 2) / 3, 8, 0x400);
        }
        mWorldMtx.transS(mPosition);
        mWorldMtx.YrotM(mRotation.y);
        mWorldMtx.XrotM(mRotation.x);
        mWorldMtx.ZrotM(mRotation.z);
        mWorldMtx.transM(0.f, -80.f, 0.f);
        mMtx_c mtx;
        mtx.YrotS(mRotation.y);
        mtx.multVec(mVec3_c(0.f, -0.5f, 30.f), mVelocity);
        mPosition.y -= 130.f;
        mObjAcch.CrrPos(*dBgS::GetInstance());
        mPosition.y += 130.f;
        if (mObjAcch.ChkGndHit()) {
            mDeathTimer = 10;
            mHealth = 1;
        }
    } else {
        if (mFallDownTimer != 0) {
            if (mFallDownTimer >= 2) {
                mFallDownTimer--;
            } else {
                mPosition += mVelocity;
                mVelocity.y -= 5.f;
                mPosition.y -= 130.f;
                mObjAcch.CrrPos(*dBgS::GetInstance());
                mPosition.y += 130.f;
                if (mObjAcch.ChkGndHit()) {
                    mDeathTimer = 10;
                    mHealth = 1;
                }
            }
        } else {
            handleCollider();
        }
        sLib::addCalcAngle(mRotation.x.ref(), mAngle.x, 0x20, 0x80);
        sLib::addCalcAngle(mAngle.x.ref(), 0, 0x2, 0x800);
        f32 fVar11 = field_0x934.x - mStartingPos.x;
        f32 fVar1 = field_0x934.z - mStartingPos.z;
        f32 twoHundred = 200.f;
        if (mIsBlown == true) {
            field_0x920 = (field_0x920 - fVar11 * 0.02f) * 0.9f;
            field_0x924 = (field_0x924 - fVar1 * 0.02f) * 0.9f;
        } else {
            field_0x920 = (field_0x920 - fVar11 * 0.1f) * 0.9f;
            field_0x924 = (field_0x924 - fVar1 * 0.1f) * 0.9f;
        }
        field_0x934.x += field_0x920;
        field_0x934.z += field_0x924;
        s16 sVar8 = -cM::atan2s(twoHundred, fVar1);
        // f32 fVar12;
        // // f32 fVar2 = twoHundred * twoHundred + fVar1 * fVar1;
        // if (twoHundred * twoHundred + fVar1 * fVar1 <= 0.f) {
        //     fVar12 = (twoHundred * twoHundred + fVar1 * fVar1);
        // } else {
        //     fVar12 = (twoHundred * twoHundred + fVar1 * fVar1) * nw4r::math::FrSqrt(twoHundred * twoHundred + fVar1 *
        //     fVar1);
        // }
        // wtf
        s16 sVar9 = cM::atan2s(
            fVar11, (twoHundred * twoHundred + fVar1 * fVar1 <= 0.f ?
                         twoHundred * twoHundred + fVar1 * fVar1 :
                         (twoHundred * twoHundred + fVar1 * fVar1) *
                             nw4r::math::FrSqrt(twoHundred * twoHundred + fVar1 * fVar1))
        );
        mWorldMtx.transS(mPosition);
        mWorldMtx.XrotM(sVar8);
        mWorldMtx.YrotM(sVar9);
        mWorldMtx.XrotM(0x4000);
        mWorldMtx.transM(0.f, -90.f, 0.f);
    }
    mModel.setLocalMtx(mWorldMtx);
    mWorldMtx.multVec(mVec3_c(0.f, 1.f, 0.f), mActualPosition);
    mSph.SetC(mActualPosition);
    mSph.SetR(50.f);
    if (mLinkage.checkConnection(dLinkage_c::CONNECTION_5)) {
        mNoCollisionTimer = 5;
    } else {
        mCollider.registerColliders();
    }
    mPositionCopy3 = mActualPosition;
    mPositionCopy2 = mActualPosition;
    AttentionManager::sInstance->addUnk3Target(*this, 1, 500.f, -400.f, 400.f);
    if (mDeathTimer != 0) {
        startSound(SE_HnComb_BREAK);
        static mVec3_c temp(1.f, 1.f, 1.f);
        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_249_, mActualPosition, nullptr, &temp, nullptr, nullptr, nullptr, nullptr
        );
        dEmitterBase_c *eff = dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_477_, mActualPosition, nullptr, &temp, nullptr, nullptr, nullptr, nullptr
        );
        if (eff != nullptr) {
            eff->bindShpEmitter(6, true);
        }
        mAng3_c ang(0.f, 0.f, 0.f);
        spawnDrop(ITEM_HORNET_LARVAE, -1, &mActualPosition, &ang);
    }
    return SUCCEEDED;
}

int dAcNpcHc_c::draw() {
    if (mDeathTimer == 0) {
        drawModelType1(&mModel);
    }
    return SUCCEEDED;
}
