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
    mAcceleration = -4.0f;
    mMaxSpeed = -80.0f;

    field_0xDEC = -(5 + cM::rndInt(10));

    for (int i = 0; i < 4; ++i) {
        mEmitters[i].init(this);
    }

    if (mRestrictSceneflag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mRestrictSceneflag)) {
        mRestrictType = 2;
        mAcceleration = 0.0f;
    }

    mbOnRope = false;

    if (mType == TYPE_HANG) {
        mAcch.CrrPos(*dBgS::GetInstance());
        mAcceleration = 0.0f;
        field_0xDEC = 0;
        changeState(StateID_RopeGrab);
        mBoundingBox.Set(mVec3_c(-750.0f, -800.0f, -250.0f), mVec3_c(100.0f, 200.0f, 1000.0f));
    } else {
        changeState(StateID_RopeGrab);
        mBoundingBox.Set(mVec3_c(-250.0f, -100.0f, -250.0f), mVec3_c(250.0f, 200.0f, 250.0f));
    }

    return SUCCEEDED;
}

int dAcOkibako_c::actorPostCreate() {
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

    field_0xEC5 = true;
    if (mManager.getExecuteOrder() != fProfile::OBJ_KIBAKO) {
        updateExecutePriority((*fProfile::sProfileList)[fProfile::OBJ_KIBAKO]->m_execute_order);
    }

    executeState();
    calcVelocity();

    mPosition += mVelocity;
    mPosition += mStts.GetCcMove();

    bool isWait = isState(StateID_Wait);

    if ((!isState(StateID_RopeGrab) || !mbOnRope) && mRestrictType == 0) {
        fn_561_11C0(isWait);
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
        field_0xDE6 = 10;
    }

    if (field_0xDE6 != 0) {
        mBgW.Move();
        field_0xDE6--;
    }

    if (dBgS::GetInstance()->ChkMoveBG(mAcch.mGnd, true)) {
        const dAcObjBase_c *pObj = dBgS::GetInstance()->GetActorPointer(mAcch.mGnd);
        if (pObj != nullptr && pObj->mProfileName == fProfile::OBJ_KIBAKO) {
            if (!static_cast<const dAcOkibako_c *>(pObj)->field_0xEC5) {
                updateExecutePriority((*fProfile::sProfileList)[fProfile::OBJ_KIBAKO]->m_execute_order + 1);
            }
        }
    }
    return SUCCEEDED;
}

int dAcOkibako_c::draw() {
    field_0xEC5 = false;

    if (isState(StateID_DeleteEvent)) {
        return SUCCEEDED;
    }

    drawModelType1(&mMdl);
    if (field_0xECC) {
        static mSphere_c sSph(mVec3_c(0.0f, 75.0f, 0.0f), 175.0f);

        f32 f = mPosition.y - mAcch.GetGroundH();
        if (mType != TYPE_HANG) {
            f = nw4r::ut::Max<f32>(f, 200.0f);
        }

        drawShadow(mShadow, nullptr, mWorldMtx, &sSph, -1, -1, -1, -1, -1, f);
    }

    return SUCCEEDED;
}

void dAcOkibako_c::checkBreakHeight() {
    if (mAcch.GetGroundH() != -1e9f) {
        field_0xDD8 = mAcch.GetGroundH();
        return;
    }

    if (field_0xDD8 - mPosition.y > 10000.0f) {
        breakBox();
    }
}

bool dAcOkibako_c::checkHitBomb() {
    return mCyl.ChkTgHit() && mCyl.ChkTgAtHitType(AT_TYPE_BOMB);
}

