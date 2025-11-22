#include "d/a/obj/d_a_obj_barrel.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_player.h"
#include "d/a/npc/d_a_npc_ce_friend.h"
#include "d/a/npc/d_a_npc_ce_lady.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_pc.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/c/c_m3d_g_pla.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_light_env.h"
#include "d/d_linkage.h"
#include "d/d_rumble.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "egg/math/eggMath.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "math.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/math/math_arithmetic.h"
#include "rvl/MTX/mtxvec.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/special_item_drop_mgr.h"

struct dAcOBarrel_c_Param_c {
    static const f32 _0;
    static const f32 _1;
    static const f32 _2;
    static const f32 _3;
    static const f32 _4;
    static const f32 _5;
    static const s16 _6;
    static const f32 _7;
    static const s16 _8;
    static const f32 _9;
    static const f32 _10;
    static const s16 _11;
    static const f32 _12;
    static const f32 _13;
};

const f32 dAcOBarrel_c_Param_c::_0 = 2.5f;
const f32 dAcOBarrel_c_Param_c::_1 = 10.f;
const f32 dAcOBarrel_c_Param_c::_2 = 7.f;
const f32 dAcOBarrel_c_Param_c::_3 = 0.125f;
const f32 dAcOBarrel_c_Param_c::_4 = 0.2f;
const f32 dAcOBarrel_c_Param_c::_5 = 0.2f;
const s16 dAcOBarrel_c_Param_c::_6 = 0x5A00;
const f32 dAcOBarrel_c_Param_c::_7 = 15.f;
const s16 dAcOBarrel_c_Param_c::_8 = 0x0500;
const f32 dAcOBarrel_c_Param_c::_9 = 0.8f;
const f32 dAcOBarrel_c_Param_c::_10 = 10.f;
const s16 dAcOBarrel_c_Param_c::_11 = 0x0300;
const f32 dAcOBarrel_c_Param_c::_12 = 0.8f;
const f32 dAcOBarrel_c_Param_c::_13 = 5.f;

// End HIO

// Data Floats
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
namespace {
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
} // namespace

