#include "d/a/obj/d_a_obj_kibako.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_scattersand.h"
#include "d/a/obj/d_a_obj_switch.h"
#include "d/col/bg/d_bg_plc.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_vec.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/math/math_arithmetic.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/event.h"

SPECIAL_ACTOR_PROFILE(OBJ_KIBAKO, dAcOkibako_c, fProfile::OBJ_KIBAKO, 0x12C, 0, 2);

STATE_DEFINE(dAcOkibako_c, Wait);
STATE_DEFINE(dAcOkibako_c, Fire);
STATE_DEFINE(dAcOkibako_c, PushPull);
STATE_DEFINE(dAcOkibako_c, RopeGrab);
STATE_DEFINE(dAcOkibako_c, DeleteEvent);

struct dAcOkibako_HIO_c {
    f32 field_0x00;
    f32 field_0x04;
    f32 field_0x08;
    static const dAcOkibako_HIO_c sHio;
};
const dAcOkibako_HIO_c dAcOkibako_HIO_c::sHio = {
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

dAcObjBase_c *dAcOkibako_c::PushPull_CallBack(dAcObjBase_c *pActor, dBgW_Base::PushPullLabel pushPull) {
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
dAcOkibako_c_PushPull_CallBack(dAcObjBase_c *i_this, dAcObjBase_c *actor, dBgW_Base::PushPullLabel pushPull) {
    return static_cast<dAcOkibako_c *>(i_this)->PushPull_CallBack(actor, pushPull);
}

bool dAcOkibako_c::createHeap() {
    static const char *sResName = "KibakoHang";
    static const char *sMdlName = "KibakoHang";
    static const char *sDzbName = "dzb/KibakoHang.dzb";
    static const char *sPlcName = "dat/KibakoHang.plc";

    void *pRes = getOarcResFile(sResName);
    if (pRes == nullptr) {
        return false;
    }
    nw4r::g3d::ResFile res(pRes);

    nw4r::g3d::ResMdl resMdl = res.GetResMdl(sMdlName);
    TRY_CREATE(mMdl.create(resMdl, &mAllocator, 0x120));

    mMdl.setPriorityDraw(0x1C, 9);
    cBgD_t *pDzb = static_cast<cBgD_t *>(getOarcFile(sResName, sDzbName));
    PLC *pPlc = static_cast<PLC *>(getOarcFile(sResName, sPlcName));

    updateMatrix();
    return !mBgW.Set(pDzb, pPlc, cBgW::MOVE_BG_e, &mWorldMtx, nullptr);
}

int dAcOkibako_c::actorCreate() {
    mType = getFromParams(0, 0xF);
    mRestrictSceneflag = getFromParams(4, 0xFF);
    mDestroySceneflag = getFromParams(12, 0xFF);
    mEventID = getFromParams(20, 0xFF);

    mPathID = getParams2Lower();
    if (mPathID == 0xFF) {
        if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mDestroySceneflag)) {
            return FAILED;
        }
    } else if (mPath.init(mPathID, mRoomID, 0, 0, false)) {
        mbOnPath = true;
    }

    CREATE_ALLOCATOR(dAcOkibako_c);

    dBgS::GetInstance()->Regist(&mBgW, this);
    mAcch.Set(&mPosition, &mOldPosition, this, 1, &mAcchCir, &mVelocity, &mAngle, &mRotation);
    mAcchCir.SetWall(10.0f, 75.0f);
    mBgW.SetPushPullCallback(dAcOkibako_c_PushPull_CallBack);
    mBgW.SetCrrFunc(dBgS_MoveBGProc_Typical);
    mBgW.OnFlag0x80();
    mBgW.OnFlag0x08();
    mStts.SetRank(0);

    mCyl.Set(sSrcCyl);
    mCyl.SetStts(mStts);

    mCcBox.Set(sSrcBox);
    mCcBox.SetStts(mStts);
    mCcBox.ClrCoSet();

    mSpawnPosition = mPosition;
    mGravity = -4.0f;
    mMaxFallSpeed = -80.0f;

    mGravityCheckTimer = -(5 + cM::rndInt(10));

    for (int i = 0; i < 4; ++i) {
        mEmitters[i].init(this);
    }

    if (mRestrictSceneflag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mRestrictSceneflag)) {
        mRestrictType = 2;
        mGravity = 0.0f;
    }

    mbOnRope = false;

    if (mType == TYPE_HANG) {
        mAcch.CrrPos(*dBgS::GetInstance());
        mGravity = 0.0f;
        mGravityCheckTimer = 0;
        changeState(StateID_RopeGrab);
        mBoundingBox.Set(mVec3_c(-750.0f, -800.0f, -250.0f), mVec3_c(100.0f, 200.0f, 1000.0f));
    } else {
        changeState(StateID_RopeGrab);
        mBoundingBox.Set(mVec3_c(-250.0f, -100.0f, -250.0f), mVec3_c(250.0f, 200.0f, 250.0f));
    }

    return SUCCEEDED;
}