void dAcOkibako_c::fn_561_11C0(bool b0) {
    if (b0) {
        mAcceleration = 0.0f;

        if (field_0xDEC > 0) {
            if (--field_0xDEC == 0) {
                mAcceleration = -4.0f;
            }
        } else if (field_0xDEC == 0) {
            fn_561_27A0();

            if (mPosition.y != mOldPosition.y) {
                field_0xDEC = 0;
                mAcceleration = -4.0f;
            } else {
                field_0xDEC = 10;
            }
        } else {
            mAcceleration = -4.0f;
            field_0xDEC++;
            fn_561_27A0();
        }
    } else {
        field_0xDEC = 0;
        mAcceleration = -4.0f;
        fn_561_27A0();
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
    if (mbOnPath) {
        const Vec *pPnt = mPath.getPath().getPoint(0);
        if (pPnt != nullptr) {
            mVec3_c distToPnt(mPosition.x - pPnt->x, mPosition.y - pPnt->y, mPosition.z - pPnt->z);
            if (distToPnt.squareMagXZ() < 2500.0f) {
                SceneflagManager::sInstance->setFlag(mRoomID, mDestroySceneflag);
                return;
            }
        }
        SceneflagManager::sInstance->unsetFlag(mRoomID, mDestroySceneflag);
    }
}

void dAcOkibako_c::calcFall() {
    if (isState(StateID_Wait)) {
        if (field_0xDE0 <= -300.0f && mAcch.ChkGndHit()) {
            startSound(SE_Kibako_LAND);
            field_0xDE0 = 0.0f;
        }

        f32 heightIncrement = mPosition.y - mOldPosition.y;
        if (cM::isZero(heightIncrement)) {
            field_0xDE0 = 0.0f;
        } else {
            field_0xDE0 += heightIncrement;
            if (field_0xDE0 < -300.0f) {
                field_0xDE0 = -300.0f;
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
        fn_561_2150();
    }

    if (mbHitByBomb) {
        breakBox();
        return;
    }

    if (mbPlayerIsPushPull) {
        mbPlayerIsPushPull = false;
        if (!field_0xEC9) {
            field_0xEC9 = true;
            mCcBox.OnCoSet();
        } else if (!mCcBox.ChkCoHit() && fn_561_1E10(mAngle.y, true)) {
            field_0xDCC = mPosition;
            getXZCirclePoint(field_0xDCC, mAngle.y, dAcPy_c::sPushRelatedConstant);
            if (fn_561_2530()) {
                changeState(StateID_PushPull);
            }
        }
    } else {
        field_0xEC9 = false;
        mCcBox.ClrCoSet();
    }
}
void dAcOkibako_c::finalizeState_Wait() {
    field_0xEC9 = false;
    mCcBox.ClrCoSet();
}

void dAcOkibako_c::initializeState_Fire() {
    mBgW.ClrFlag0x4();
    field_0xDE4 = 60;
    mCyl.ClrTgNoConHit();
    mCyl.SetTgType(AT_TYPE_BELLOWS | AT_TYPE_BOMB);
    field_0xDDC = 6.0f;
}
void dAcOkibako_c::executeState_Fire() {
    if (mbHitByBomb) {
        breakBox();
    }
    if (field_0xDE4 != 0) {
        field_0xDE4--;
    }
}
void dAcOkibako_c::finalizeState_Fire() {}

void dAcOkibako_c::initializeState_PushPull() {
    dAcPy_c::GetLinkM()->onFlags_0x360(0x10000);
    field_0xDE8 = s16(dAcPy_c::GetLink()->sPushRelatedConstant * 0.1f) + 1;
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

    if (cLib::chasePosXZ(mPosition, field_0xDCC, 10.0f)) {
        updatePathGoalFlag();
        if (field_0xECB) {
            startSound(SE_Kibako_MOVE_LIMIT);
        }
        changeState(StateID_Wait);
        return;
    }

    if (!mSw.isLinked()) {
        if (mAcchCir.ChkWallHit()) {
            s32 target = cLib::targetAngleY(field_0xDCC, mPosition);
            s32 wallAngle = mAcchCir.GetWallAngleY();
            if (target == wallAngle) {
                updatePathGoalFlag();
                startSound(SE_Kibako_MOVE_LIMIT);
                changeState(StateID_Wait);
                return;
            }
        }
    }

    if (field_0xDE8 != 0) {
        field_0xDE8--;
    } else {
        updatePathGoalFlag();
        changeState(StateID_Wait);
    }
}
void dAcOkibako_c::finalizeState_PushPull() {
    dAcPy_c::GetLinkM()->offFlags_0x360(0x10000);
    field_0xEC7 = false;
}

void dAcOkibako_c::initializeState_RopeGrab() {
    mAcceleration = 0.0f;
    field_0xECC = true;
}
void dAcOkibako_c::executeState_RopeGrab() {
    if (mbOnRope) {
        return;
    }

    mAcceleration = -4.0f;
    if (mAcch.ChkGndHit()) {
        startSound(SE_Kibako_LAND);
        breakBox();
    }
}
void dAcOkibako_c::finalizeState_RopeGrab() {
    field_0xECC = false;
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

bool dAcOkibako_c::fn_561_1E10(s16 dir, bool set) {
    static const s16 sAngs[3] = {0x4000, 0x0, -0x4000};
    field_0xECB = false;

    mVec3_c start = mPosition;
    mVec3_c end = mPosition;
    for (int i = 0; i < 3; ++i) {
        start = mPosition;
        f32 radius = 73.0f;
        getXZCirclePoint(start, dir, radius);
        if (i != 1) {
            getXZCirclePoint(start, dir + sAngs[i], radius);
        }

        start.y += 10.0f;
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
                    field_0xECB = true;
                }
            }
        }
    }
    return true;
}

inline bool angleInRange(s16 rot, u16 a, u16 b) {
    return rot - (a + 1) <= b - 2;
}
void dAcOkibako_c::fn_561_2150() {
    mVec3_c toPlayer = dAcPy_c::GetLinkM()->getPostionDifference(*this);
    if (toPlayer.squareMagXZ() > 90000.0f) {
        return;
    }

    if (nw4r::math::FAbs(dAcPy_c::GetLinkM()->mPosition.y - mPosition.y) > 15.0f) {
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
            if (!fn_561_1E10(rot, false) && !fn_561_1E10(rot2, false)) {
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

bool dAcOkibako_c::fn_561_2530() {
    mVec3_c pos = field_0xDCC;
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
                field_0xEC7 = true;
            }
        } else if (mSw.get() != pSw) {
            return false;
        }
    }
    return true;
}

void dAcOkibako_c::fn_561_27A0() {
    if (field_0xEC7) {
        mVec3_c pos = mPosition;
        mAcch.CrrPos(*dBgS::GetInstance());
        mPosition.x = pos.x;
        mPosition.z = pos.z;
    } else {
        mAcch.CrrPos(*dBgS::GetInstance());
    }
}