static const char *sBarrelResFile[] = {
    "Barrel",     // Normal
    "BarrelBomb", // Bomb
    "Barrel",     // Type2
    "Barrel",     // Kraken
};
static const char *sBarrelResMdl[] = {
    "Barrel",     // Normal
    "BarrelBomb", // Bomb
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

void dummy_func() {
    struct f {
        f32 a;
    } a = {2.f};
}

int dAcOBarrel_c::actorCreate() {
    field_0xE18 = getFromParams(12, 0xFF);
    mBrokenFlag = getFromParams(20, 0xFF);
    field_0xE10 = getFromParams(4, 0x3);

    field_0xE04 = (int)getFromParams(6, 0x3) <= 1 && mType == Normal;
    field_0xE05 = (int)getFromParams(6, 0x3) == 1 && mType == Normal;

    field_0xE07 = getFromParams(10, 0x3) == 0;

    /**
      D300: r06:  l0: id:FC 0F params1: 0xFFFA57F0 -> 0x01 (Seed Drops)
    D300_1: r08:  l0: id:FC 33 params1: 0xFCEFF7F0 -> 0x01 (Bomb Drops)
    D300_1: r08:  l0: id:FC 37 params1: 0xFCFFF7F0 -> 0x01 (Bomb Drops)
    D300_1: r08:  l0: id:FC 38 params1: 0xFD0FF7F0 -> 0x01 (Bomb Drops)
      D301: r00:  l0: id:00 12 params1: 0xFFFFF7F0 -> 0x01 (Arrow Drops)
      D301: r00:  l0: id:00 13 params1: 0xFFFFF7F0 -> 0x01 (Arrow Drops)
      D301: r00:  l0: id:00 14 params1: 0xFFFFF7F0 -> 0x01 (Arrow Drops)
      D301: r00:  l0: id:00 15 params1: 0xFFFFF7F0 -> 0x01 (Arrow Drops)
    */
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

    field_0xDDC = 0.f;
    field_0xDEC = mAng::Zero();
    quat_0xD50.set(1.f, 0.f, 0.f, 0.f);
    mScale.set(1.f, 1.f, 1.f);
    quat_0xD90.set(1.f, 0.f, 0.f, 0.f);
    quat_0xD80.set(1.f, 0.f, 0.f, 0.f);
    quat_0xD70.set(1.f, 0.f, 0.f, 0.f);
    quat_0xDA0.set(1.f, 0.f, 0.f, 0.f);

    if (isType_0()) {
        mRotation.z += 0x4000;
        unsetActorProperty(AC_PROP_0x1);
        mCyl.SetAtGrp(0x10);
    }

    mEmitter0.init(this);
    mEmitter1.init(this);

    if (mRotation.x != 0 || mRotation.z != 0) {
        field_0xE02 = true;
        mQuat_c qz, qx;
        qx.setAxisRotation(mVec3_c::Ex, mRotation.x.radian());
        qz.setAxisRotation(mVec3_c::Ez, mRotation.z.radian());
        quat_0xD80 = qx * qz;

        mRotation.x = mAng::Zero();
        mRotation.z = mAng::Zero();
    }

    mMdl.setPriorityDraw(0x7F, 0x7F);
    if (mBrokenFlag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mBrokenFlag)) {
        return FAILED;
    }

    if (mType == Kraken) {
        mStateMgr.changeState(StateID_KrakenBorn);
    } else {
        mStateMgr.changeState(StateID_Wait);
        getLinkage().set(0x24, 51.f, 0.f, 0.f, nullptr);
    }
    updateMtx();
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
        scale = 2.5f;
    } else {
        scale = 1.f;
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

        field_0xDDC += mPosition.y - mOldPosition.y;
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
        updateMtx();
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
            dJEffManager_c::spawnGroundEffect(mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 0, 1.f, field_0x1B0);
        }

        if (field_0xE0A) {
            startSound(SE_Barrel_ROLL_GROUND);
        }
        field_0xE0A = true;
    } else if (mObjAcch.ChkGndHit()) {
        field_0xDDC = 0.f;
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
            mCyl.ClrAtSet();
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
    field_0xDDC = 0.f;
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
    s32 ry = dAcPy_c::GetLink()->mRotation.y - mRotation.y;

    mQuat_c qy, qz;
    bool z = rotX > 0x4000;
    bool y = mAng(ry).absDiff(0) > 0x4000;

    // Fakematch?
    qy.setAxisRotation(mVec3_c::Ey, mAng::short2rad(y ? mAng(ry) - 0x8000 : ry));
    qz.setAxisRotation(mVec3_c::Ez, mAng::short2rad(z ? 0x4000 : -0x4000));

    quat_0xD60 = qy * qz;
    if (!field_0xE02) {
        quat_0xD70 = quat_0xD50;
    }

    mCyl.SetCo_0x400();
    mCyl.ClrAtSet();

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
            field_0xDDC = 0.f;
        }
    }

    if (dAcPy_c::GetLink()->checkCurrentAction(/* PICK UP */ 0x40)) {
        quat_0xD80.slerpTo(quat_0xD60, 0.2f, quat_0xD80);
        quat_0xD80.normalise();
        quat_0xD70.slerpTo(quat_0xD50, 0.2f, quat_0xD70);
    } else {
        if (!getLinkage().checkConnection(dLinkage_c::CONNECTION_1)) {
            field_0xDDC = 0.f;
            mStateMgr.changeState(StateID_Wait);
        } else {
            mStateMgr.changeState(StateID_Grab);
        }
    }
}
void dAcOBarrel_c::finalizeState_GrabUp() {
    mCyl.ClrCo_0x400();
    mCyl.ClrAtSet();
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
void dumb_rodata_ordering() {
    struct {
        f32 a;
    } a = {-0.f};
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
void dAcOBarrel_c::initializeState_Slope() {
    cM3dGPla tri;
    dBgS::GetInstance()->GetTriPla(mObjAcch.GetGnd(), &tri);

    if (isSpeedStopped()) {
        mAngle.y = tri.GetAngleY();
        mSpeed = 5.f;
    }
    field_0xDEE = tri.GetAngleY();
    field_0xDEE -= mAngle.y;

    mAng ang(mAng::abs(mAngle.y - field_0xDEE));
    field_0xDF0 = nw4r::math::FSqrt(ang.degree2() / 180.f) * 910.f;

    mVec3_c v = mWorldMtx.multVecSR(-mVec3_c::Ez);
    field_0xDEA = mAng::fromVec(v);

    field_0xE09 = false;

    if (dBgS_ObjGndChk::CheckPos(mPosition + mVec3_c::Ey * 100.f)) {
        const dAcObjBase_c *pObj = dBgS::GetInstance()->GetActorPointer(dBgS_ObjGndChk::GetInstance());
        if (dBgS::GetInstance()->ChkMoveBG(dBgS_ObjGndChk::GetInstance(), true)) {
            if (pObj && pObj->mProfileName == fProfile::OBJ_VSD) {
                field_0xE09 = true;
                quat_0xD70.set(1.f, 0.f, 0.f, 0.f);
            }
        }
    }
}
void dAcOBarrel_c::executeState_Slope() {
    if (mObjAcch.ChkGroundLanding() && !field_0xE0E) {
        fn_293_37B0();
        dJEffManager_c::spawnGroundEffect(mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 0, 1.f, field_0x1B0);
        startSound(SE_Barrel_ROLL_GROUND);
    } else if (mObjAcch.ChkGndHit()) {
        field_0xDDC = 0.f;
        mVelocity.y = 0.f;
        fn_293_30D0();

        if (field_0xE02) {
            fn_293_31B0();
        } else {
            fn_293_58C0();
            mVec3_c v = field_0xDBC;
            v.rotY(-mRotation.y);

            if (field_0xE09) {
                v = mVec3_c::Ey;
            }

            quat_0xD70.Set(mVec3_c::Ey, v);
        }

        fn_293_3560();

        if (isSpeedStopped(EGG::Math<f32>::epsilon())) {
            field_0xE00 = false;
        } else if (field_0xE02) {
            field_0xE00 = fn_293_4F80();
        }

        if (!field_0xE08 && !isType_0() && mYOffset < 0.f && fn_293_4BC0()) {
            mSpeed = 0.f;
            field_0xE00 = false;
            fn_293_5910();
        }

        if (mSpeed > 0.f && field_0xE02) {
            holdSoundWithFloatParam(SE_Barrel_ROLL_LV, mSpeed);
            mCyl.OnAtSet();
        } else {
            mCyl.ClrAtSet();
        }

        if (!field_0xE08 && !isType_0() && checkYOffsetField_0x100()) {
            killNoItemDrop();
            return;
        }

        if (!fn_293_4A90(mAng::deg2short(1))) {
            mStateMgr.changeState(StateID_Wait);
            return;
        }
    }

    if (fn_293_4C60()) {
        mStateMgr.changeState(StateID_Water);
    } else if (getLinkage().checkConnection(dLinkage_c::CONNECTION_1)) {
        mStateMgr.changeState(StateID_GrabUp);
    }
}
void dAcOBarrel_c::finalizeState_Slope() {
    field_0xE09 = false;
}

void dAcOBarrel_c::initializeState_Water() {
    field_0xDE4.x = cM::rndInt(0x100) + 0x900;
    field_0xDF2 = cM::rndInt(0x100) + 0x1000;

    mAcceleration *= 0.25f;
    mVelocity.y *= 0.25f;

    mStts.SetRank(9);

    field_0xE16 = 120;

    startSound(SE_Barrel_ROLL_FLOAT);
    startSound(SE_O_FALL_WATER_M);
}
void dAcOBarrel_c::executeState_Water() {
    if (mType == dAcOBarrel_c::Type2 && 0 == sLib::calcTimer(&field_0xE16)) {
        mVelocity.y = nw4r::ut::Max(mVelocity.y + mAcceleration * 0.2f, mMaxSpeed * 0.2f);

        if (mObjAcch.ChkGndHit()) {
            fn_293_4200();
        }
    } else {
        fn_293_5850(&sFloats1, true);

        if (mScale.y > 0.f) {
            mWaterEffect.execute(mObjAcch.GetWtrGroundH(), mObjAcch.GetGroundH());
        }

        if (mType != dAcOBarrel_c::Type2 && nw4r::math::FAbs(field_0xDE0 + mAcceleration) < 0.1f &&
            nw4r::math::FAbs(mVelocity.y + mAcceleration) < 0.1f) {
            mStateMgr.changeState(StateID_Water2);
        } else if (!fn_293_4C60()) {
            mStateMgr.changeState(StateID_Wait);
        }
    }
}
void dAcOBarrel_c::finalizeState_Water() {
    field_0xDF2 = mAng::Zero();
}

void dAcOBarrel_c::initializeState_Water2() {
    mVelocity.y = cM::rndFX(0.1f) + 0.2f;
    field_0xDF2 = cM::rndInt(0x50) + 0x300;
    field_0xDE4.x = cM::rndInt(0x100) + 0x400;
}
void dAcOBarrel_c::executeState_Water2() {
    static f32 sFloat = 1.f;
    fn_293_5850(&sFloat, false);
    if (mScale.y > 0.f) {
        mWaterEffect.execute(mObjAcch.GetWtrGroundH(), mObjAcch.GetGroundH());
    }
    if (!fn_293_4C60()) {
        mStateMgr.changeState(StateID_Wait);
    }
}
void dAcOBarrel_c::finalizeState_Water2() {
    field_0xDF2 = mAng::Zero();
}

void dAcOBarrel_c::initializeState_Explode() {
    setActorProperty(AC_PROP_0x4);
    mSpeed = 0.f;
    mVelocity = mVec3_c::Zero;
    mAcceleration = 0.f;

    mSph.ClrTgSet();
    mSph.ClrCoSet();
    mSph.OnAtSet();

    field_0xE13 = 5;

    f32 dist = mPosition.squareDistance(dAcPy_c::GetLink()->mPosition);
    if (dist < 400.f * 400.f) {
        dRumble_c::start(dRumble_c::sRumblePreset3, dRumble_c::FLAG_SLOT0 | dRumble_c::FLAG_ACTIVE);
    } else if (dist < 800.f * 800.f) {
        dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0 | dRumble_c::FLAG_ACTIVE);
    }
}
void dAcOBarrel_c::executeState_Explode() {
    if (0 == sLib::calcTimer(&field_0xE13)) {
        if (field_0xE07) {
            itemDroppingAndGivingRelated(nullptr, 0);
        }
        deleteRequest();
    }
}
void dAcOBarrel_c::finalizeState_Explode() {}

void dAcOBarrel_c::initializeState_KrakenBorn() {
    mSpeed = 30.f;
    mCyl.OffAtGrp(0x2);

    field_0xE00 = true;
    field_0xE02 = true;
}
void dAcOBarrel_c::executeState_KrakenBorn() {
    if (mObjAcch.ChkGroundLanding()) {
        fn_293_37B0();
        dJEffManager_c::spawnGroundEffect(mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 0, 1.f, field_0x1B0);
        startSound(SE_Barrel_ROLL_GROUND);
    } else if (mObjAcch.ChkGndHit()) {
        field_0xDDC = 0.f;
        mVelocity.y = 0.f;
        if (mSpeed > 0.f) {
            holdSoundWithFloatParam(SE_Barrel_ROLL_LV, mSpeed);
        }
    }

    if (fn_293_4C60()) {
        deleteRequest();
    }
}
void dAcOBarrel_c::finalizeState_KrakenBorn() {}

void dAcOBarrel_c::initializeState_Rebirth() {
    SpecialItemDropMgr::GetInstance()->giveSpecialDropItem(
        getParams2UpperByte(), mRoomID, &mPosition, 0, mRotation.y, -1
    );
    field_0xDC8 = mPosition;
    mpPosition = &field_0xDC8;
    mRotation = mRotationCopy;
    quat_0xD90.set(1.f, 0.f, 0.f, 0.f);
    quat_0xD70.set(1.f, 0.f, 0.f, 0.f);
    quat_0xD80.set(1.f, 0.f, 0.f, 0.f);
    quat_0xDA0.set(1.f, 0.f, 0.f, 0.f);

    mSpeed = 0.f;
    mVelocity.y = 0.f;
    field_0xDDC = 0.f;

    mCyl.ClrCoSet();
    mCyl.ClrTgSet();
    getLinkage().fn_80050EA0(this);

    mStts.SetRank(0);
    setObjectProperty(OBJ_PROP_0x200);

    switch (getParams2UpperByte()) {
        case SPECIAL_ITEM_10BOMBS: mDropItem = 2; break;
        case SPECIAL_ITEM_ARROWS:  mDropItem = 0; break;
        case SPECIAL_ITEM_SEEDS:   mDropItem = 1; break;
    }

    field_0xDFE = 150;
    unsetActorProperty(AC_PROP_0x1);
}
void dAcOBarrel_c::executeState_Rebirth() {
    field_0xDDC = 0.f;

    // Why??
    mPosition.set(mPositionCopy.x, mPositionCopy.y, mPositionCopy.z);
    mOldPosition.set(mPositionCopy.x, mPositionCopy.y, mPositionCopy.z);

    u8 count = 0xFF;
    switch (mDropItem) {
        case 2: count = dAcItem_c::getTotalBombCount(); break;
        case 0: count = dAcItem_c::getTotalArrowCount(); break;
        case 1: count = dAcItem_c::getTotalSeedCount(); break;
    }

    if (checkObjectProperty(OBJ_PROP_0x2) && count <= 3) {
        if (0 == sLib::calcTimer(&field_0xDFE)) {
            mStateMgr.changeState(StateID_Wait);
        }
    } else {
        field_0xDFE = 150;
    }
}
void dAcOBarrel_c::finalizeState_Rebirth() {
    mCyl.OnCoSet();
    mCyl.OnTgSet();
    field_0xE02 = false;
    mpPosition = &mPosition;
    unsetObjectProperty(OBJ_PROP_0x200);
    setActorProperty(AC_PROP_0x1);
    mStts.SetRank(9);
}

void dAcOBarrel_c::fn_293_30D0() {
    if (isSpeedStopped() && field_0xDFC == 0) {
        if (field_0xE05 && mCeFriend.isLinked()) {
            dAcNpcCeFriend_c *pFriend = mCeFriend.get();
            if (pFriend && pFriend->fn_11_2A40()) {
                return;
            }
        }
        AttentionManager *pAttnMgr = AttentionManager::GetInstance();
        if (mYOffset >= 0.f) {
            pAttnMgr->addPickUpTarget(*this, 120.f);
            pAttnMgr->addUnk3Target(*this, 1, 500.f, -200.f, 200.f);
        }
    }
}

void dAcOBarrel_c::fn_293_31B0() {
    cM3dGPla pla;
    dBgS::GetInstance()->GetTriPla(mObjAcch.GetGnd(), &pla);

    mAng nrmAngle = mAng::fromRad(field_0xDBC.angle(pla.GetN()));
    if (nrmAngle > mAng::deg2short(75)) {
        field_0xE06 = true;
    }

    field_0xDBC = pla.GetN();

    // Excuse me?
    mVec3_c _ = mVelocity;

    mVelocity.x += pla.GetN().x * 7.f;
    mVelocity.z += pla.GetN().z * 7.f;
    mVelocity.y = -mVelocity.inprodXZ(pla.GetN()) / pla.GetN().y;

    mSpeed = nw4r::math::FSqrt(mVelocity.squareMagXZ());
    mSpeed = cM::minMaxLimit(mSpeed, -20.f, 20.f);

    if (mAng::fromVec(pla.GetN()).absDiff(mAngle.y) < 0x2000) {
        sLib::addCalcAngle(mAngle.y.ref(), pla.GetAngleY(), 5, mAng::deg2short(10), 0x100);
    } else {
        mAngle.y = mAng::fromVec(mVelocity);
    }

    if (!isSpeedStopped()) {
        mAng ang = pla.GetAngleY();
        s32 target = ang - 0x8000;

        mVec3_c v = mWorldMtx.multVecSR(-mVec3_c::Ez);
        field_0xDEA = mAng::fromVec(v);

        target = mAng(target).absDiff(field_0xDEA) > 0x4000 ? mAng(target) - 0x8000 : target;

        if (field_0xE00 && field_0xE02) {
            field_0xE01 = fn_293_4ED0();
            sLib::addCalcAngle(field_0xDEC.ref(), mAng(target) - field_0xDEA, 10, mAng::deg2short(5), 0x100);

            mQuat_c q;
            q.setAxisRotation(mVec3_c::Ey, field_0xDEC.radian());
            quat_0xD70.slerpTo(q, 0.3f, quat_0xD70);
        }
    }

    if (field_0xE09) {
        quat_0xD70.set(1.f, 0.f, 0.f, 0.f);
    }
}

void dAcOBarrel_c::fn_293_3560() {
    f32 f;
    if (field_0xE11 == 3) {
        f = 0.2f;
    } else {
        f = 0.125f;
    }

    cM3dGPla pla;
    dBgS::GetInstance()->GetTriPla(mObjAcch.GetGnd(), &pla);

    mAng nrmAngle = mAng::fromRad(mVec3_c::Ey.angle(pla.GetN()));

    f *= nrmAngle.cos();
    if (nrmAngle < mAng::deg2short(5) || mAng::fromVec(pla.GetN()).absDiff(mAngle.y) > 0x4000) {
        if (!field_0xE00) {
            f *= 8.f;
        }
        sLib::chase(&mSpeed, 0.f, f + getYOffset() * -0.05f);
    } else {
        if (!field_0xE00) {
            if (mStateMgr.isState(StateID_Slope)) {
                f = -f * 2.f - 1.5f;
            } else {
                f *= -8.f;
            }
        }
        mSpeed = cM::minMaxLimit(mSpeed + f, 0.f, 30.f);
    }
}

void dAcOBarrel_c::fn_293_37B0() {
    mVelocity.y *= -0.5f;

    bool someState;
    if (mType == Type2) {
        someState = field_0xE17 > 2;
    } else {
        someState = mVelocity.y < 8.f;
    }

    if (mType != Type2 || !someState) {
        mSpeed *= 0.95f;
    }

    if (someState) {
        mVelocity.y = 0.f;
        field_0xDE4.x = mAng::Zero();
        field_0xDDC = 0.f;
        field_0xE0E = true;
    } else {
        field_0xE17++;

        field_0xDF2 = cM::rndInt(0x50) + 0x900;
        field_0xDE4.x = cM::rndInt(0x100) + 0x400;

        if (mSpeed < 5.f) {
            fn_293_58C0();
            mAng nrmAngle = mAng::fromRad(field_0xDBC.angle(mVec3_c::Ey));
            mSpeed = nrmAngle.sin() * 12.5f;

            // why is it mType & 2???
            if (isType_1() && field_0xE17 < 2) {
                mSpeed = nw4r::ut::Max(mSpeed, cM::rndFX(5.f) + 25.f);
                field_0xE00 = true;
            }

            mAngle.y = mAng::fromVec(field_0xDBC);
        }
        field_0xE0E = false;
    }

    if (fn_293_4BC0()) {
        field_0xDE4.x = mAng::Zero();
        mSpeed = 0.f;
        mVelocity.y = 0.f;
        field_0xE00 = false;
    }
}

void dAcOBarrel_c::updateMtx() {
    if (!mStateMgr.isState(StateID_Water2)) {
        sLib::addCalcAngle(field_0xDE4.x.ref(), 0, 30, 0x200, 0x20);
    }
    field_0xDE4.y += field_0xDF2;

    getLinkage().fn_800511E0(this);

    mMtx_c m2, m1, m3, m4;

    m1.fromQuat(quat_0xDA0);
    m2.fromQuat(quat_0xD70 * quat_0xD80 * quat_0xD90);

    mWorldMtx += m1;

    m3.transS(0.f, 51.f, 0.f);
    mWorldMtx += m3;

    mWorldMtx.YrotM(field_0xDE4.y);
    mWorldMtx.XrotM(field_0xDE4.x);
    mWorldMtx.YrotM(-field_0xDE4.y);
    mWorldMtx += m2;

    m4.transS(0.f, -51.f, 0.f);
    mWorldMtx += m4;
}

void dAcOBarrel_c::fn_293_3DB0() {
    if (!field_0xE00) {
        return;
    }
    f32 f0, f1;
    f0 = 0.5f * mVelocity.mag();
    f0 = mAng(f0 * 182.f).radian();
    f1 = mAng(mAngle.y - mRotation.y).radian();

    if (field_0xE01) {
        f0 *= -1.f;
    }

    mQuat_c q0, q1, q2;
    q1.setAxisRotation(mVec3_c::Ey, f1);
    q0.setAxisRotation(mVec3_c::Ey, -f1);
    q2.setAxisRotation(mVec3_c::Ey, f0);

    quat_0xD90 = q1 * q2 * q0 * quat_0xD90;
}

extern "C" void fn_800307E0(const mVec3_c &, s32);
void dAcOBarrel_c::fn_293_4200() {
    getLinkage().fn_80050EA0(this);

    if (field_0xE04 || field_0xE05) {
        dAcNpcCeLady_c *pCeLady = mCeLady.get();
        bool b = true;
        if (pCeLady) {
            pCeLady->fn_12_1E80(this, &b);
        }
        if (!b) {
            return;
        }
    }

    startSound(SE_Barrel_BROKEN);

    mVec3_c emitPosition = mPosition;
    emitPosition.y += (field_0xE02 ? 25.f : 50.f);

    if (mBrokenFlag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mBrokenFlag)) {
        SceneflagManager::sInstance->setFlag(mRoomID, mBrokenFlag);
    }

    dLightEnv_c::GetPInstance()->setBPM8_Type6_2(&mPosition);
    if (mType == Bomb) {
        mVec3_c scale = mScale;
        if (getFromParams(8, 0x3) == 0) {
            scale *= 1.5f;
        }

        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_322_, mPosition, nullptr, &scale, nullptr, nullptr, 0, 0
        );
        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_323_, mPosition, nullptr, &scale, nullptr, nullptr, 0, 0
        );
        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_324_, mPosition, nullptr, &scale, nullptr, nullptr, 0, 0
        );
        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_325_, mPosition, nullptr, &scale, nullptr, nullptr, 0, 0
        );
        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_242_, emitPosition, nullptr, &mScale, nullptr, nullptr, 0, 0
        );

        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_474_, emitPosition, nullptr, &mScale, nullptr, nullptr, 0, 0
        )
            ->bindShpEmitter(dJEffManager_c::BarrelBomb, true);

        if (fn_293_4C60()) {
            dJEffManager_c::spawnEffect(
                PARTICLE_RESOURCE_ID_MAPPING_658_, emitPosition, nullptr, &mScale, nullptr, nullptr, 0, 0
            );
        }
        fn_800307E0(mPosition, 0);
        startSound(SE_Barrel_BOMB);
        mStateMgr.changeState(StateID_Explode);

    } else {
        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_243_, emitPosition, nullptr, &mScale, nullptr, nullptr, 0, 0
        );
        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_475_, emitPosition, nullptr, &mScale, nullptr, nullptr, 0, 0
        )
            ->bindShpEmitter(dJEffManager_c::Barrel, true);

        if (field_0xE07) {
            itemDroppingAndGivingRelated(nullptr, 0);
        }

        if (field_0xE0B) {
            mStateMgr.changeState(StateID_Rebirth);
        } else {
            deleteRequest();
        }
    }
}

