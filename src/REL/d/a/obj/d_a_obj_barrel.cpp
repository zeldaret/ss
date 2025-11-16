#include "d/a/obj/d_a_obj_barrel.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/npc/d_a_npc_ce_friend.h"
#include "d/a/npc/d_a_npc_ce_lady.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_linkage.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/math/math_arithmetic.h"
#include "rvl/MTX/mtxvec.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/event_manager.h"

f32 sFloats1(1.f);

SPECIAL_ACTOR_PROFILE(OBJ_BARREL, dAcOBarrel_c, fProfile::OBJ_BARREL, 0x239, 0, 3);

STATE_DEFINE(dAcOBarrel_c, Wait);
STATE_DEFINE(dAcOBarrel_c, Grab);
STATE_DEFINE(dAcOBarrel_c, GrabUp);
STATE_DEFINE(dAcOBarrel_c, GrabPut);
STATE_DEFINE(dAcOBarrel_c, Slope);
STATE_DEFINE(dAcOBarrel_c, Water);
STATE_DEFINE(dAcOBarrel_c, Water2);
STATE_DEFINE(dAcOBarrel_c, Explode);
STATE_DEFINE(dAcOBarrel_c, KrakenBorn);
STATE_DEFINE(dAcOBarrel_c, Rebirth);

static dCcD_SrcCyl sSrcCyl = {
    {
     /* mObjAt */ {AT_TYPE_PHYSICS, 0x10013, {0, 0, 0}, 4, 0, 0, 0, 0, 0},
     /* mObjTg */
        {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000), 0x2000111, {0, 0xF, 0x407}, 0, 0},
     /* mObjCo*/ {0xE9},
     },
    {
     55.f, 110.f,
     }
};
static dCcD_SrcSph sSrcSph = {
    {
     /* mObjAt */ {AT_TYPE_PHYSICS, 0x10003, {0, 0, 0}, 4, 0, 0, 0, 0, 0},
     /* mObjTg */
        {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000 | AT_TYPE_WIND),
         0x2000111,
         {0, 0xF, 0x407},
         0,
         0},
     /* mObjCo*/ {0xE9},
     },
    {55.f}
};

static const char *sBarrelResFile[] = {
    "Barrel",     // Normal
    "BarrelBomb", // Explode
    "Barrel",     // Type2
    "Barrel",     // Kraken
};
static const char *sBarrelResMdl[] = {
    "Barrel",     // Normal
    "BarrelBomb", // Explode
    "Barrel",     // Type2
    "Barrel",     // Kraken
};

bool dAcOBarrel_c::createHeap() {
    mType = getFromParams(0, 0xF);
    if (mType == 0xF) {
        mType = 0;
    }

    mResFile = nw4r::g3d::ResFile(getOarcResFile(sBarrelResFile[mType]));
    TRY_CREATE(mMdl.create(mResFile.GetResMdl(sBarrelResMdl[mType]), &mAllocator, 0x120, 1, nullptr));

    return true;
}

