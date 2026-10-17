#include "d/a/obj/d_a_obj_push_block.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_scattersand.h"
#include "d/a/obj/d_a_obj_stage_kraken.h"
#include "d/a/obj/d_a_obj_switch.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/bg/d_bg_w_base.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_sc_game.h"
#include "d/d_vec.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_sphere.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/math/math_arithmetic.h"
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(OBJ_PUSH_BLOCK, dAcOpushBlk_c, fProfile::OBJ_PUSH_BLOCK, 0x12D, 0, 2);

STATE_DEFINE(dAcOpushBlk_c, Wait);
STATE_DEFINE(dAcOpushBlk_c, Move);
STATE_DEFINE(dAcOpushBlk_c, RopeGrab);

struct dAcOpushBlk_HIO_c {
    f32 field_0x00;
    f32 field_0x04;
    f32 field_0x08;
    static const dAcOpushBlk_HIO_c sHio;
};
const dAcOpushBlk_HIO_c dAcOpushBlk_HIO_c::sHio = {
    10.0f,
    75.0f,
    150.0f,
};

static dCcD_SrcCyl sSrcCyl = {
    {/* mObjAt */ {AT_TYPE_PHYSICS, 0x91, {0, 0, 0}, 2, 0, 0, 0, CUT_DIR_NONE, 0},
     /* mObjTg */
     {0, 0, {0, 0, 0x407}, 0, CUT_DIR_NONE},
     /* mObjCo */ {0}},
    {75.0f, 100.0f}
};
static dCcD_SrcUnk sSrcBox = {
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, CUT_DIR_NONE, 0},
     /* mObjTg */
     {0, 0, {0, 0, 0x407}, 0, CUT_DIR_NONE},
     /* mObjCo */ {0x48C9}},
    {-75.0f, 0.0f, -75.0f, 75.0f, 140.0f, 75.0f},
};

dAcObjBase_c *dAcOpushBlk_c::PushPull_CallBack(dAcObjBase_c *pActor, dBgW_Base::PushPullLabel pushPull) {
    if (pushPull & (dBgW_Base::PPLABEL_PULL | dBgW_Base::PPLABEL_PUSH)) {
        if (pushPull & dBgW_Base::PPLABEL_PUSH) {
            mAngle.y = pActor->mRotation.y;
        } else {
            mAngle.y = pActor->mRotation.y + 0x8000;
        }

        if (pActor == dAcPy_c::GetLink()) {
            mbPlayerIsPushPull = true;
        }
    }

    return this;
}

static dAcObjBase_c *
dAcOpushBlk_c_PushPull_CallBack(dAcObjBase_c *i_this, dAcObjBase_c *actor, dBgW_Base::PushPullLabel pushPull) {
    return static_cast<dAcOpushBlk_c *>(i_this)->PushPull_CallBack(actor, pushPull);
}

bool dAcOpushBlk_c::createHeap() {
    static const char *sResNames[TYPE_MAX] = {"BoxIron", "BoxIronHang"};
    static const char *sMdlNames[TYPE_MAX] = {"BoxIronN", "BoxIronHang"};
    static const char *sDzbNames[TYPE_MAX] = {"dzb/BoxIron.dzb", "dzb/BoxIronHang.dzb"};
    static const char *sPlcNames[TYPE_MAX] = {"dat/BoxIron.plc", "dat/BoxIronHang.plc"};

    void *pRes = getOarcResFile(sResNames[mType]);
    if (pRes == nullptr) {
        return false;
    }
    mRes = nw4r::g3d::ResFile(pRes);

    TRY_CREATE(mMdlPresent.create(mRes.GetResMdl(sMdlNames[mType]), &mAllocator, 0x120));
    if (mType == TYPE_NORMAL) {
        TRY_CREATE(mMdlPast.create(mRes.GetResMdl("BoxIronT"), &mAllocator, 0x120));
    }

    cBgD_t *pDzb = static_cast<cBgD_t *>(getOarcFile(sResNames[mType], sDzbNames[mType]));
    PLC *pPlc = static_cast<PLC *>(getOarcFile(sResNames[mType], sPlcNames[mType]));
    updateMatrix();
    return !mBgW.Set(pDzb, pPlc, cBgW::MOVE_BG_e, &mWorldMtx, nullptr);
}