bool dAcOBarrel_c::fn_293_45A0() {
    if (field_0xE06) {
        return true;
    }

    // Amos -> Armos. A Barrel hitting a Armos will break it
    if (mCyl.ChkAtHit()) {
        if (mCyl.GetAtFlag0x2() || mCyl.GetAtFlag0x4() || mCyl.GetAtActor()->mProfileName == fProfile::E_AM) {
            return true;
        }
    }

    if (mObjAcch.ChkWallHit(nullptr)) {
        if (mAng(mAcchCir.GetWallAngleY()).absDiff(mAngle.y) > mAng::deg2short(70) && mSpeed > 5.f) {
            return true;
        }
        if (mStateMgr.isState(StateID_GrabPut) && dAcPy_c::GetLink()->checkCurrentAction(/* PUT_DOWN_MEDIUM */ 0x42)) {
            mVec3_c put = mVec3_c::Ez * 50.f;
            put.rotY(mAcchCir.GetWallAngleY());

            mVec3_c end = mPosition + put;
            if (dBgS_ObjLinChk::LineCross(&mPosition, &end, this)) {
                return true;
            }
        }
    }

    if (mCyl.ChkTgHit() &&
        mCyl.ChkTgAtHitType(AT_TYPE_SWORD | AT_TYPE_BOMB | AT_TYPE_DAMAGE | AT_TYPE_BUBBLE | AT_TYPE_0x800000)) {
        return true;
    }

    if (mCyl.ChkTgHit() && mCyl.ChkTgAtHitType(AT_TYPE_PHYSICS)) {
        if (mCyl.GetTgActor()->mProfileName == fProfile::OBJ_STEP_STATUE ||
            mCyl.GetTgActor()->mProfileName == fProfile::OBJ_PUSH_BLOCK) {
            return true;
        }
    }

    // Unreachable condition due to first check
    if (mCyl.ChkTgHit() && mCyl.ChkTgAtHitType(AT_TYPE_DAMAGE)) {
        if (mCyl.GetTgActor()->mProfileName == fProfile::E_BC) {
            return true;
        }
    }

    if (!mObjAcch.ChkGndHit()) {
        if (mCyl.ChkCoHit() && !getLinkage().checkState(dLinkage_c::STATE_ACTIVE) &&
            mCyl.GetCoActor()->mTargetFiRelated == 4 && mSpeed > 0.f) {
            return true;
        }
    }

    if (mType == Bomb && (mStateMgr.isState(StateID_Water) || mStateMgr.isState(StateID_Water2))) {
        if (mCyl.ChkCoHit()) {
            dAcObjBase_c *pActor = mCyl.GetCoActor();
            if (pActor->isPlayer() || pActor->mProfileName == fProfile::OBJ_TIME_BOAT) {
                return true;
            }
        }
    }

    if (mObjAcch.ChkGndHit() && mYOffset >= 0.f) {
        if ((!isType_0() && field_0xDDC < -500.f) || (isType_0() && field_0xDDC < -1000.f)) {
            return true;
        }
    }

    return fn_293_4CE0();
}