int dAcOBarrel_c::actorCreate() {
    field_0xE18 = getFromParams(12, 0xFF);
    mBrokenFlag = getFromParams(20, 0xFF);
    field_0xE10 = getFromParams(4, 0x3);

    field_0xE04 = (int)getFromParams(6, 0x3) <= 1 && mType == Normal;
    field_0xE05 = (int)getFromParams(6, 0x3) == 1 && mType == Normal;

    field_0xE07 = getFromParams(10, 0x3) == 0;
    field_0xE0B = getFromParams(10, 0x3) == 1;
    field_0xE08 = getFromParams(8, 0x3) == 0;

    CREATE_ALLOCATOR(dAcOBarrel_c);

    if (!field_0xE08 && !isType_0()) {
        fn_8002EA30(-100.f, 0.f, 2.f, 1.f, 4.f, &mObjAcch);
    }

    mObjAcch.Set(this, 1, &mAcchCir);
    mAcchCir.SetWall(20.f, 55.f);
    mObjAcch.SetField_0xC(4);
    mStts.SetRank(9);

    mCyl.Set(sSrcCyl);
    mCyl.SetStts(mStts);
    if (!isType_0()) {
        mCyl.ClrAtSet();
    }
    mSph.Set(sSrcSph);
    mSph.SetStts(mStts);
    mObjAcch.OffClrSpeedY();

    mAcceleration = -4.f;
    mMaxSpeed = -40.f;

    field_0xDD8.y = 0.f;
    field_0xDEC = 0;
    quat_0xD50.set(1.f, 0.f, 0.f, 0.f);
    mScale.set(1.f, 1.f, 1.f);
    quat_0xD70.set(1.f, 0.f, 0.f, 0.f);
    quat_0xD80.set(1.f, 0.f, 0.f, 0.f);
    quat_0xD90.set(1.f, 0.f, 0.f, 0.f);
    quat_0xDA0.set(1.f, 0.f, 0.f, 0.f);

    if (isType_0()) {
        mRotation.z += 0x4000;
        unsetActorProperty(AC_PROP_0x1);
        mCyl.SetAtGrp(0x10);
    }

    /* field_0xCC8 = this */
    /* field_0xCFC = this */

    if (mRotation.x != 0 || mRotation.z != 0) {
        field_0xE02 = true;
        mQuat_c qz, qx;
        qx.setAxisRotation(mVec3_c::Ex, mRotation.x.radian());
        qz.setAxisRotation(mVec3_c::Ez, mRotation.x.radian());
        quat_0xD80 = qx * qz;

        mRotation.x = mAng::Zero();
        mRotation.z = mAng::Zero();
    }

    mMdl.setPriorityDraw(0x7F, 0x7F);
    if (mBrokenFlag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mBrokenFlag)) {
        return FAILED;
    }

    if (mType == 3) {
        mStateMgr.changeState(StateID_KrakenBorn);
    } else {
        mStateMgr.changeState(StateID_Wait);
        getLinkage().set(0x24, 51.f, 0.f, 0.f, nullptr);
    }
    updateMatrix();
    getLinkage().bushTpFunc(mObjAcch);

    mBoundingBox.Set(mVec3_c(-55.f, -10.f, -55.f), mVec3_c(55.f, 150.f, 55.f));

    return SUCCEEDED;
}

int dAcOBarrel_c::actorPostCreate() {
    if (field_0xE04) {
        mCeLady.link(static_cast<dAcNpcCeLady_c *>(fManager_c::searchBaseByProfName(fProfile::NPC_CE_LADY)));
        mCeFriend.link(static_cast<dAcNpcCeFriend_c *>(fManager_c::searchBaseByProfName(fProfile::NPC_CE_FRIEND)));
    }

    if (field_0xE08) {
        if (!isType_0()) {
            if (dBgS_ObjGndChk::CheckPos(mPosition + 100.f * mVec3_c::Ey)) {
                mPosition.y = dBgS_ObjGndChk::GetGroundHeight();
            }
        }
    }

    f32 scale;
    if (getFromParams(8, 0x3) == 0) {
        scale = 1.f;
    } else {
        scale = 2.5f;
    }

    if (field_0xE10 == 0 && isTimeshiftableType()) {
        (void)mTimeArea.check(mRoomID, mPosition, 0, 0.f, 0.1f);
        if (0.f < mTimeArea.getDistMaybe()) {
            field_0xE0C = true;
            mScale.set(scale, scale, scale);
        } else {
            field_0xE0D = true;
            mScale.set(0.f, 0.f, 0.f);
        }
    }

    mWaterEffect.init(this, 100.f, scale, 0.f);
    mWaterEffect.setIsSmall(true);

    if (field_0xE05 || checkFlag_0xE18()) {
        mStts.SetDefaultRank();
    } else if (field_0xE04) {
        mStts.SetRank(0xC);
    }
    return SUCCEEDED;
}

int dAcOBarrel_c::doDelete() {
    return SUCCEEDED;
}