int dAcOpushBlk_c::actorCreate() {
    mType = getFromParams(0, 0xF);
    mGoalSceneflag = getFromParams(12, 0xFF);
    mRestrictSceneflag = getFromParams(20, 0xFF);
    mbGoalSwitch = getFromParams(28, 0x3);
    if (mbGoalSwitch >= 2) {
        mbGoalSwitch = 1;
    }
    mbLargerRadius = getFromParams(30, 0x3);
    if (mbLargerRadius >= 2) {
        mbLargerRadius = 0;
    }
    mSpawnPosition = mPosition;
    bool switchActive = SceneflagManager::sInstance->checkBoolFlag(mRoomID, mGoalSceneflag);
    if (switchActive || mbGoalSwitch == 0) {
        s32 pathIdx = getFromParams(4, 0xFF);
        if (pathIdx != 0xFF && mPath.init(pathIdx, mRoomID, 0, 0, false)) {
            mbOnPath = true;
        }
    }

    CREATE_ALLOCATOR(dAcOpushBlk_c);

    if (mType == TYPE_NORMAL) {
        mMdlPast.setPriorityDraw(0xF, 0);
        mMdlPresent.setPriorityDraw(0xB, 0);
    } else {
        mMdlPresent.setPriorityDraw(0x1C, 9);
    }

    dBgS::GetInstance()->Regist(&mBgW, this);
    mAcch.Set(&mPosition, &mOldPosition, this, 2, mAcchCir, &mVelocity, nullptr, nullptr);
    mAcchCir[0].SetWall(10.0f, 75.0f);
    mAcchCir[1].SetWall(140.0f, 75.0f);
    mStts.SetRank(0);

    mCcBox.Set(sSrcBox);
    mCcBox.SetStts(mStts);
    mCcBox.ClrCoSet();

    mCyl.Set(sSrcCyl);
    mCyl.SetStts(mStts);
    mCyl.ClrAtSet();

    mAcceleration = -4.0f;
    mMaxSpeed = -80.0f;

    if (dScGame_c::isCurrentStage("B301")) {
        mTentalusFightState = 1;

        mAcceleration = -5.0f;
        mMaxSpeed = -80.0f;
        mMdlPresent.setTevColorAll(GX_TEVREG1, mColor(0x19, 0x19, 0x19, 0xFF), false);
    }

    for (int i = 0; i < 4; ++i) {
        mEmitter[i].init(this);
    }

    if (mRestrictSceneflag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mRestrictSceneflag)) {
        mRestrictType = 2;
        mAcceleration = 0.0f;
    }

    mbDrawShadow = false;

    if (mType == TYPE_HANG && !switchActive) {
        mAcch.CrrPos(*dBgS::GetInstance());
        mAcceleration = 0.0f;
        changeState(StateID_RopeGrab);
    } else {
        changeState(StateID_Wait);
    }

    mBoundingBox.Set(mVec3_c(-100.0f, -50.0f, -100.0f), mVec3_c(100.0f, 200.0f, 100.0f));
    mBgW.SetPushPullCallback(dAcOpushBlk_c_PushPull_CallBack);
    mBgW.SetCrrFunc(dBgS_MoveBGProc_Typical);
    mBgW.OnFlag0x80();
    mBgW.OnFlag0x08();
    return SUCCEEDED;
}

