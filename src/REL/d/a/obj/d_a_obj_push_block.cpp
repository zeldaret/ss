#include "d/a/obj/d_a_obj_push_block.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
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
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_sphere.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/math/math_arithmetic.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(OBJ_PUSH_BLOCK, dAcOpushBlk_c, fProfile::OBJ_PUSH_BLOCK, 0x12D, 0, 2);

STATE_DEFINE(dAcOpushBlk_c, Wait);
STATE_DEFINE(dAcOpushBlk_c, Move);
STATE_DEFINE(dAcOpushBlk_c, RopeGrab);

static dCcD_SrcCyl sSrcCyl = {
    {/* mObjAt */ {AT_TYPE_PHYSICS, 0x91, {0, 0, 0}, 0, 2, 0, 0, CUT_DIR_NONE, 0},
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
        mTentalusFight = 1;

        mAcceleration = -5.0f;
        mMaxSpeed = -80.0f;
        mMdlPresent.setTevColorAll(GX_TEVREG1, mColor(0x19, 0x19, 0x19, 0xFF), false);
    }

    for (int i = 0; i < 4; ++i) {
        mEmitter[i].init(this);
    }

    if (mRestrictSceneflag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mRestrictSceneflag)) {
        field_0xF02 = 2;
        mAcceleration = 0.0f;
    }

    field_0xF0F = false;

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

    if (field_0xF02 == 2) {
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
            field_0xF02 = 0;
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
    field_0xF05 = true;

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

    if (!(isState(StateID_RopeGrab) && mbOnRope) && field_0xF02 == 0) {
        if (field_0xF08) {
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
                field_0xF0C = true;
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
            if (!static_cast<const dAcOpushBlk_c *>(pObj)->field_0xF05) {
                updateExecutePriority((*fProfile::sProfileList)[fProfile::OBJ_PUSH_BLOCK]->m_execute_order + 1);
            }
        }
    }
    return SUCCEEDED;
}

int dAcOpushBlk_c::draw() {
    field_0xF05 = false;
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

    if (field_0xF0F) {
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
    field_0xF06 = false;
    if (mTentalusFight == 2) {
        mTentalusFight = 1;
    }
}
void dAcOpushBlk_c::executeState_Wait() {
    s32 _weird_zero = 0;

    if (mTentalusFight != 0) {
        if (!mStageKraken.isLinked()) {
            mStageKraken.link(dAcOstageKraken_c::getInstance());
        }
        if (mStageKraken.isLinked()) {
            if (field_0xEFA != 0) {
                field_0xEFA--;
            }

            switch (mTentalusFight) {
                case 1:
                    field_0xEE4.set(420.0f, 1540.0f, -570.0f);
                    mTentalusFight = 2;
                    // fall-through
                case 2: {
                    mRotation = mStageKraken.get()->mRotation;
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
                        field_0xEE4.z += 0.1f * _weird_zero + 22.0f;
                    }
                    mRotation.z = 1000.0f * mAng(field_0xEFA * 0xF00).sin();
                    if (mRotation.x >= 0x4000) {
                        mRotation.x = 0;
                        mTentalusFight = 6;
                        field_0xEE4.z += _weird_zero + 60.0f;
                        field_0xEE4.y += _weird_zero + 60.0f;
                    }
                } break;
                case 6: {
                    if (mAcch.ChkGndHit()) {
                        dJEffManager_c::spawnGroundEffect(
                            mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 0, 1.5f, field_0x1B0
                        );
                        mTentalusFight = 7;
                        mVelocity.y = 25.0f;
                        field_0xEFA = 15;
                    }
                } break;
                case 7: {
                    mRotation.x = field_0xEFA * 150.0f * mAng(field_0xEFA * 0x1C00).sin();
                    mRotation.z = field_0xEFA * 150.0f * mAng(field_0xEFA * 0x1C00).sin();
                    if (field_0xEFA == 0) {
                        mTentalusFight = 1;
                    }

                } break;
            }
            mVec3_c v0;
            mStageKraken.get()->mWorldMtx.multVec(field_0xEE4, v0);
            mPosition = v0;

            mMtx_c m0;
            m0.ZrotS(-mStageKraken.get()->mRotation.z);
            // ...
        }
    }
}
void dAcOpushBlk_c::finalizeState_Wait() {}

void dAcOpushBlk_c::initializeState_Move() {}
void dAcOpushBlk_c::executeState_Move() {}
void dAcOpushBlk_c::finalizeState_Move() {}

void dAcOpushBlk_c::initializeState_RopeGrab() {}
void dAcOpushBlk_c::executeState_RopeGrab() {}
void dAcOpushBlk_c::finalizeState_RopeGrab() {}