int dAcOBarrel_c::actorExecute() {
    bool b4D00 = fn_293_4D00();
    mStateMgr.executeState();

    if (!getLinkage().checkConnection(dLinkage_c::CONNECTION_1)) {
        if (!b4D00) {
            calcVelocity();
            mPosition += mVelocity;
            mPosition += mStts.GetCcMove();
            fn_293_3DB0();
        }

        field_0xDD8.y += mPosition.y - mOldPosition.y;
    }

    if (!b4D00) {
        getLinkage().bushTpFunc(mObjAcch);
        unsetActorProperty(AC_PROP_0x1);
    } else {
        setActorProperty(AC_PROP_0x1);
    }

    if (!mStateMgr.isState(StateID_Explode) && fn_293_45A0()) {
        fn_293_4200();
    }

    if (!mStateMgr.isState(StateID_Explode)) {
        if (fn_293_50B0()) {
            mCyl.SetAtGrp(0xC);
        } else {
            mCyl.OffAtGrp(0xC);
        }
    }

    if (field_0xE10 == 0 && isTimeshiftableType()) {
        fn_293_5150();
    }

    if (!mStateMgr.isState(StateID_Explode)) {
        if (!(field_0xE10 == 0 && isTimeshiftableType() && mScale.y < 1.f)) {
            mCyl.SetAtVec(mVelocity);
            mCyl.SetC(mPosition);
            mCyl.SetR(mScale.x * 55.f);
            mCyl.SetH(mScale.y * 110.f);

            dCcS::GetInstance()->Set(&mCyl);
        }
    } else {
        f32 radius = getFromParams(8, 0x3) == 0 ? 800.f : 400.f;
        mSph.SetR(radius);
        mSph.SetC(mPosition);
        mSph.SetAtType(AT_TYPE_BOMB);
        mSph.SetAtGrp(0x1C);
        mSph.SetAtDamage(10);
        mSph.SetAtVec(mVec3_c::Zero);

        dCcS::GetInstance()->Set(&mSph);
        dCcS::GetInstance()->GetMassMng().SetObj(&mSph, 1);
    }

    if (!b4D00 || field_0xE02 == 0) {
        updateMatrix();
    }

    mMdl.setLocalMtx(mWorldMtx);
    mMdl.setScale(mScale);

    mPositionCopy3 = mPositionCopy2 = mPosition + mVec3_c(0.f, 51.f, 0.f);
    mPositionCopy3.y += 71.f;

    return SUCCEEDED;
}

void dAcOBarrel_c::registerInEvent() {
    if (mStateMgr.isState(StateID_Wait) && mObjAcch.ChkGndHit()) {
        const dAcObjBase_c *pObj = dBgS::GetInstance()->GetActorPointer(mObjAcch.GetGnd());
        if (pObj && EventManager::FUN_800a0570(pObj)) {
            setActorProperty(AC_PROP_0x4);
        }
    }
}

void dAcOBarrel_c::unkVirtFunc_0x6C() {
    if (mStateMgr.isState(StateID_Wait)) {
        unsetActorProperty(AC_PROP_0x4);
    }
}

void dAcOBarrel_c::doInteraction(s32 i) {
    if (i != PICK_UP) {
        return;
    }

    if (checkFlag_0xE18()) {
        SceneflagManager::sInstance->setFlag(mRoomID, field_0xE18);
    }
}

int dAcOBarrel_c::draw() {
    drawModelType1(&mMdl);

    static mQuat_c shadow(0.f, 50.f, 0.f, 80.f);

    if (mYOffset >= 0.f) {
        drawShadow(mShadow, nullptr, mWorldMtx, &shadow, -1, -1, -1, -1, -1, mPosition.y - mObjAcch.GetGroundH());
    }

    return SUCCEEDED;
}