int dAcOpushBlk_c::actorPostCreate() {
    if (mbOnPath && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mGoalSceneflag) &&
        mPath.getPath().getPoint(0) != nullptr) {
        mVec3_c pnt;
        pnt.copyFrom(mPath.getPath().getPoint(0));

        dAcOsw_c *pSw = getSwitch(pnt);
        if (pSw != nullptr) {
            mSw.link(pSw);
            if (mSw.isLinked()) {
                mSw.get()->addBox();
            }
        }

        mOldPosition = mPosition = pnt;

        updateMatrix();
        mMdlPresent.setLocalMtx(mWorldMtx);
        if (mType == TYPE_NORMAL) {
            mMdlPast.setLocalMtx(mWorldMtx);
        }
        mBgW.Move();
    }

    if (mRestrictType == 2) {
        dAcObjBase_c *parent = nullptr;
        do {
            dAcObjBase_c *pVsd =
                static_cast<dAcObjBase_c *>(fManager_c::searchBaseByProfName(fProfile::OBJ_VSD, parent));
            parent = pVsd;
            if (pVsd != nullptr && nw4r::math::FAbs(pVsd->mPosition.y - mPosition.y) < 30.0f) {
                if (mPosition.squareDistanceToXZ(pVsd->mPosition) < 900.0f) {
                    mScatterSand.link(static_cast<dAcOScatterSand *>(pVsd));
                    break;
                }
            }
        } while (parent != nullptr);

        if (parent == nullptr) {
            mRestrictType = 0;
        }
    }

    return SUCCEEDED;
}

int dAcOpushBlk_c::doDelete() {
    if (mSw.isLinked()) {
        mSw.get()->removeBox();
        mSw.unlink();
    }
    return SUCCEEDED;
}

int dAcOpushBlk_c::actorExecute() {
    mbExecuteThisFrame = true;

    if (mManager.getExecuteOrder() != fProfile::OBJ_PUSH_BLOCK) {
        updateExecutePriority((*fProfile::sProfileList)[fProfile::OBJ_PUSH_BLOCK]->m_execute_order);
    }

    mCyl.ClrAtSet();
    if (mType == TYPE_NORMAL) {
        mTime.update(mRoomID, mPosition, 150.0f);
    }
    executeState();
    calcVelocity();

    mPosition += mVelocity;
    mPosition += mStts.GetCcMove();

    if (!(isState(StateID_RopeGrab) && mbOnRope) && mRestrictType == 0) {
        if (mbOnlyChangeHeight) {
            mVec3_c pos = mPosition;
            mAcch.CrrPos(*dBgS::GetInstance());
            mPosition.x = pos.x;
            mPosition.z = pos.z;
        } else {
            mAcch.CrrPos(*dBgS::GetInstance());
        }
    }

    if (isState(StateID_RopeGrab)) {
        mVec3_c start = mPosition;
        mVec3_c end = mPosition;
        f32 where = 2.0f + 75.0f * mRotation.x.sin();
        end.y -= where;
        if (dBgS_ObjLinChk::LineCross(&start, &end, this)) {
            where = where - (start.y - dBgS_ObjLinChk::GetInstance().GetLinEnd().y);
            if (where > 0.0f) {
                mbIsFalling = true;
                mPosition.y += where;
            }
        }
    }

    calcFall();

    mVec3_c center = mPosition;
    getXZCirclePoint(center, mAngle.y, dAcPy_c::GetLink()->sPushRelatedConstant);
    mCcBox.Set(center, mAngle.y);
    dCcS::GetInstance()->Set(&mCcBox);

    center.set(mPosition.x, mPosition.y - 20.0f, mPosition.z);
    mCyl.SetC(center);
    dCcS::GetInstance()->Set(&mCyl);

    if (isState(StateID_RopeGrab)) {
        if (!mbOnRope) {
            updateMatrix();
        }
    } else {
        updateMatrix();
    }

    mMdlPresent.setLocalMtx(mWorldMtx);
    if (mType == TYPE_NORMAL) {
        mMdlPast.setLocalMtx(mWorldMtx);
    }

    mBgW.Move();

    if (dBgS::GetInstance()->ChkMoveBG(mAcch.mGnd, true)) {
        const dAcObjBase_c *pObj = dBgS::GetInstance()->GetActorPointer(mAcch.mGnd);
        if (pObj != nullptr && pObj->mProfileName == fProfile::OBJ_PUSH_BLOCK) {
            if (!static_cast<const dAcOpushBlk_c *>(pObj)->mbExecuteThisFrame) {
                updateExecutePriority((*fProfile::sProfileList)[fProfile::OBJ_PUSH_BLOCK]->m_execute_order + 1);
            }
        }
    }
    return SUCCEEDED;
}