bool dAcOBarrel_c::fn_293_4A90(const mAng &ang) {
    f32 rad;
    s32 diff;
    cM3dGPla tri;
    dBgS::GetInstance()->GetTriPla(mObjAcch.GetGnd(), &tri);

    rad = mVec3_c::Ey.angle(tri.GetN());
    return ang < mAng::fromRad(rad).absDiff(0);
}

bool dAcOBarrel_c::fn_293_4BC0() {
    int code = dBgS::GetInstance()->GetSpecialCode(mObjAcch.GetGnd());
    return code == POLY_ATTR_LAVA || code == POLY_ATTR_SAND_MED || code == POLY_ATTR_SAND_DEEP_INSTANT ||
           code == POLY_ATTR_SAND_DEEP_SLOW;
}

bool dAcOBarrel_c::fn_293_4C20() const {
    int code = dBgS::GetInstance()->GetSpecialCode(mObjAcch.GetGnd());
    return code == POLY_ATTR_LAVA;
}

bool dAcOBarrel_c::fn_293_4C60() {
    f32 f = mPosition.y - 51.f;
    if (mType == Type2) {
        f = mPosition.y + 51.f * mAng(0); // eh this works
    }

    return mObjAcch.ChkWaterHit() && f < mObjAcch.GetWtrGroundH();
}

