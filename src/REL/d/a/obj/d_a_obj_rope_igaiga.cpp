#include "d/a/obj/d_a_obj_rope_igaiga.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_wtr_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/ut/ut_Color.h"
#include "rvl/GX/GXTypes.h"
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(OBJ_ROPE_IGAIGA, dAcOropeIgaiga_c, fProfile::OBJ_ROPE_IGAIGA, 0x265, 0, 2);

STATE_DEFINE(dAcOropeIgaiga_c, Init);
STATE_DEFINE(dAcOropeIgaiga_c, Rope);
STATE_DEFINE(dAcOropeIgaiga_c, Ground);
STATE_DEFINE(dAcOropeIgaiga_c, Tree);
STATE_DEFINE(dAcOropeIgaiga_c, Stick);
STATE_DEFINE(dAcOropeIgaiga_c, Water);

dCcD_SrcSph dAcOropeIgaiga_c::sSphSrc = {
    /* mObjInf */
    {/* mObjAt */ {0x400, 0x1F, {0, 0, 0}, 1, 0, 0, 0, 0, 0},
     /* mObjTg */ {0xFEB77DFF, 0x311, {0, 0, 0x407}, 0, 0},
     /* mObjCo */ {0xA9}},
    /* mSphInf */
    {20.f},
};

const s16 dAcOropeIgaiga_c::sRopeTimerMin1 = 0x2D;
const s16 dAcOropeIgaiga_c::sRopeTimerMin2 = 0x3C;
const f32 dAcOropeIgaiga_c::lbl_257_rodata_4[3] = {500.f, 250.f, 50.f};
const s16 dAcOropeIgaiga_c::lbl_257_rodata_10 = 0x96;
const s16 dAcOropeIgaiga_c::lbl_257_rodata_12 = 0x64;
const s16 dAcOropeIgaiga_c::lbl_257_rodata_14 = 0x41;
const s16 dAcOropeIgaiga_c::lbl_257_rodata_16 = 0x41;
const s16 dAcOropeIgaiga_c::lbl_257_rodata_18 = 0x64;
const s16 dAcOropeIgaiga_c::lbl_257_rodata_1A = 0x64;
const s16 dAcOropeIgaiga_c::lbl_257_rodata_1C = 0x4;
const s16 dAcOropeIgaiga_c::lbl_257_rodata_1E = 0x41;
const s16 dAcOropeIgaiga_c::lbl_257_rodata_20 = 0x64;
const f32 dAcOropeIgaiga_c::lbl_257_rodata_28 = 20.f;

// copy from d_a_obj_fairy - TODO move it to a shared file
inline static void vecCylCalc(mVec3_c &target, const mAng &rot, f32 factor) {
    target.x += factor * rot.sin();
    target.z += factor * rot.cos();
}

bool dAcOropeIgaiga_c::createHeap() {
    void *data = getOarcResFile("NeedleBall");
    if (data == nullptr) {
        return false;
    }
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("NeedleBall");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x120, 1, nullptr));
    return true;
}

int dAcOropeIgaiga_c::create() {
    mStartStateTree = getFromParams(0, 0xF);
    if (mStartStateTree >= 2) {
        mStartStateTree = 0;
    }
    mSceneflag = getFromParams(4, 0xFF);
    CREATE_ALLOCATOR(dAcOropeIgaiga_c);
    mObjAcch.Set(&mPosition, &mOldPosition, this, 1, &mAcchCir, &mVelocity, nullptr, nullptr);
    mAcchCir.SetWall(20.0f, 20.0f);
    mStts.mRank = 0;
    mCollider.Set(sSphSrc);
    mCollider.SetStts(mStts);
    mCollider.ClrAtSet();
    mAcceleration = 0.0f;
    mMaxSpeed = -100.0f;
    mInitTimer = 5;
    mPnts2Idx = 0xff;
    field_0x91E = -1;
    field_0x928 = 0xff;
    if (mStartStateTree == 1) {
        mStateMgr.changeState(StateID_Tree);
    } else {
        mStateMgr.changeState(StateID_Init);
    }
    mBoundingBox.Set(mVec3_c(-50.0f, -50.0f, -50.0f), mVec3_c(50.0f, 50.0f, 50.0f));
    return SUCCEEDED;
}