int dAcOpushBlk_c::draw() {
    mbExecuteThisFrame = false;
    if (mType == TYPE_NORMAL) {
        if (mTime.isPast()) {
            drawModelType1(&mMdlPast);
        }
        if (mTime.isPresent()) {
            drawModelType1(&mMdlPresent);
        }
    } else {
        drawModelType1(&mMdlPresent);
    }

    if (mbDrawShadow) {
        static mSphere_c sSph(mVec3_c(0.0f, 75.0f, 0.0f), 175.0f);

        f32 f = mPosition.y - mAcch.GetGroundH();
        if (mType != TYPE_HANG) {
            f = nw4r::ut::Max<f32>(f, 200.0f);
        }

        drawShadow(mShadow, nullptr, mWorldMtx, &sSph, -1, -1, -1, -1, -1, f);
    }

    return SUCCEEDED;
}

void dAcOpushBlk_c::initializeState_Wait() {
    mbCheckedMoveable = false;
    if (mTentalusFightState == 2) {
        mTentalusFightState = 1;
    }
}
void dAcOpushBlk_c::executeState_Wait() {
    s32 _weird_zero = 0;

    if (mTentalusFightState != 0) {
        if (!mStageKraken.isLinked()) {
            mStageKraken.link(dAcOstageKraken_c::getInstance());
        }
        dAcOstageKraken_c *pKraken = mStageKraken.get();
        if (mStageKraken.isLinked()) {
            if (field_0xEFA != 0) {
                field_0xEFA--;
            }

            switch (mTentalusFightState) {
                case 1:
                    mTentalusFightPosition.set(420.0f, 1540.0f, -570.0f);
                    mTentalusFightState = 2;
                    // fall-through
                case 2: {
                    mRotation.set(pKraken->mRotation);
                    if (mPosition.y < -1000.0f) {
                        mPosition.y = -1000.0f;
                        mVelocity.y = 0.0f;
                    }
                } break;
                case 5: {
                    if (field_0xEFA < 15) {
                        mRotation.x = mRotation.x + 1500;
                    } else {
                        mVelocity.y = 0.0f;
                        mTentalusFightPosition.z += 22.0f + 0.1f * _weird_zero;
                    }
                    mRotation.z = 1000.0f * mAng(field_0xEFA * 0xF00).sin();
                    if (mRotation.x >= 0x4000) {
                        mRotation.x = 0;
                        mTentalusFightPosition.z += _weird_zero + 60.0f;
                        mPosition.y -= _weird_zero + 60.0f;
                        mTentalusFightState = 6;
                    }
                } break;
                case 6: {
                    if (mAcch.ChkGndHit()) {
                        dJEffManager_c::spawnGroundEffect(
                            mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 0, 1.5f, field_0x1B0
                        );
                        mTentalusFightState = 7;
                        mVelocity.y = 25.0f;
                        field_0xEFA = 15;
                    }
                } break;
                case 7: {
                    mRotation.x = field_0xEFA * mAng(field_0xEFA * 0x1C00).sin() * 150.0f;
                    mRotation.z = field_0xEFA * mAng(field_0xEFA * 0x1900).sin() * 150.0f;
                    if (field_0xEFA == 0) {
                        mTentalusFightState = 1;
                    }

                } break;
            }
            mMtx_c m0;
            mVec3_c v0;
            pKraken->mWorldMtx.multVec(mTentalusFightPosition, v0);
            mPosition.x = v0.x;
            mPosition.z = v0.z;

            v0 = mPosition - pKraken->mPosition;

            m0.ZrotS(-pKraken->mRotation.z);
            m0.XrotM(-pKraken->mRotation.x);
            m0.YrotM(-pKraken->mRotation.y);
            m0.multVec(v0, v0);

            mTentalusFightPosition.y = v0.y;
            if (dAcPy_c::GetLink()->isRecovering()) {
                if (!field_0xF10) {
                    field_0xF10 = true;
                    fBase_c *pObj = nullptr;
                    do {
                        pObj = fManager_c::searchBaseByProfName(fProfile::B_KR, pObj);
                        if (pObj != nullptr) {
                            mTentalusFightState = 1;
                            mPosition.set(mStartingPos);
                            mOldPosition.set(mStartingPos);
                            mRotation.set(mStartingRot);
                            break;
                        }
                    } while (pObj != nullptr);
                }
            } else {
                field_0xF10 = false;
            }
        }
    }
    checkMoveable();
    recheckPushblockAround();
    if (mRestrictType == 1) {
        if (!mScatterSand.isLinked()) {
            mRestrictType = 0;
            mAcceleration = -4.0f;
        }
    } else if (mRestrictType == 2) {
        dAcOScatterSand *pSand = mScatterSand.get();
        if (pSand != nullptr) {
            f32 maxScaleXZ = MAX(pSand->mScale.x, pSand->mScale.z);
            if (maxScaleXZ * 50.0f <= 75.0f && !pSand->getField_0x6E4()) {
                pSand->setField_0x6E4(true);
                pSand->setField_0x6DC(10);
            }
        }

        if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mRestrictSceneflag) || pSand == nullptr) {
            if (pSand != nullptr && !pSand->getField_0x6E4()) {
                pSand->setField_0x6E4(true);
                pSand->setField_0x6DC(10);
            }
            mRestrictType = 1;
        } else {
            mBgW.ClrFlag0x4();
        }
    } else {
        updateMoveBg();
    }

    if (mbPlayerIsPushPull) {
        mbPlayerIsPushPull = false;
        if (!mbCoOn) {
            mbCoOn = true;
            mCcBox.OnCoSet();
        } else if (!mCcBox.ChkCoHit() && checkCanTryMove(mAngle.y, true)) {
            mPreMovePosition = mPosition;
            getXZCirclePoint(mPreMovePosition, mAngle.y, dAcPy_c::sPushRelatedConstant);
            if (checkNotHittingNewSwitch()) {
                changeState(StateID_Move);
            }
        }
    } else {
        mbCoOn = false;
        mCcBox.ClrCoSet();
    }
}
void dAcOpushBlk_c::finalizeState_Wait() {
    mbCoOn = false;
    mCcBox.ClrCoSet();
}

