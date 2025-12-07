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
    field_0x510 = mRotation.y;
    mRotation.y = 0;
    CREATE_ALLOCATOR(dAcOgrassCoil_c);
    mStts.SetRank(8);
    mCollider.Set(sCylSrc);
    mCollider.SetStts(mStts);
    updateMatrix();
    mWorldMtx.YrotM(field_0x510 - mRotation.y);
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

const s16 dAcOgrassCoil_c::sEffectRotIncrement = 0xe39;

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
        bool hit = false;
        if (mCollider.ChkTgHit()) {
            if (mCollider.ChkTgAtHitType(AT_TYPE_SLINGSHOT)) {
                hit = true;
            } else {
                mEffectRot.y = field_0x510;
                u16 effectIndex = PARTICLE_RESOURCE_ID_MAPPING_485_;
                s32 i = 0;
                for (s32 i = 0; i < 2; i++) {
                    if (i == 1) {
                        mEffectRot.y += mAng(sEffectRotIncrement);
                    }
                    dEmitterBase_c *emitter = dJEffManager_c::spawnEffect(
                        effectIndex, mPosition, &mEffectRot, nullptr, nullptr, nullptr, 0, 0
                    );
                    if (emitter != nullptr) {
                        emitter->bindShpEmitter(dJEffManager_c::GrassCoil, true);
                    }
                }
                mCollider.ClrTgSet();
                mCut = true;
                field_0x50E = mAng(-0xccd);
                startSound(SE_Gcoil_CUT);
                mCollider.SetR(25.0f);
                mCollider.SetH(70.0f);
                mSpawnPos = mPosition;
                mSpawnPos.y += 30.0f;
                itemDroppingAndGivingRelated(&mSpawnPos, 0);
            }
        }
        if (hit || !(field_0x50E || !mCollider.ChkCoHit() || !(dAcPy_c::GetLink()->mSpeed > 0.0f))) {
            if (!mCut) {
                field_0x50E = mAng(-0x93f);
            } else {
                field_0x50E = mAng(-0x16c);
            }
            field_0x50C = mAng(cLib::targetAngleY(dAcPy_c::GetLink()->mPosition, mPosition) - 0x8000);
        }
    }

    if (field_0x50E != 0) {
        s32 _a = 0;

        field_0x50E += mAng(-field_0x50A.mVal * (0.35f + (0.01f * _a))).mVal;
        field_0x50E *= 0.85f + (0.01f * _a);
        field_0x50A += field_0x50E;
    }
    if (field_0x50A != mRotation.x) {
        sLib::addCalcAngle(mRotation.x.ref(), field_0x50A, 5, 0x4fa, 0xb6);
    }
    if (field_0x50C != mRotation.y) {
        sLib::addCalcAngle(mRotation.y.ref(), field_0x50C, 5, 0x4fa, 0xb6);
    }
    updateMatrix();
    mWorldMtx.YrotM(field_0x510 - mRotation.y);
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