bool dAcOBarrel_c::fn_293_4CE0() const {
    return field_0xDDC < -10000.f;
}

bool dAcOBarrel_c::fn_293_4D00() {
    return (
        !getLinkage().checkState(dLinkage_c::STATE_ACTIVE)                                 // ensure not Linked
        && mStateMgr.isState(StateID_Wait)                                                 // Must be on normal ground
        && !field_0xE02 && !field_0xE08                                                    //
        && mYOffset >= 0.f                                                                 //
        && mObjAcch.ChkGndHit() && dBgS::GetInstance()->ChkMoveBG(mObjAcch.GetGnd(), true) //
        && isSpeedStopped() && isVerticalSpeedStopped()                                    // No Movement XZ or Y
        && !fn_293_4A90(mAng::deg2short(0.09f))                                            // Perfectly Flat Ground
    );
}

bool dAcOBarrel_c::fn_293_4ED0() const {
    mVec3_c up = mVec3_c::Ey, v;
    mWorldMtx.multVecSR(up);

    mQuat_c q;
    q.setAxisRotation(up, mAng::short2rad(mAng::deg2short(1)));

    mMtx_c m;
    m.fromQuat(q);

    m.multVecSR(mVelocity, v);
    return (v.y - mVelocity.y) > 0.f;
}