void dAcOpushBlk_c::initializeState_Move() {
    dAcPy_c::GetLinkM()->onFlags_0x360(0x10000);
    mMoveTimer = s16(dAcPy_c::GetLink()->sPushRelatedConstant * 0.1f) + 1;
    if (dAcPy_c::GetLink()->mRotation.y.absDiff(mAngle.y) > 0x4000) {
        fn_217_2310();
    }
}

void dAcOpushBlk_c::executeState_Move() {
    static const s16 sAngs[4] = {0x2000, 0x6000, -0x6000, -0x2000};
    holdSound(SE_PushBlk_MOVE_LV);
    f32 radius = 75.0f;
    for (int i = 0; i < 4; ++i) {
        mVec3_c spawnPos = mPosition;
        getXZCirclePoint(spawnPos, sAngs[i] + mRotation.y, radius * 1.4142135f);
        mEmitter[i].holdEffect(PARTICLE_RESOURCE_ID_MAPPING_514_, spawnPos, nullptr, nullptr, nullptr, nullptr);
    }

    if (cLib::chasePosXZ(mPosition, mPreMovePosition, 10.0f)) {
        updatePathGoalFlag();
        if (mbAtLimit) {
            startSound(SE_PushBlk_MOVE_LIMIT);
        }
        changeState(StateID_Wait);
        return;
    }

    if (!mSw.isLinked()) {
        if (mAcchCir[0].ChkWallHit()) {
            s32 target = cLib::targetAngleY(mPreMovePosition, mPosition);
            s32 wallAngle = mAcchCir[0].GetWallAngleY();
            if (target == wallAngle) {
                updatePathGoalFlag();
                startSound(SE_PushBlk_MOVE_LIMIT);
                changeState(StateID_Wait);
            }
        }
    }
    if (mAcchCir[1].ChkWallHit()) {
        s32 target = cLib::targetAngleY(mPreMovePosition, mPosition);
        s32 wallAngle = mAcchCir[1].GetWallAngleY();
        if (target == wallAngle) {
            updatePathGoalFlag();
            changeState(StateID_Wait);
        }
    }

    if (mMoveTimer != 0) {
        mMoveTimer--;
    } else {
        updatePathGoalFlag();
        changeState(StateID_Wait);
    }
}
void dAcOpushBlk_c::finalizeState_Move() {
    dAcPy_c::GetLinkM()->offFlags_0x360(0x10000);
    if (dAcPy_c::GetLink()->mRotation.y.absDiff(mAngle.y) < 0x4000) {
        mbPendingBlockCheck = true;
    }
    mbOnlyChangeHeight = false;
}