int dAcOropeIgaiga_c::doDelete() {
    return SUCCEEDED;
}

int dAcOropeIgaiga_c::actorExecute() {
    mStateMgr.executeState();
    if (field_0x91E >= 0) {
        u8 red = 0;
        if (field_0x91E < 0x3C) {
            if ((field_0x91E / 3 & 1) == 0) {
                red = 0xff;
                if (!mSoundPlayed) {
                    startSound(SE_RopeIga_COUNT);
                    mSoundPlayed = true;
                }
            } else {
                mSoundPlayed = false;
            }
        } else if ((field_0x91E & 4) != 0) {
            red = 0xff;
            if (!mSoundPlayed) {
                startSound(SE_RopeIga_COUNT);
                mSoundPlayed = true;
            }
        } else {
            mSoundPlayed = false;
        }

        mMdl.setTevKColorAll(GX_KCOLOR3, mColor(red, 0, 0, 0xff), false);
    }
    f32 prevYPosition = mPosition.y;
    mPosition.y += 20.0f;
    mCollider.SetC(mPosition);
    dCcS::GetInstance()->Set(&mCollider);
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mPosition.y = prevYPosition;
    return SUCCEEDED;
}

int dAcOropeIgaiga_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOropeIgaiga_c::initializeState_Init() {}

void dAcOropeIgaiga_c::executeState_Init() {
    if (mInitTimer != 0) {
        mInitTimer--;
        if (mCollider.ChkCoHit()) {
            dAcOivyRope_c *rope = (dAcOivyRope_c *)mCollider.GetCoActor();
            if (rope != NULL && (rope->mProfileName == fProfile::OBJ_IVY_ROPE) && rope->checkSubtype(1)) {
                ivyRopeRef.link(rope);
                mInitTimer = 0;
                mPnts2Idx = 0.5f + mPosition.distance(rope->mPosition) * rope->lbl_256_rodata_40;
                mPosition = rope->getPnts2()[mPnts2Idx];
                f32 vecCylCalcFactor = cM::rndF(5.f) + 15.f;
                mPositionOffset.x = 0.f;
                mPositionOffset.y = 0.f;
                mPositionOffset.z = 0.f;
                s16 ang1 = cLib::targetAngleY(rope->getTightRopeEnd(), rope->getTightRopeStart());
                s16 ang2 = cM::rndInt(100) & 1 ? -1 : 1;
                vecCylCalc(mPositionOffset, ang1 + ang2 * 0x4000, vecCylCalcFactor);
                f32 positionYOffset = cM::rndFX(15.f);
                mPositionOffset.y += positionYOffset;
                mPosition += mPositionOffset;
                mPositionRelated = mPosition;
            }
        }
    } else if (mPnts2Idx == 255) {
        mStateMgr.changeState(StateID_Ground);
        mVelocityYRelated = 1;
    } else {
        mStateMgr.changeState(StateID_Rope);
    }
}

void dAcOropeIgaiga_c::finalizeState_Init() {
    mCollider.mCo.SetGrp(mCollider.mCo.mSrc.mSPrm & 0x160);
}

void dAcOropeIgaiga_c::initializeState_Rope() {
    mAcceleration = -3.0f;
}