bool dAcOBarrel_c::fn_293_4F80() {
    mVec3_c up = mVec3_c::Ey;
    mWorldMtx.multVecSR(up);

    f32 rad = up.angle(mVelocity);
    mAng ang = mAng::fromRad(rad);
    return mAng::deg2short(75.f) < ang && ang < (mAng::deg2short(105.005f));
}

bool dAcOBarrel_c::fn_293_50B0() {
    return (
        field_0xE00 && field_0xE02 && mSpeed > 15.f && getXZAngleToPlayer().absDiff(mAngle.y) < mAng::deg2short(60.005)
    );
}

void dAcOBarrel_c::fn_293_5150() {
    mVec3_c v = mPosition;
    v.y += field_0xE02 ? 25.f : 50.f;
    mVec3_c scale(0.75f, 0.75f, 0.75f);

    if (mType == Bomb) {
        mTimeArea.check(mRoomID, mPosition, 0, 0.f, 0.1f);

        f32 scaleTarget = getFromParams(8, 0x3) == 0 ? 2.5f : 1.f;
        scale *= scaleTarget;

        if (isWithinPlayerRadius(5800.f)) {
            if (mScale.y == 0.f) {
                if (!field_0xE0C) {
                    dJEffManager_c::spawnEffect(
                        PARTICLE_RESOURCE_ID_MAPPING_465_, v, nullptr, &scale, nullptr, nullptr, 0, 0
                    );
                    startSound(SE_TIMESLIP_TIMESLIP);
                    field_0xE0C = true;
                }
            } else {
                field_0xE0C = false;
            }
            sLib::chase(&mScale.y, scaleTarget, scaleTarget * 0.2f);
        } else {
            if (mScale.y == scaleTarget) {
                if (!field_0xE0D) {
                    dJEffManager_c::spawnEffect(
                        PARTICLE_RESOURCE_ID_MAPPING_465_, v, nullptr, &scale, nullptr, nullptr, 0, 0
                    );
                    startSound(SE_TIMESLIP_TIMESLIP_REV);
                    field_0xE0D = true;
                }
            } else {
                field_0xE0D = false;
            }
            sLib::chase(&mScale.y, 0.f, 0.2f);
        }

        mScale.x = mScale.z = mScale.y;
    }
}
void dAcOBarrel_c::fn_293_5360() {
    if (!field_0xE03) {
        static const s16 s = {0x2000};
        field_0xE03 = fn_8002eff0(120.f, s, &mCyl, 10.f, -10.f);
    }

    if (!field_0xE04 && !field_0xE05) {
        fn_293_54D0();

        if (field_0xE03 && dAcPy_c::GetLink()->checkFlags0x350(0x2000)) {
            field_0xE03 = false;

            if (field_0xDFC != 0) {
                fn_293_4200();
            } else if (mType == Bomb) {
                fn_293_4200();

            } else {
                fn_293_5440();
            }
        }
    }
}