void dAcOpushBlk_c::initializeState_RopeGrab() {
    mbDrawShadow = true;
    mAcceleration = 0.0f;
    mLandSequenceState = 3;
}
void dAcOpushBlk_c::executeState_RopeGrab() {
    if (mbOnRope) {
        return;
    }

    mAcceleration = -4.0f;
    cLib::chasePosXZ(mPosition, mSpawnPosition, 5.0f);
    if (mLandSequenceState < 3) {
        sLib::addCalcAngle(mRotation.x.ref(), 0, 5, 1820, 182);
        sLib::addCalcAngle(mRotation.z.ref(), 0, 5, 1820, 182);
    } else {
        sLib::chaseAngle(mRotation.x.ref(), 3641, 350);
        sLib::addCalcAngle(mRotation.z.ref(), 0, 5, 0x71c, 182);
    }

    if (mAcch.ChkGndHit() || (mLandSequenceState == 3 && mbIsFalling)) {
        if (mLandSequenceState == 3) {
            startSound(SE_PushBlk_LAND);
        }
        if (mLandSequenceState == 2) {
            dJEffManager_c::spawnGroundEffect(mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 0, 1.5f, field_0x1B0);
        }

        if (mLandSequenceState == 0) {
            changeState(StateID_Wait);
        } else {
            mVelocity.y = mLandSequenceState * 4.0f;
            mLandSequenceState--;
        }
    }
}
void dAcOpushBlk_c::finalizeState_RopeGrab() {
    mPosition.x = mSpawnPosition.x;
    mPosition.z = mSpawnPosition.z;
    mRotation.x = 0;
    mRotation.z = 0;
    mbDrawShadow = false;
}