void dAcOropeIgaiga_c::executeState_Rope() {
    if (mRopeTimer != 0) {
        mRopeTimer--;
    } else {
        field_0x922 = 0;
    }
    mVec3_c pos = mPosition;
    bool rope0x1087 = false;
    bool rope0x1002 = false;
    dAcOivyRope_c *rope = ivyRopeRef.get();
    if (rope != nullptr) {
        rope0x1002 = rope->getField_0x1002();
        rope0x1087 = rope->getField_0x1087();
        if (rope0x1002) {
            pos.y = rope->getPnts2()[mPnts2Idx].y;
            pos.y += mPositionOffset.y;
        } else {
            pos = rope->getPnts2()[mPnts2Idx];
            pos += mPositionOffset;
        }
        pos.y -= 20.f;
    }
    if (rope0x1087) {
        s32 val = fn_257_1CA0();
        if (val > 0) {
            u32 ropeTimerMin = sRopeTimerMin1;
            field_0x922 += val;
            if (val == 2) {
                ropeTimerMin = sRopeTimerMin2;
            }
            if (ropeTimerMin > mRopeTimer) {
                mRopeTimer = ropeTimerMin;
            }

            if (mVelocityYRelated == 0) {
                mVelocityYRelated = val + 1;
                mVelocity.y = 5.f * mVelocityYRelated;
                fn_257_1F20();
            }
        }
    }
    if (mVelocityYRelated > 0) {
        mRotation.x += mRotationXOffset;
        calcVelocity();
        mPosition += mVelocity;
        if (mPosition.y < pos.y) {
            mPosition.y = pos.y;
            mVelocityYRelated -= 1;
            if (mVelocityYRelated != 0) {
                mVelocity.y = 7.f * mVelocityYRelated;
            }
        }
    }
    if (mVelocityYRelated == 0) {
        if (rope0x1002) {
            sLib::addCalc(&mPosition.y, pos.y, 0.5f, 20.f, 3.f);
        } else {
            cLib::addCalcPos(&mPosition, pos, 0.5f, 20.f, 3.f);
        }
    }
    s32 val = fn_257_1D20();
    fn_257_1DF0();
    if (val == 1) {
        mStateMgr.changeState(StateID_Stick);
    } else if (val == 3) {
        mStateMgr.changeState(StateID_Ground);
    } else if (field_0x922 >= 3) {
        mAngle.y = cM::rndInt(0xFFFF);
        mSpeed = 5.f;
        mVelocity.y = 20.f;
        mStateMgr.changeState(StateID_Ground);
    }
}

void dAcOropeIgaiga_c::finalizeState_Rope() {}

void dAcOropeIgaiga_c::initializeState_Ground() {
    mVelocityYRelated = -1;
    mAcceleration = -4.f;
    mStts.SetRank(0);
    fn_257_1F20();
    mGroundTimer = 0x1e;
}

void dAcOropeIgaiga_c::executeState_Ground() {
    if (mGroundTimer != 0) {
        mGroundTimer--;
    }
    s32 a = fn_257_1D20();
    fn_257_1DF0();
    calcVelocity();
    mPosition += mVelocity;
    if (a != 0) {
        if (mVelocityYRelated == 0) {
            fn_257_20F0(5.f, 10.f);
        }
    }
    if (mVelocityYRelated != 0) {
        mObjAcch.CrrPos(*dBgS::GetInstance());
        if (mObjAcch.ChkGndHit()) {
            if (mVelocityYRelated == -1) {
                mVelocityYRelated = 3;
                mSpeed = 3.f;
                s16 angleYOffset = cM::rndInt(0x2aab);
                if (angleYOffset & 1) {
                    angleYOffset = -angleYOffset;
                }
                mAngle.y += angleYOffset;
                if (field_0x922 >= 3 || *mStateMgr.getOldStateID() == StateID_Tree) {
                    fn_257_1FD0();
                }
                startSound(SE_RopeIga_GROUND);
            } else if (mVelocityYRelated > 0) {
                mVelocityYRelated--;
                sLib::chase(&mSpeed, 0.f, 1.f);
            }
            if (mVelocityYRelated > 0) {
                mVelocity.y = 7.f * mVelocityYRelated;
            } else if (mVelocityYRelated == 0) {
                mAcceleration = 0.f;
                mSpeed = 0.f;
            }
        }
    } else if (field_0x91E > 0) {
        mAcceleration = -4.f;
        mObjAcch.CrrPos(*dBgS::GetInstance());
    }
    if (mVelocityYRelated != 0 && !mObjAcch.ChkGndHit()) {
        mRotation.x += mRotationXOffset;
    }
    if (a == 1 && mGroundTimer == 0) {
        mStateMgr.changeState(StateID_Stick);
    }
    if (field_0x91E >= 0) {
        f32 posY = mPosition.y;
        if (fn_257_1FF0(&posY, false)) {
            mPositionRelated = mPosition;
            mPositionRelated.y = posY - 20.f;
            mStateMgr.changeState(StateID_Water);
        }
    }
}

void dAcOropeIgaiga_c::finalizeState_Ground() {}

void dAcOropeIgaiga_c::initializeState_Tree() {
    mAcceleration = 0.0f;
}