void dAcOBarrel_c::initializeState_Wait() {
    mAcceleration = -4.f;
    field_0xE0E = false;
}
void dAcOBarrel_c::executeState_Wait() {
    fn_293_30D0();

    if (field_0xE00 && field_0xE02) {
        field_0xE01 = fn_293_4ED0();
    }

    if (mObjAcch.ChkGroundLanding() && !field_0xE0E) {
        fn_293_37B0();
        if (mVelocity.y > 0.f) {
            dJEffManager_c::spawnGroundEffect(mPosition, mPolyAttr0, mPolyAttr1, mVelocity, 0, 1.f, field_0x1B0);
        }

        if (field_0xE0A) {
            startSound(SE_Barrel_ROLL_GROUND);
        }
        field_0xE0A = true;
    } else if (mObjAcch.ChkGndHit()) {
        field_0xDD8.y = 0.f;
        mVelocity.y = 0.f;
        field_0xDBC = mVec3_c::Ey;
        fn_293_5360();
        if (!field_0xE02) {
            fn_293_58C0();
            mVec3_c v = field_0xDBC;
            v.rotY(-mRotation.y);
            quat_0xD70.Set(mVec3_c::Ey, v);
        }

        if (fn_293_4A90(mAng::deg2short(1.f))) {
            mStateMgr.changeState(StateID_Slope);
            return;
        }

        if (!field_0xE08 && !isType_0() && mYOffset < 0.f && fn_293_4BC0()) {
            mSpeed = 0.f;
            field_0xE00 = false;
            fn_293_5910();
        }

        if (!field_0xE08 && !isType_0() && checkYOffsetField_0x100()) {
            killNoItemDrop();
            return;
        }

        fn_293_3560();

        if (mSpeed < 0.2f) {
            field_0xE00 = false;
            sLib::chase(&mSpeed, 0.f, 0.05f);
        }

        if (mSpeed > 0.f && field_0xE02) {
            holdSoundWithFloatParam(SE_Barrel_ROLL_LV, mSpeed);
            mCyl.OnAtSet();
        } else {
            mCyl.OffAtSet();
        }
    }

    if (fn_293_4C60()) {
        mStateMgr.changeState(StateID_Water);
    } else if (getLinkage().checkConnection(dLinkage_c::CONNECTION_1)) {
        mStateMgr.changeState(StateID_GrabUp);
    }
}
void dAcOBarrel_c::finalizeState_Wait() {}

void dAcOBarrel_c::initializeState_Grab() {
    mVelocity = mVec3_c::Zero;
    mAngle = mAng3_c::Zero;

    field_0xE00 = false;

    mCyl.SetCo_0x400();
    mCyl.ClrAtSet();
}
void dAcOBarrel_c::executeState_Grab() {
    if (!getLinkage().checkState(dLinkage_c::STATE_DELETE)) {
        field_0xE11 = getLinkage().getState();
    }

    if (dAcPy_c::GetLink()->checkCurrentAction(/* PUT_DOWN_MEDIUM */ 0x42)) {
        field_0xE00 = false;
        mStateMgr.changeState(StateID_GrabPut);
    } else if (!getLinkage().checkConnection(dLinkage_c::CONNECTION_1)) {
        field_0xE00 = true;
        field_0xE01 = fn_293_4ED0();
        mStateMgr.changeState(StateID_Wait);
    }
}
void dAcOBarrel_c::finalizeState_Grab() {
    field_0xDD8.y = 0.f;
    field_0xDD4 = cM::rndF(40.f);
    mCyl.ClrCo_0x400();
    u8 state = getLinkage().getState();
    if (state == dLinkage_c::STATE_2 || state == dLinkage_c::STATE_3) {
        mCyl.OnAtSet();
    }
}