bool dAcOpushBlk_c::checkCanTryMove(s16 dir, bool set) {
    static const s16 sAngs[3] = {0x4000, 0x0, -0x4000};
    mbAtLimit = false;

    f32 f8 = 8.0f;
    dAcOsw_c *pSw = mSw.get();
    if (pSw && pSw->mProfileName == fProfile::OBJ_SW) {
        f32 f2 = pSw->mPosition.y;
        if (f8 - (mPosition.y - f2) < f8) {
            f8 = 8.0f;
        }
    }

    mVec3_c start = mPosition;
    mVec3_c end = mPosition;
    for (int i = 0; i < 3; ++i) {
        start = mPosition;
        f32 radius = 73.0f;
        getXZCirclePoint(start, dir, radius);
        if (i != 1) {
            getXZCirclePoint(start, dir + sAngs[i], radius);
        }

        start.y += f8;
        end = start;

        getXZCirclePoint(end, dir, dAcPy_c::GetLink()->sPushRelatedConstant + 2.0f);

        if (dBgS_ObjLinChk::LineCross(&start, &end, this)) {
            f32 a = (dBgS_ObjLinChk::GetInstance().GetLinEnd() - start).absXZ();
            f32 b = (end - start).absXZ();
            if (b - a > 1.5f) {
                bool bHitSwitch = false;
                if (dBgS::GetInstance()->ChkMoveBG(dBgS_ObjLinChk::GetInstance(), false)) {
                    const dAcObjBase_c *pObj = dBgS::GetInstance()->GetActorPointer(dBgS_ObjLinChk::GetInstance());
                    if (pObj != nullptr && pObj->mProfileName == fProfile::OBJ_SW) {
                        bHitSwitch = true;
                    }
                }

                if (!bHitSwitch) {
                    return false;
                }

            } else {
                if (set) {
                    mbAtLimit = true;
                }
            }
        }
    }
    return true;
}

void dAcOpushBlk_c::recheckPushblockInDir(s16 rot) {
    dAcOpushBlk_c *pObj = checkBlockedByPushBlock(rot);
    if (pObj != nullptr) {
        pObj->mbCheckedMoveable = false;
    }
}

void dAcOpushBlk_c::fn_217_2310() {
    dAcOpushBlk_c *pObj = checkBlockedByPushBlock(mAngle.y - 0x8000);
    if (pObj != nullptr) {
        pObj->mBgW.OnFlag0x04();
        pObj->mbCanMove = true;
    }
}

void dAcOpushBlk_c::recheckPushblockAround() {
    if (mbPendingBlockCheck && mAcch.ChkGndHit()) {
        mbPendingBlockCheck = false;
        recheckPushblockInDir(mRotation.y);
        recheckPushblockInDir(mRotation.y - 0x4000);
        recheckPushblockInDir(mRotation.y + 0x4000);
        recheckPushblockInDir(mRotation.y - 0x8000);
    }
}

void dAcOpushBlk_c::checkMoveable() {
    if (!mbCheckedMoveable && mAcch.ChkGndHit()) {
        mbCheckedMoveable = true;

        if (!(checkCanTryMove(mRotation.y, false) || checkCanTryMove(mRotation.y - 0x4000, false) ||
              checkCanTryMove(mRotation.y + 0x4000, false) || checkCanTryMove(mRotation.y - 0x8000, false))) {
            mBgW.ClrFlag0x4();
            mbPlayerIsPushPull = false;
            mbCanMove = false;
        } else {
            mBgW.OnFlag0x04();
            mbCanMove = true;
        }
    }
}
void dAcOpushBlk_c::updateMoveBg() {
    if (!mbCanMove) {
        return;
    }
    if (nw4r::math::FAbs(dAcPy_c::GetLink()->mPosition.y - mPosition.y) > 15.0f) {
        mBgW.ClrFlag0x4();
    } else {
        mBgW.OnFlag0x04();
    }
}
dAcOpushBlk_c *dAcOpushBlk_c::checkBlockedByPushBlock(s16 rot) {
    if (!checkCanTryMove(rot, false)) {
        if (dBgS::GetInstance()->ChkMoveBG(dBgS_ObjLinChk::GetInstance(), false)) {
            const dAcObjBase_c *pObj = dBgS::GetInstance()->GetActorPointer(dBgS_ObjLinChk::GetInstance());
            if (pObj != nullptr && pObj->mProfileName == fProfile::OBJ_PUSH_BLOCK) {
                return (dAcOpushBlk_c *)(pObj); // Removes const qualifier
            }
        }
    }
    return nullptr;
}