int dAcOkibako_c::actorPostCreate() {
    static const f32 sandDistanceThreshold = 900.0f;
    static const f32 sandHeightThreshhold = 30.0f;
    if (mbOnPath && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mDestroySceneflag) &&
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
        mMdl.setLocalMtx(mWorldMtx);
        mBgW.Move();
    }

    if (mRestrictType == 2) {
        dAcObjBase_c *parent = nullptr;
        do {
            dAcObjBase_c *pVsd =
                static_cast<dAcObjBase_c *>(fManager_c::searchBaseByProfName(fProfile::OBJ_VSD, parent));
            parent = pVsd;
            if (pVsd != nullptr && nw4r::math::FAbs(pVsd->mPosition.y - mPosition.y) < sandHeightThreshhold) {
                if (mPosition.squareDistanceToXZ(pVsd->mPosition) < sandDistanceThreshold) {
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

int dAcOkibako_c::doDelete() {
    if (mSw.isLinked()) {
        mSw.get()->removeBox();
        mSw.unlink();
    }
    return SUCCEEDED;
}

int dAcOkibako_c::actorExecuteInEvent() {
    if (mEventRelated.isThisActorInEvent()) {
        if (mEventRelated.getCurrentEventCommand() == 'jmap') {
            mEventID = 0xFF;
            mEventRelated.advanceNext();
        }
    }
    return actorExecute();
}

int dAcOkibako_c::actorExecute() {
    if (checkHitBomb()) {
        mbHitByBomb = true;
    }

    mbExecuteThisFrame = true;
    if (mManager.getExecuteOrder() != fProfile::OBJ_KIBAKO) {
        updateExecutePriority((*fProfile::sProfileList)[fProfile::OBJ_KIBAKO]->m_execute_order);
    }

    executeState();
    calcVelocity();

    mPosition += mVelocity;
    mPosition += mStts.GetCcMove();

    bool isWait = isState(StateID_Wait);

    if ((!isState(StateID_RopeGrab) || !mbOnRope) && mRestrictType == 0) {
        updateGravity(isWait);
    }

    calcFall();
    checkBreakHeight();

    if (!mbHitByBomb) {
        mCyl.SetC(mPosition);
        dCcS::GetInstance()->Set(&mCyl);
    }

    mVec3_c pos = mPosition;
    getXZCirclePoint(pos, mAngle.y, dAcPy_c::GetLink()->sPushRelatedConstant);
    mCcBox.Set(pos, mAngle.y);
    dCcS::GetInstance()->Set(&mCcBox);

    if (isState(StateID_RopeGrab)) {
        if (!mbOnRope) {
            updateMatrix();
        }
    } else {
        updateMatrix();
    }

    mMdl.setLocalMtx(mWorldMtx);

    if (!isWait || mPosition.y != mOldPosition.y) {
        mMoveCooldown = 10;
    }

    if (mMoveCooldown != 0) {
        mBgW.Move();
        mMoveCooldown--;
    }

    if (dBgS::GetInstance()->ChkMoveBG(mAcch.mGnd, true)) {
        const dAcObjBase_c *pObj = dBgS::GetInstance()->GetActorPointer(mAcch.mGnd);
        if (pObj != nullptr && pObj->mProfileName == fProfile::OBJ_KIBAKO) {
            if (!static_cast<const dAcOkibako_c *>(pObj)->mbExecuteThisFrame) {
                updateExecutePriority((*fProfile::sProfileList)[fProfile::OBJ_KIBAKO]->m_execute_order + 1);
            }
        }
    }
    return SUCCEEDED;
}

int dAcOkibako_c::draw() {
    mbExecuteThisFrame = false;

    if (isState(StateID_DeleteEvent)) {
        return SUCCEEDED;
    }

    drawModel(&mMdl);
    if (mbDrawShadow) {
        static mSphere_c sSph(mVec3_c(0.0f, 75.0f, 0.0f), 175.0f);

        f32 f = mPosition.y - mAcch.GetGroundH();
        if (mType != TYPE_HANG) {
            f = nw4r::ut::Max<f32>(f, 200.0f);
        }

        drawShadow(mShadow, nullptr, mWorldMtx, sSph, -1, -1, -1, -1, -1, f);
    }

    return SUCCEEDED;
}

void dAcOkibako_c::checkBreakHeight() {
    static const f32 breakHeightThreshhold = 10000.0f;

    if (mAcch.GetGroundH() != -1e9f) {
        mGroundHeight = mAcch.GetGroundH();
        return;
    }

    if (mGroundHeight - mPosition.y > breakHeightThreshhold) {
        breakBox();
    }
}

bool dAcOkibako_c::checkHitBomb() {
    return mCyl.ChkTgHit() && mCyl.ChkTgAtHitType(AT_TYPE_BOMB);
}

void dAcOkibako_c::updateGravity(bool isFree) {
    if (isFree) {
        mGravity = 0.0f;

        if (mGravityCheckTimer > 0) {
            if (--mGravityCheckTimer == 0) {
                mGravity = -4.0f;
            }
        } else if (mGravityCheckTimer == 0) {
            crrPos();

            if (mPosition.y != mOldPosition.y) {
                mGravityCheckTimer = 0;
                mGravity = -4.0f;
            } else {
                mGravityCheckTimer = 10;
            }
        } else {
            mGravity = -4.0f;
            mGravityCheckTimer++;
            crrPos();
        }
    } else {
        mGravityCheckTimer = 0;
        mGravity = -4.0f;
        crrPos();
    }
}

void dAcOkibako_c::breakBox() {
    if (!isState(StateID_DeleteEvent)) {
        startSound(SE_Kibako_BREAK);

        mVec3_c effPos = mPosition;
        effPos.y += 75.0f;

        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_736_, effPos, nullptr, nullptr, nullptr, nullptr, mPolyAttr0, mPolyAttr1
        );
        dEmitterBase_c *pEmitter = dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_738_, mPosition, nullptr, nullptr, nullptr, nullptr, 0, 0
        );
        if (pEmitter != nullptr) {
            pEmitter->bindShpEmitter(dJEffManager_c::KibakoHang, true);
        }

        if (mPathID == 0xFF) {
            SceneflagManager::sInstance->setFlag(mRoomID, mDestroySceneflag);
        }

        dBgS::GetInstance()->Release(&mBgW);

        if (mEventID != 0xFF) {
            changeState(StateID_DeleteEvent);
        } else {
            deleteRequest();
        }
    }
}

void dAcOkibako_c::updatePathGoalFlag() {
    static const f32 pntDistanceThreshold = 2500.0f;
    if (mbOnPath) {
        const Vec *pPnt = mPath.getPath().getPoint(0);
        if (pPnt != nullptr) {
            mVec3_c distToPnt(mPosition.x - pPnt->x, mPosition.y - pPnt->y, mPosition.z - pPnt->z);
            if (distToPnt.squareMagXZ() < pntDistanceThreshold) {
                SceneflagManager::sInstance->setFlag(mRoomID, mDestroySceneflag);
                return;
            }
        }
        SceneflagManager::sInstance->unsetFlag(mRoomID, mDestroySceneflag);
    }
}

void dAcOkibako_c::calcFall() {
    static const f32 fallThreshhold = -300.0f;
    if (isState(StateID_Wait)) {
        if (mFallDistance <= fallThreshhold && mAcch.ChkGndHit()) {
            startSound(SE_Kibako_LAND);
            mFallDistance = 0.0f;
        }

        f32 heightIncrement = mPosition.y - mOldPosition.y;
        if (cM::isZero(heightIncrement)) {
            mFallDistance = 0.0f;
        } else {
            mFallDistance += heightIncrement;
            if (mFallDistance < fallThreshhold) {
                mFallDistance = fallThreshhold;
            }
        }
    }
}

void dAcOkibako_c::initializeState_Wait() {
    mBgW.OnFlag0x04();
    field_0xDEA = 0;
}
void dAcOkibako_c::executeState_Wait() {
    if (mRestrictType == 1) {
        if (!mScatterSand.isLinked()) {
            mRestrictType = 0;
            mGravity = -4.0f;
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
        checkPushPull();
    }

    if (mbHitByBomb) {
        breakBox();
        return;
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
                changeState(StateID_PushPull);
            }
        }
    } else {
        mbCoOn = false;
        mCcBox.ClrCoSet();
    }
}
void dAcOkibako_c::finalizeState_Wait() {
    mbCoOn = false;
    mCcBox.ClrCoSet();
}

void dAcOkibako_c::initializeState_Fire() {
    mBgW.ClrFlag0x4();
    mFireTimer = 60;
    mCyl.ClrTgNoConHit();
    mCyl.SetTgType(AT_TYPE_BELLOWS | AT_TYPE_BOMB);
    field_0xDDC = 6.0f;
}
void dAcOkibako_c::executeState_Fire() {
    if (mbHitByBomb) {
        breakBox();
    }
    if (mFireTimer != 0) {
        mFireTimer--;
    }
}
void dAcOkibako_c::finalizeState_Fire() {}

void dAcOkibako_c::initializeState_PushPull() {
    static const f32 distanceScale = 0.1f;
    dAcPy_c::GetLinkM()->onFlags_0x360(0x10000);
    mMoveTimer = s16(dAcPy_c::GetLink()->sPushRelatedConstant * distanceScale) + 1;
}
void dAcOkibako_c::executeState_PushPull() {
    static const s16 sAngs[4] = {0x2000, 0x6000, -0x6000, -0x2000};
    holdSound(SE_Kibako_MOVE_LV);
    f32 radius = 75.0f;
    for (int i = 0; i < 4; ++i) {
        mVec3_c spawnPos = mPosition;
        getXZCirclePoint(spawnPos, sAngs[i] + mRotation.y, radius * 1.4142135f);
        mEmitters[i].holdEffect(PARTICLE_RESOURCE_ID_MAPPING_514_, spawnPos, nullptr, nullptr, nullptr, nullptr);
    }

    if (mbHitByBomb) {
        updatePathGoalFlag();
        changeState(StateID_Wait);
        return;
    }

    if (cLib::chasePosXZ(mPosition, mPreMovePosition, 10.0f)) {
        updatePathGoalFlag();
        if (mbAtLimit) {
            startSound(SE_Kibako_MOVE_LIMIT);
        }
        changeState(StateID_Wait);
        return;
    }

    if (!mSw.isLinked()) {
        if (mAcchCir.ChkWallHit()) {
            s32 target = cLib::targetAngleY(mPreMovePosition, mPosition);
            s32 wallAngle = mAcchCir.GetWallAngleY();
            if (target == wallAngle) {
                updatePathGoalFlag();
                startSound(SE_Kibako_MOVE_LIMIT);
                changeState(StateID_Wait);
                return;
            }
        }
    }

    if (mMoveTimer != 0) {
        mMoveTimer--;
    } else {
        updatePathGoalFlag();
        changeState(StateID_Wait);
    }
}
void dAcOkibako_c::finalizeState_PushPull() {
    dAcPy_c::GetLinkM()->offFlags_0x360(0x10000);
    mbOnlyChangeHeight = false;
}

void dAcOkibako_c::initializeState_RopeGrab() {
    static const int stateSet = 3;
    mGravity = 0.0f;
    mbDrawShadow = true;
}
void dAcOkibako_c::executeState_RopeGrab() {
    if (mbOnRope) {
        return;
    }

    mGravity = -4.0f;
    if (mAcch.ChkGndHit()) {
        startSound(SE_Kibako_LAND);
        breakBox();
    }
}
void dAcOkibako_c::finalizeState_RopeGrab() {
    mbDrawShadow = false;
}

void dAcOkibako_c::initializeState_DeleteEvent() {
    if (mEventID != 0xFF && !mEventRelated.isThisActorInEvent()) {
        Event event(mEventID, mRoomID, 0x100001, nullptr, nullptr);
        mEventRelated.scheduleEvent(event, 0);
    }
}
void dAcOkibako_c::executeState_DeleteEvent() {
    if (mEventID != 0xFF) {
        if (!mEventRelated.isThisActorInEvent()) {
            Event event(mEventID, mRoomID, 0x100001, nullptr, nullptr);
            mEventRelated.scheduleEvent(event, 0);
        }
    } else {
        if (!mEventRelated.isThisActorInEvent()) {
            deleteRequest();
        }
    }
}
void dAcOkibako_c::finalizeState_DeleteEvent() {}

bool dAcOkibako_c::checkCanTryMove(s16 dir, bool set) {
    static const s16 sAngs[3] = {0x4000, 0x0, -0x4000};
    mbAtLimit = false;

    static const f32 pushExtend = 2.0f;
    static const f32 probeIncrement = 73.0f;
    static const f32 yOffset = 10.0f;
    mVec3_c start = mPosition;
    mVec3_c end = mPosition;
    for (int i = 0; i < 3; ++i) {
        start = mPosition;
        f32 radius = probeIncrement;
        getXZCirclePoint(start, dir, radius);
        if (i != 1) {
            getXZCirclePoint(start, dir + sAngs[i], radius);
        }

        start.y += 10.0f;
        end = start;

        getXZCirclePoint(end, dir, dAcPy_c::GetLink()->sPushRelatedConstant + pushExtend);

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

void dAcOkibako_c::checkPushPull() {
    static const f32 distanceTheshold = 90000.0f;
    static const f32 heightThreshhold = 15.0f;
    mVec3_c toPlayer = dAcPy_c::GetLinkM()->getPostionDifference(*this);
    if (toPlayer.squareMagXZ() > distanceTheshold) {
        return;
    }

    if (nw4r::math::FAbs(dAcPy_c::GetLinkM()->mPosition.y - mPosition.y) > heightThreshhold) {
        field_0xDEA = 0;
        mBgW.ClrFlag0x4();
    } else {
        bool b = true;
        if (field_0xDEA == 0) {
            field_0xDEA = 45 + cM::rndInt(30);

            mAng ang0 = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);
            mAng ang1 = ang0 - mRotation.y;
            mAng rot = mRotation.y;
            if ((u16)((ang1)-0x2001) <= 0x3FFE || (u16)(ang1 + 0x5FFF) <= 0x3FFE) {
                rot = rot + 0x4000;
            }

            mAng rot2 = rot - 0x8000;
            if (!checkCanTryMove(rot, false) && !checkCanTryMove(rot2, false)) {
                b = false;
            }
            if (!b) {
                mBgW.ClrFlag0x4();
            } else {
                mBgW.OnFlag0x04();
            }
        } else {
            field_0xDEA--;
        }
    }
}

dAcOsw_c *dAcOkibako_c::getSwitch(const mVec3_c &pos) {
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

bool dAcOkibako_c::isSwitchInArea(const mVec3_c &pos) {
    static const s16 sAngs[4] = {0x2000, 0x6000, -0x6000, -0x2000};

    static const f32 switchHeightAdjust = 10.0f;

    mVec3_c center = pos;
    center.y -= switchHeightAdjust;

    dAcOsw_c *pSw = mSw.get();
    if (pSw == nullptr) {
        return true;
    }

    mVec3_c extent(150.0f, 30.0f, 150.0f);
    mArea.set(center, mRotation.y, extent, nullptr, 0.0f);
    f32 radius = (pSw->getField_0x5E8() - switchHeightAdjust) * 1.4142135f * 0.5f;
    for (int i = 0; i < 4; ++i) {
        mVec3_c checkPos = pSw->mPosition;
        getXZCirclePoint(checkPos, sAngs[i] + pSw->mRotation.y, radius);

        if (mArea.checkAreaBox(checkPos)) {
            return true;
        }
    }

    return false;
}

bool dAcOkibako_c::checkNotHittingNewSwitch() {
    static const f32 probeIncrement = 15.0f;
    mVec3_c pos = mPreMovePosition;
    f32 radius = 75.0f;
    radius -= 20.0f;
    getXZCirclePoint(pos, mAngle.y, radius);

    if (mSw.isLinked() && !isSwitchInArea(pos)) {
        mSw.get()->removeBox();
        mSw.unlink();
    }

    mVec3_c checkPos = pos;
    getXZCirclePoint(checkPos, mAngle.y + 0x4000, probeIncrement);

    dAcOsw_c *pSw = getSwitch(checkPos);
    if (pSw == nullptr) {
        mVec3_c checkPos = pos;
        getXZCirclePoint(checkPos, mAngle.y - 0x4000, probeIncrement);
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

void dAcOkibako_c::crrPos() {
    if (mbOnlyChangeHeight) {
        mVec3_c pos = mPosition;
        mAcch.CrrPos(*dBgS::GetInstance());
        mPosition.x = pos.x;
        mPosition.z = pos.z;
    } else {
        mAcch.CrrPos(*dBgS::GetInstance());
    }
}