void dAcOropeIgaiga_c::executeState_Tree() {
    s32 tmp = fn_257_1D20();
    if (tmp == 1) {
        mStateMgr.changeState(StateID_Stick);
    } else if (tmp == 3 || SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag)) {
        mStateMgr.changeState(StateID_Ground);
        f32 random = cM::rndF(2.f);
        mGroundTimer = 0;
        mAcceleration = -(3.f + random);
    }
    fn_257_1DF0();
}

void dAcOropeIgaiga_c::finalizeState_Tree() {}

void dAcOropeIgaiga_c::initializeState_Stick() {
    mStts.SetRank(0);
    mStickTimer = 10;
    mVelocity.y = 17.f;
    mAcceleration = -3.f;
    if (dAcPy_c::GetLink()->getCenterTranslation().y > mPosition.y) {
        mLinkHigher = true;
    } else {
        mLinkHigher = false;
    }
    u32 random;
    if (mLinkHigher) {
        random = cM::rndInt(400) + 200;
    } else {
        random = cM::rndInt(600);
    }
    field_0x928 = random / 100;
    if (field_0x928 > 5) {
        field_0x928 = 5;
    }
    field_0x91A = 0;
    mCollider.ClrTgSet();
    f32 rnd = cM::rndF(10.f);
    mVelocity.y = rnd + 3.f;
    if (dAcPy_c::GetLink()->checkActionFlags(0x10000)) {
        s16 rotationYOffset = cM::rndInt(0x31C7) + 0x38e4;
        f32 f1 = (rotationYOffset - 0x38E4) / 12743.f;
        if (f1 > 1.f) {
            f1 = 1.f;
        } else if (f1 < 0.f) {
            f1 = 0.f;
        }
        f32 f2 = cM::rndF(3.f) + 15.f;
        mVecCylCalcFactor = 15.f * f1 + f2;
        if ((field_0x928 & 1) == 0) {
            rotationYOffset = -rotationYOffset;
        }
        mRotationYOffset = rotationYOffset;
    } else {
        if (field_0x928 >= 4) {
            mVecCylCalcFactor = cM::rndF(10.f);
        } else {
            mVecCylCalcFactor = cM::rndF(35.f);
        }
        s16 rotationYOffset = cM::rndInt(0x1555) + 0x2aab;
        if ((field_0x928 & 1) == 0) {
            rotationYOffset = -rotationYOffset;
        }
        if (field_0x928 >= 4) {
            rotationYOffset += cM::rndInt(0x1555) + 0x2aab;
        }
        mRotationYOffset = rotationYOffset;
    }
    mLinkPosDiff = mPosition - dAcPy_c::GetLink()->getCenterTranslation();
    if (dAcPy_c::GetLink()->getCurrentAction() == 0x25 /* (IDL_ON_VINES|NO_STAMINA) */) {
        mIdleOnVinesNoStamina = true;
    } else {
        mIdleOnVinesNoStamina = false;
    }
}