dAcOsw_c *dAcOpushBlk_c::getSwitch(const mVec3_c &pos) {
    mVec3_c start, end;

    start = pos;
    end = pos;

    start.y += 30.0f;
    end.y -= 5.0f;

    if (dBgS_ObjLinChk::LineCross(&start, &end, this)) {
        if (dBgS::GetInstance()->ChkMoveBG(dBgS_ObjLinChk::GetInstance(), false)) {
            const dAcObjBase_c *pObj = dBgS::GetInstance()->GetActorPointer(dBgS_ObjLinChk::GetInstance());
            if (pObj != nullptr && pObj->mProfileName == fProfile::OBJ_SW) {
                return (dAcOsw_c *)(pObj); // Removes const qualifier
            }
        }
    }
    return nullptr;
}

bool dAcOpushBlk_c::isSwitchInArea(const mVec3_c &pos) {
    static const s16 sAngs[4] = {0x2000, 0x6000, -0x6000, -0x2000};

    mVec3_c center = pos;
    center.y -= 10.0f;

    dAcOsw_c *pSw = mSw.get();
    if (pSw == nullptr) {
        return true;
    }

    mVec3_c extent(150.0f, 30.0f, 150.0f);
    mArea.set(center, mRotation.y, extent, nullptr, 0.0f);
    f32 radius = (pSw->getField_0x5E8() - 10.f) * 1.4142135f * 0.5f;
    for (int i = 0; i < 4; ++i) {
        mVec3_c checkPos = pSw->mPosition;
        getXZCirclePoint(checkPos, sAngs[i] + pSw->mRotation.y, radius);

        if (mArea.checkAreaBox(checkPos)) {
            return true;
        }
    }

    return false;
}

bool dAcOpushBlk_c::checkNotHittingNewSwitch() {
    mVec3_c pos = mPreMovePosition;
    f32 radius = 75.0f;
    radius -= 20.0f;
    getXZCirclePoint(pos, mAngle.y, radius);

    if (mSw.isLinked() && !isSwitchInArea(pos)) {
        mSw.get()->removeBox();
        mSw.unlink();
    }

    mVec3_c checkPos = pos;
    getXZCirclePoint(checkPos, mAngle.y + 0x4000, 15.0f);

    dAcOsw_c *pSw = getSwitch(checkPos);
    if (pSw == nullptr) {
        mVec3_c checkPos = pos;
        getXZCirclePoint(checkPos, mAngle.y - 0x4000, 15.0f);
        pSw = getSwitch(checkPos);
    }

    if (pSw != nullptr) {
        if (!mSw.isLinked()) {
            mSw.link(pSw);
            if (mSw.isLinked()) {
                mSw.get()->addBox();
                mbOnlyChangeHeight = true;
            }
        } else if (mSw.get() != pSw) {
            return false;
        }
    }
    return true;
}

void dAcOpushBlk_c::updatePathGoalFlag() {
    if (!mbGoalSwitch && mbOnPath) {
        const Vec *pPnt = mPath.getPath().getPoint(0);
        if (pPnt != nullptr) {
            f32 rad = !mbLargerRadius ? 100.0f : 6400.0f;
            mVec3_c distToPnt(mPosition.x - pPnt->x, mPosition.y - pPnt->y, mPosition.z - pPnt->z);
            if (distToPnt.squareMagXZ() < rad) {
                SceneflagManager::sInstance->setFlag(mRoomID, mGoalSceneflag);
                return;
            }
        }
        SceneflagManager::sInstance->unsetFlag(mRoomID, mGoalSceneflag);
    }
}

void dAcOpushBlk_c::calcFall() {
    if (isState(StateID_Wait)) {
        if (mFallDistance <= -300.0f && mAcch.ChkGndHit()) {
            startSound(SE_PushBlk_LAND);
            mCyl.OnAtSet();
            mFallDistance = 0.0f;
        }

        f32 heightIncrement = mPosition.y - mOldPosition.y;
        if (cM::isZero(heightIncrement)) {
            mFallDistance = 0.0f;
        } else {
            mFallDistance += heightIncrement;
            if (mFallDistance < -300.0f) {
                mFallDistance = -300.0f;
            }
        }
    }
}