void dAcOBarrel_c::fn_293_5440() {
    field_0xDB0 = dAcPy_c::GetLink()->mVelocity;
    field_0xDB0.y = 0.f;
    field_0xDB0.normalize();
    field_0xDF8.setF(1820.f);
    field_0xDFA.setF(2730.f);
    field_0xDFC = 90;
}

void dAcOBarrel_c::fn_293_54D0() {
    if (0 != sLib::calcTimer(&field_0xDFC)) {
        mMtx_c m;

        mVec3_c vy = mVec3_c::Ey, vx = mVec3_c::Ex;
        mQuat_c qplayer1, qplayer0;

        qplayer0.setAxisRotation(field_0xDB0, mAng(field_0xDF4.sin() * field_0xDF8).radian());
        field_0xDF4 += mAng::deg2short(12);
        if (mAng::abs(field_0xDF4) < mAng::deg2short(12)) {
            field_0xDF8 *= dAcOBarrel_c_Param_c::_12;
        }

        m.makeRotationFromVecs(mVec3_c::Ez, field_0xDB0, 1.f);
        m.multVecSR(vx);

        qplayer1.setAxisRotation(vx, mAng(field_0xDF6.sin() * field_0xDFA).radian());
        field_0xDF6 += mAng::deg2short(20);
        if (mAng::abs(field_0xDF6) < mAng::deg2short(20)) {
            field_0xDFA *= dAcOBarrel_c_Param_c::_9;
        }

        quat_0xDA0 = qplayer0 * qplayer1;

    } else {
        mQuat_c q;
        q.set(1.f, 0.f, 0.f, 0.f);
        quat_0xDA0.slerpTo(q, 0.2f, quat_0xDA0);
    }
}