void dAcOropeIgaiga_c::executeState_Stick() {
    const dAcPy_c *link = dAcPy_c::GetLink();
    f32 posY = mPosition.y;
    if (fn_257_1FF0(&posY, false)) {
        mPositionRelated = mPosition;
        mPositionRelated.y = posY - 20.f;
        mStateMgr.changeState(StateID_Water);
    } else if (link->checkActionFlags(0x40000) || !link->checkFlags0x340(0x100)) {
        fn_257_1FF0(&posY, true);
        mPositionRelated = mPosition;
        mPositionRelated.y = posY - 20.f;
        mPosition.y = posY - 20.f;
        mStateMgr.changeState(StateID_Water);
        fn_257_21A0();
    } else if (fn_257_2310(mRotationYOffset)) {
        fn_257_1DF0();
        mVec3_c linkPos = dAcPy_c::GetLink()->getCenterTranslation();

        f32 linkPosDiffAbsXZ = mLinkPosDiff.absXZ();
        if (mIdleOnVinesNoStamina) {
            linkPosDiffAbsXZ *= 0.3f;
        }
        if (!dAcPy_c::GetLinkM()->isRoomID(mRoomID)) {
            mAngle.y = cM::rndInt(0x7fff);
            mSpeed = 13.f;
            mVelocity.y = 20.f;
            mStateMgr.changeState(StateID_Ground);
        } else {
            vecCylCalc(linkPos, dAcPy_c::GetLink()->mRotation.y + mRotationYOffset, linkPosDiffAbsXZ);
            linkPos.y += mLinkPosDiff.y;
            mRotation.y = cLib::targetAngleY(linkPos, dAcPy_c::GetLink()->mPosition);
            mPosition.x = linkPos.x;
            mPosition.z = linkPos.z;
            if (mVelocityYRelated > 0) {
                mAcceleration = -3.5f;
                mRotation.x += mRotationXOffset;
                calcVelocity();
                mPosition += mVelocity;
                if (mPosition.y < linkPos.y) {
                    mPosition.y = linkPos.y;
                    mVelocityYRelated--;
                    if (mVelocityYRelated != 0) {
                        mVelocity.y = 3.f * mLinkSpeedRelated * mVelocityYRelated;
                    }
                }
            } else {
                mPosition.y = linkPos.y;
                f32 linkSpeedRelated = dAcPy_c::GetLink()->mSpeed / 20.f;
                if (linkSpeedRelated > 1.f) {
                    linkSpeedRelated = 1.f;
                }
                if (dAcPy_c::GetLinkM()->checkActionFlags(0xc70852)) {
                    field_0x938 = true;
                } else if (field_0x938) {
                    field_0x938 = false;
                    linkSpeedRelated = 1.f;
                }
                mLinkSpeedRelated = linkSpeedRelated;
                if (linkSpeedRelated < 0.3f) {
                    mVelocityYRelated = 0;
                } else {
                    mVelocityYRelated = 3;
                    mLinkSpeedRelated += cM::rndFX(0.15f);
                }
                mRotationXOffset = mLinkSpeedRelated * 910.f;
            }
            if (fn_257_21D0()) {
                mAngle.y = cM::rndInt(0x7fff);
                mSpeed = 13.f;
                mVelocity.y = 20.f;
                mStateMgr.changeState(StateID_Ground);
            }
        }
    }
}

void dAcOropeIgaiga_c::finalizeState_Stick() {
    field_0x928 = 0xff;
    mCollider.OnTgSet();
    mCollider.OnCoSet();
}

void dAcOropeIgaiga_c::initializeState_Water() {
    mStts.SetRank(5);
    mRotationXOffset = 100;
    mPositionYRelated = mPositionRelated.y - 2.0f;
    fn_257_20F0(3.f, 10.f);
}

const s16 dAcOropeIgaiga_c::lbl_257_rodata_90 = 0x2d8;

void dAcOropeIgaiga_c::executeState_Water() {
    fn_257_1D20();
    fn_257_1DF0();
    if (mVelocityYRelated != 0) {
        calcVelocity();
        mPosition += mVelocity;
        mPosition += mStts.mCcMove;
    }

    if (mVelocityYRelated != 0) {
        if (fn_257_1FF0(nullptr, 0) != 0) {
            mVelocityYRelated--;
            if (mVelocityYRelated > 0) {
                sLib::chase(&mSpeed, 0, 1.f);
                mVelocity.y = 7.f * mVelocityYRelated;
            } else {
                fn_257_21A0();
            }
        }
    } else {
        f32 f1 = 0.1f;
        if (fn_257_1D20() == 1) {
            mSpeed = 8.5f;
            mAngle.y = cLib::targetAngleY(dAcPy_c::GetLink()->mPosition, mPosition);
        }
        sLib::chase(&mSpeed, 0, 1.f);

        if (!cM::isZero(mSpeed)) {
            calcVelocity();
            mPosition += mVelocity;
            mPosition += mStts.mCcMove;
            mRotation.x += mAng(0x2d8);
            f1 = 2.f;
        }
        if (sLib::chase(&mPosition.y, mPositionYRelated, f1) != 0) {
            if (mPositionYRelated > mPositionRelated.y) {
                mPositionYRelated = mPositionRelated.y - 2.f;
            } else {
                mPositionYRelated = mPositionRelated.y + 1.f;
            }
        }
    }
    if (mObjAcch.ChkGndHit()) {
        mStateMgr.changeState(StateID_Ground);
        mVelocityYRelated = 0;
    }
    mObjAcch.CrrPos(*dBgS::GetInstance());
}

void dAcOropeIgaiga_c::finalizeState_Water() {
    mVelocityYRelated = 0;
    mStts.SetRank(0);
    fn_257_21A0();
}

