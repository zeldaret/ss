#include "d/a/obj/d_a_obj_grass_coil.h"

#include "c/c_lib.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(OBJ_GRASS_COIL, dAcOgrassCoil_c, fProfile::OBJ_GRASS_COIL, 0x263, 0, 2);

STATE_DEFINE(dAcOgrassCoil_c, Wait);

dCcD_SrcCyl dAcOgrassCoil_c::sCylSrc = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {AT_TYPE_SWORD | AT_TYPE_BOMB | AT_TYPE_0x40 | AT_TYPE_SLINGSHOT | AT_TYPE_WHIP | AT_TYPE_ARROW |
          AT_TYPE_CLAWSHOT | AT_TYPE_0x800000,
      0x200111,
      {0, 0xb, 0x407},
      0x0,
      0x0},
     /* mObjCo */ {0x29}},
    /* mCylInf */
    {50.f, 130.f}
};

const u32 dAcOgrassCoil_c::sCalcAngleRatio = 5;

bool dAcOgrassCoil_c::createHeap() {
    void *data = getOarcResFile("GrassCoilNormal");
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("GrassCoilNormal");
    TRY_CREATE(mMdl[0].create(mdl, &mAllocator, 0x120, 1, nullptr));
    nw4r::g3d::ResMdl mdlCut = mResFile.GetResMdl("GrassCoilNormalCut");
    TRY_CREATE(mMdl[1].create(mdlCut, &mAllocator, 0x120, 1, nullptr));
    return SUCCEEDED;
}

int dAcOgrassCoil_c::create() {
    mSpawnRotY = mRotation.y;
    mRotation.y = 0;
    CREATE_ALLOCATOR(dAcOgrassCoil_c);
    mStts.SetRank(8);
    mCollider.Set(sCylSrc);
    mCollider.SetStts(mStts);
    updateMatrix();
    mWorldMtx.YrotM(mSpawnRotY - mRotation.y);
    mMdl[mCut].setLocalMtx(mWorldMtx);
    mStateMgr.changeState(StateID_Wait);
    mCullingDistance = 9000.0f;
    mBoundingBox.Set(mVec3_c(-100.0f, -50.0f, -100.0f), mVec3_c(100.0f, 200.0f, 100.0f));
    field_0x1A8 = 0.0f;
    return SUCCEEDED;
}

int dAcOgrassCoil_c::doDelete() {
    return SUCCEEDED;
}

const s16 dAcOgrassCoil_c::sEffectRotIncrement = 0xe39; // ~20 degrees

int dAcOgrassCoil_c::actorExecute() {
    if (checkObjectProperty(OBJ_PROP_0x1)) {
        if (dAcPy_c::GetLink()->mPosition.squareDistanceToXZ(mPosition) > 1000000.f) {
            return SUCCEEDED;
        }
    }
    if (!mCut) {
        AttentionManager::GetInstance()->addUnk3Target(*this, 1, 250.0f, -100.0f, 100.0f);
    }
    if (!mCut) {
        bool hitBySlingshot = false;
        if (mCollider.ChkTgHit()) {
            if (mCollider.ChkTgAtHitType(AT_TYPE_SLINGSHOT)) {
                hitBySlingshot = true;
            } else {
                mEffectRot.y = mSpawnRotY;
                for (s32 i = 0; i < 2; i++) {
                    if (i == 1) {
                        mEffectRot.y += mAng(0xe39);
                    }
                    dEmitterBase_c *emitter = dJEffManager_c::spawnEffect(
                        PARTICLE_RESOURCE_ID_MAPPING_485_, mPosition, &mEffectRot, nullptr, nullptr, nullptr, 0, 0
                    );
                    if (emitter != nullptr) {
                        emitter->bindShpEmitter(dJEffManager_c::GrassCoil, true);
                    }
                }
                mCollider.ClrTgSet();
                mCut = true;
                mSwayAmt = -mAng::d2s(18.0011);
                startSound(SE_Gcoil_CUT);
                mCollider.SetR(25.0f);
                mCollider.SetH(70.0f);
                mSpawnPos = mPosition;
                mSpawnPos.y += 30.0f;
                itemDroppingAndGivingRelated(&mSpawnPos, 0);
            }
        }
        if (hitBySlingshot || (mSwayAmt == 0 && mCollider.ChkCoHit() && dAcPy_c::GetLink()->mSpeed > 0.0f)) {
            if (!mCut) {
                mSwayAmt = -mAng::d2s(13.0025);
            } else {
                mSwayAmt = -mAng::d2s(2);
            }
            mTargetRotY = cLib::targetAngleY(dAcPy_c::GetLink()->mPosition, mPosition) - 0x8000;
        }
    }

    if (mSwayAmt != 0) {
        s32 _a = 0;

        mSwayAmt += s16(-mTargetRotX * (0.35f + (0.01f * _a)));
        mSwayAmt *= 0.85f + (0.01f * _a);
        mTargetRotX += mSwayAmt;
    }
    if (mTargetRotX != mRotation.x) {
        sLib::addCalcAngle(mRotation.x.ref(), mTargetRotX, 5, mAng::d2s(7), mAng::d2s(1));
    }
    if (mTargetRotY != mRotation.y) {
        sLib::addCalcAngle(mRotation.y.ref(), mTargetRotY, 5, mAng::d2s(7), mAng::d2s(1));
    }
    updateMatrix();
    mWorldMtx.YrotM(mSpawnRotY - mRotation.y);
    mMdl[mCut].setLocalMtx(mWorldMtx);
    if (!mCut) {
        mCollider.SetC(mPosition);
        dCcS::GetInstance()->Set(&mCollider);
    }
    return SUCCEEDED;
}

const float dAcOgrassCoil_c::sSqDistThreshold = 1000000.f;
const u32 dAcOgrassCoil_c::lbl_268_rodata_4C = 2;

int dAcOgrassCoil_c::draw() {
    drawModelType1(&mMdl[mCut]);
    return SUCCEEDED;
}

void dAcOgrassCoil_c::initializeState_Wait() {}
void dAcOgrassCoil_c::executeState_Wait() {}
void dAcOgrassCoil_c::finalizeState_Wait() {}