void dAcOBarrel_c::initializeState_GrabUp() {
    // This entire function seems goofy
    mVec3_c v = mVec3_c::Ey;
    MTXMultVecSR(mWorldMtx, v, v);
    mAng rotX = mAng::fromRad(v.angle(mVec3_c::Ex));
    bool z = rotX > 0x4000;

    mAng rotY = dAcPy_c::GetLink()->mRotation.y - mRotation.y;

    mQuat_c qy, qz;
    qy.setAxisRotation(mVec3_c::Ey, mAng::short2rad(sLib::absDiff(rotY, 0) > 0x4000 ? rotY += 0x8000 : rotY));
    qz.setAxisRotation(mVec3_c::Ez, mAng::short2rad(z ? 0x4000 : -0x4000));

    quat_0xD60 = qy * qz;
    if (!field_0xE02) {
        quat_0xD70 = quat_0xD50;
    }

    mCyl.SetCo_0x400();
    mCyl.OffAtSet();

    if (field_0xE04) {
        field_0xE15 = 10;
    }

    mStts.SetRank(9);
}
void dAcOBarrel_c::executeState_GrabUp() {
    if (field_0xE04 || field_0xE05) {
        dAcNpcCeLady_c *pCeLady = mCeLady.get();
        dAcNpcCeFriend_c *pCeFriend = mCeFriend.get();
        if (0 == sLib::calcTimer(&field_0xE15)) {
            if ((pCeLady && pCeLady->fn_12_1C20(this)) || (pCeFriend && pCeFriend->fn_11_17C0(this))) {
                getLinkage().fn_80050EA0(this);
                quat_0xD80 = quat_0xD60;
                mStateMgr.changeState(StateID_Wait);
                return;
            }
            field_0xDD8.y = 0.f;
        }
    }

    if (dAcPy_c::GetLink()->checkCurrentAction(/* PICK UP */ 0x40)) {
        quat_0xD80.slerpTo(quat_0xD60, 0.2f, quat_0xD80);
        quat_0xD80.normalise();
        quat_0xD70.slerpTo(quat_0xD50, 0.2f, quat_0xD70);
    } else {
        if (!getLinkage().checkConnection(dLinkage_c::CONNECTION_1)) {
            field_0xDD8.y = 0.f;
            mStateMgr.changeState(StateID_Wait);
        } else {
            mStateMgr.changeState(StateID_Grab);
        }
    }
}
void dAcOBarrel_c::finalizeState_GrabUp() {
    mCyl.ClrCo_0x400();
    mCyl.OffAtSet();
    field_0xE02 = true;
}

void dAcOBarrel_c::initializeState_GrabPut() {
    field_0xE12 = 8;

    if (EventManager::isInEvent() && EventManager::isCurrentEvent("CarryReqCancel")) {
        if (field_0xE05 || checkFlag_0xE18()) {
            mStts.SetDefaultRank();
        } else if (field_0xE04) {
            mStts.SetRank(0xC);
        }
    }
}
void dAcOBarrel_c::executeState_GrabPut() {
    if (dAcPy_c::GetLink()->checkCurrentAction(/* PUT_DOWN_MEDIUM */ 0x42) && 0 == sLib::calcTimer(&field_0xE12)) {
        quat_0xD80.slerpTo(quat_0xD50, 0.2f, quat_0xD80);
        quat_0xD80.normalise();
    }
    if (!getLinkage().checkConnection(dLinkage_c::CONNECTION_1)) {
        mStateMgr.changeState(StateID_Wait);
    }
}
void dAcOBarrel_c::finalizeState_GrabPut() {
    field_0xE02 = false;
    quat_0xD80 = quat_0xD50;

    if (dBgS_ObjGndChk::CheckPos(mPosition + mVec3_c::Ey * 10.f) &&
        nw4r::math::FAbs(mPosition.y - dBgS_ObjGndChk::GetGroundHeight()) < 100.f) {
        return;
    }
    mCyl.OnAtSet();
}

void dAcOBarrel_c::initializeState_Slope() {}
void dAcOBarrel_c::executeState_Slope() {}
void dAcOBarrel_c::finalizeState_Slope() {}
void dAcOBarrel_c::initializeState_Water() {}
void dAcOBarrel_c::executeState_Water() {}
void dAcOBarrel_c::finalizeState_Water() {}
void dAcOBarrel_c::initializeState_Water2() {}
void dAcOBarrel_c::executeState_Water2() {}
void dAcOBarrel_c::finalizeState_Water2() {}
void dAcOBarrel_c::initializeState_Explode() {}
void dAcOBarrel_c::executeState_Explode() {}
void dAcOBarrel_c::finalizeState_Explode() {}
void dAcOBarrel_c::initializeState_KrakenBorn() {}
void dAcOBarrel_c::executeState_KrakenBorn() {}
void dAcOBarrel_c::finalizeState_KrakenBorn() {}
void dAcOBarrel_c::initializeState_Rebirth() {}
void dAcOBarrel_c::executeState_Rebirth() {}
void dAcOBarrel_c::finalizeState_Rebirth() {}