u32 dAcOropeIgaiga_c::fn_257_1CA0() {
    f32 distThresh1 = 62500.0f;
    f32 distThresh2 = 250000.0f;
    f32 squareMagXZ = dAcPy_c::GetLink()->mPosition.squareDistanceToXZ(mPosition);
    if (squareMagXZ < distThresh1) {
        return 2;
    }
    return squareMagXZ < distThresh2;
}

s32 dAcOropeIgaiga_c::fn_257_1D20() {
    if (fn_257_2720()) {
        fn_257_1FD0();
        return 1;
    }
    if (mCollider.ChkTgHit()) {
        fn_257_1FD0();
        if (mCollider.ChkTgAtHitType(2)) {
            startSound(SE_SW_HIT_RopeIga);
        }
        dAcObjBase_c *tgActor = mCollider.GetTgActor();
        if (tgActor != nullptr && tgActor->mProfileName == fProfile::OBJ_ROPE_IGAIGA) {
            return 2;
        }
        return 3;
    }
    return 0;
}

void dAcOropeIgaiga_c::fn_257_1DF0() {
    if (field_0x91E > 0) {
        field_0x91E--;
        if (field_0x91E == 3) {
            mCollider.OnAtSet();
            mCollider.SetR(50.f);
            mCollider.mCo.OffSPrm(1);
            mVec3_c pos = mPosition;
            pos.y += 20.f;
            dJEffManager_c::spawnEffect(
                PARTICLE_RESOURCE_ID_MAPPING_498_, pos, nullptr, nullptr, nullptr, nullptr, 0, 0
            );
            startSound(SE_RopeIga_EXPLODE);
        } else if (field_0x91E == 0) {
            deleteRequest();
        }
        if ((u16)(field_0x91E - 1) <= 1) {
            if (mCollider.ChkAtHit()) {
                deleteRequest();
            }
        }
    }
}

void dAcOropeIgaiga_c::fn_257_1F20() {
    s16 ang = mStateMgr.isState(StateID_Rope) ? 0xb6 : 0x4fa;
    s16 random = cM::rndInt(0x222);
    s32 ang2 = ang + random;
    if (ang2 & 1) {
        ang2 = (s16)-ang2;
    }
    mRotationXOffset = ang2;
}

void dAcOropeIgaiga_c::fn_257_1FD0() {
    if (field_0x91E != -1) {
        return;
    }
    field_0x91E = 0x99;
}

bool dAcOropeIgaiga_c::fn_257_1FF0(f32 *posY, bool p2) {
    if (p2 || mVelocity.y < 0.f) {
        mVec3_c _pos = mPosition;
        f32 f1 = p2 ? 200.f : 50.f;
        f32 f2 = p2 ? -200.f : -30.f;
        if (dBgS_WtrChk::CheckPos(&mPosition, true, f1, f2)) {
            f32 waterHeight = dBgS_WtrChk::GetWaterHeight();
            if (p2 || mPosition.y < waterHeight - 20.f) {
                if (posY != nullptr) {
                    *posY = waterHeight;
                }
                return true;
            }
        }
    }
    return false;
}

void dAcOropeIgaiga_c::fn_257_20F0(f32 speed, f32 velocityY) {
    fn_257_1F20();
    mVelocityYRelated = 3;
    mSpeed = speed;
    mAngle.y = cLib::targetAngleY(dAcPy_c::GetLink()->mPosition, mPosition);
    s16 angleOffset = cM::rndInt(0x2aab);
    if (angleOffset & 1) {
        angleOffset = -angleOffset;
    }
    mAngle.y += angleOffset;
    mVelocity.y = velocityY;
    mAcceleration = -4.f;
}

void dAcOropeIgaiga_c::fn_257_21A0() {
    mVelocityYRelated = 0;
    mVelocity.y = 0.f;
    mAcceleration = 0.f;
    mSpeed = 0.f;
    mRotationXOffset = 100;
}