// FAKEMATCH ??
inline f32 some_calc(f32 acc, f32 gnd, f32 pos) {
    f32 a = (gnd - pos);
    return nw4r::math::FAbs(acc) * a;
}
void dAcOBarrel_c::fn_293_5850(f32 *pMult, bool reduceY) {
    f32 pos = mPosition.y;
    f32 gnd = mObjAcch.GetWtrGroundH();
    field_0xDE0 = some_calc(mAcceleration, gnd, pos) / 50.f;

    mSpeed *= 0.93f;

    if (reduceY) {
        mVelocity.y *= 0.85f;
    }
    mVelocity.y += field_0xDE0 * *pMult;
}

void dAcOBarrel_c::fn_293_58C0() {
    cM3dGPla pla;
    dBgS::GetInstance()->GetTriPla(mObjAcch.GetGnd(), &pla);
    field_0xDBC = pla.GetN();
}

void dAcOBarrel_c::fn_293_5910() {
    static mVec3_c scale(1.5f, 1.5f, 1.5f);

    if (fn_293_4C20()) {
        return;
    }

    mEmitter0.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_515_, mPosition, nullptr, &scale, nullptr, nullptr);
    mEmitter1.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_516_, mPosition, nullptr, &scale, nullptr, nullptr);
}

struct T {
    f32 a, b, c, d, e, f, g, h, i;
    static const T floats;
};
const T T::floats = {50.f, 0.93f, 0.85f, 0.0f, 0.2f, -10000.f, 5.0f, -0.05f, 20.f};