bool dAcOropeIgaiga_c::fn_257_21D0() {
    if (field_0x91A != 0) {
        field_0x91A--;
    }
    dAcPy_c *link = dAcPy_c::GetLinkM();
    if (link->getCurrentAction() == 0x49 /* SHIELD_BASH */) {
        if (!mLinkShieldBash) {
            field_0x91A += lbl_257_rodata_1E;
        }
        mLinkShieldBash = true;
    } else {
        mLinkShieldBash = false;
        if (link->checkFlags0x354(0x2000)) {
            field_0x91A += lbl_257_rodata_14;
        } else if (link->checkFlags0x354(0x4)) {
            field_0x91A += lbl_257_rodata_16;
        } else if (link->getCurrentAction() == 0xC /* ROLL */) {
            field_0x91A += lbl_257_rodata_18;
        } else if (link->getCurrentAction() == 0x27 /* START_VINE_LEAP */) {
            field_0x91A += lbl_257_rodata_1A;
        } else if (link->checkSwordAndMoreStates(0x40) /* SPRINTING */) {
            field_0x91A += lbl_257_rodata_1C;
        }
    }
    if (mIdleOnVinesNoStamina) {
        if (dAcPy_c::GetLink()->getCurrentAction() != 0x25 /* (IDL_ON_VINES|NO_STAMINA) */) {
            field_0x91A += lbl_257_rodata_20;
        }
    } else if (dAcPy_c::GetLink()->getCurrentAction() == 0x25) {
        mIdleOnVinesNoStamina = true;
    }
    if ((s32)field_0x91A >= 100) {
        return true;
    }
    return false;
}

const f32 dAcOropeIgaiga_c::sFloats2[6] = {20.f, 30.f, -55.f, -65.f, -25.f, -35.f};

bool dAcOropeIgaiga_c::fn_257_2310(s16 rotationYOffset) {
    if (mStickTimer != 0) {
        mVec3_c translation = dAcPy_c::GetLink()->getCenterTranslation();
        vecCylCalc(translation, dAcPy_c::GetLink()->mRotation.y + rotationYOffset, mVecCylCalcFactor);
        translation.y += sFloats2[field_0x928];
        if (mStickTimer == 10 && field_0x928 < 4) {
            mVec3_c pos = dAcPy_c::GetLink()->mPosition;
            s16 ang1 = cLib::targetAngleY(pos, mPosition);
            s16 ang2 = cLib::targetAngleY(pos, translation);
            if (sLib::absDiff(ang1, ang2) > 0x471C) {
                if (field_0x928 & 1) {
                    field_0x928--;
                } else {
                    field_0x928++;
                }
                if (field_0x928 > 5) {
                    field_0x928 = 5;
                }
            }
        }
        mStickTimer--;
        mRotation.y = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);
        mRotation.x += mRotationXOffset;
        cLib::addCalcPosXZ(&mPosition, translation, 0.3f, 10.f, 1.f);
        calcVelocity();
        if (mLinkHigher) {
            if (mVelocity.y < -5.f) {
                mVelocity.y = 10.f;
            }
            if (mPosition.y > translation.y) {
                mVelocity.y = 0.f;
                if (mStickTimer > 10) {
                    mStickTimer = 0;
                }
            } else {
                mPosition.y += mVelocity.y;
            }
        } else {
            if (mVelocity.y < -15.f) {
                mVelocity.y = 0.f;
            }
            mPosition.y += mVelocity.y;
            if (mPosition.y < translation.y) {
                mVelocity.y = 10.f;
                if (mStickTimer > 10) {
                    mStickTimer = 0;
                }
            }
        }
        if (fn_257_2720()) {
            mLinkPosDiff = mPosition - dAcPy_c::GetLink()->getCenterTranslation();
        } else {
            if (dAcPy_c::GetLinkM()->checkActionFlags(0xc70852) == 0) {
                translation = (translation + mPosition) * 0.5f;
            }
            mLinkPosDiff = translation - dAcPy_c::GetLink()->getCenterTranslation();
            mStickTimer = 0;
        }
        return false;
    }
    mCollider.OffCoSet();
    return true;
}

bool dAcOropeIgaiga_c::fn_257_2720() {
    if (mCollider.ChkCoHit()) {
        if (mCollider.GetCoActor()->isPlayer() &&
            dAcPy_c::GetLink()->getCenterTranslation().squareDistanceToXZ(mPosition) < 1600.f) {
            return true;
        }
    }
    return false;
}

const f32 dAcOropeIgaiga_c::sSquareXZDistanceThreshold = 1600.f;
