#include "d/a/obj/d_a_obj_tubo.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_player.h"
#include "d/a/npc/d_a_npc_ce_friend.h"
#include "d/a/npc/d_a_npc_ce_lady.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_log.h"
#include "d/col/bg/d_bg_pc.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/c/c_m3d_g_pla.h"
#include "d/col/cc/d_cc_s.h"
#include "d/flag/dungeonflag_manager.h"
#include "d/flag/sceneflag_manager.h"
#include "egg/math/eggMath.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/math/math_arithmetic.h"
#include "rvl/MTX/mtxvec.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/special_item_drop_mgr.h"

void float_ordering() {
    f32 f[] = {15.f, .4f, .4f, 5.f};
}

#pragma explicit_zero_data on
u8 dAcOtubo_c::sUnk0 = 1;
u32 dAcOtubo_c::sUnk1 = 0;
#pragma explicit_zero_data off

SPECIAL_ACTOR_PROFILE(OBJ_TUBO, dAcOtubo_c, fProfile::OBJ_TUBO, 0x194, 0, 130);

STATE_DEFINE(dAcOtubo_c, Wait);
STATE_DEFINE(dAcOtubo_c, Grab);
STATE_DEFINE(dAcOtubo_c, Put);
STATE_DEFINE(dAcOtubo_c, Slope);
STATE_DEFINE(dAcOtubo_c, Rebirth);

dCcD_SrcSph dAcOtubo_c::sSphSrc = {
    /* mObjInf */
    {/* mObjAt */ {AT_TYPE_PHYSICS, 0x12, {0, 0, 0}, 2, 0, 0, 0, 0, 0},
     /* mObjTg */ {~(AT_TYPE_BUGNET | AT_TYPE_0x80000 | AT_TYPE_0x8000), 0x801111, {0, 0xA, 0x40F}, 8, 0},
     /* mObjCo */ {0x1E9}},
    /* mSphInf */
    {30.f},
};

bool dAcOtubo_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("Tubo"));
    const char *subtype = getSubtype() == 0 ? "Tubo00" : "Tubo01";
    TRY_CREATE(mMdl.create(mRes.GetResMdl(subtype), &heap_allocator, 0x120, 1, nullptr));
    return true;
}

int dAcOtubo_c::actorCreate() {
    CREATE_ALLOCATOR(dAcOtubo_c);
    mObjAcch.Set(this, 1, &mAcchCir);
    mAcchCir.SetWall(20.f, 30.f);
    mEff_0x91C.init(this, 60.f, mScale.x, 0.f);
    mEff_0x91C.setField_0x01(1);
    int unkParam = getParams_0x3000();
    if (unkParam != 1 && unkParam != 2) {
        fn_8002EA30(-60.f, -20.f, 2.f, 1.f, 4.f, &mObjAcch);
    }
    mStts.SetRank(5);
    mSph.Set(sSphSrc);
    mSph.SetStts(mStts);
    mSph.ClrAtSet();

    forwardAccel = -4.f;
    forwardMaxSpeed = -40.f;
    mbMovingForward = 0;
    mField_0x9D8 = 2.f;

    mQuat_0x98C.set(1.f, 0.f, 0.f, 0.f);
    mQuat_0x97C.set(1.f, 0.f, 0.f, 0.f);
    mQuat_0x99C.set(1.f, 0.f, 0.f, 0.f);

    mSceneflag = getSceneflag();
    mSubtype = getSubtype();
    mbField_0x9ED = getParams_0x3000() == 0;
    mbField_0x9EF = getParams_0x3000() == 2;
    if (getParams_0x3000() == 1) {
        mTimer_0x9F4 = 0xF;
        mSph.SetTgInfo_0x2(0);
    }
    mbField_0x9F0 = getParams_0xC000() == 0;

    if (mSceneflag < 0xFF && checkSceneflag()) {
        fillUpperParams2Byte();
    }

    mStateMgr.changeState(StateID_Wait);
    boundingBox.Set(mVec3_c(-100.f, -40.f, -100.f), mVec3_c(100.f, 80.f, 100.f));
    mActorCarryInfo.set(0xC8A0, 28.f, 50.f, 28.f, nullptr); // TODO (ActorCarry Flags)
    mActorCarryInfo.field_0x8C = 26.f;
    mActorCarryInfo.field_0x90 = 46.f;
    mActorCarryInfo.field_0x94 = 28.f;
    mActorCarryInfo.field_0x98 = 0.f;

    return SUCCEEDED;
}

int dAcOtubo_c::actorPostCreate() {
    mCeLady.link((dAcNpcCeLady_c *)fManager_c::searchBaseByProfName(fProfile::NPC_CE_LADY, nullptr));

    if (mbField_0x9EF) {
        if (DungeonflagManager::sInstance->getCounterOrFlag(3, 8) != 0) {
            return FAILED;
        }

        setActorProperty(0x4);

        if (dBgS_ObjGndChk ::CheckPos(position + mVec3_c::Ey * 50.f)) {
            position.y = dBgS_ObjGndChk::GetGroundHeight();
        }
    }

    return SUCCEEDED;
}

int dAcOtubo_c::doDelete() {
    return SUCCEEDED;
}

int dAcOtubo_c::actorExecute() {
    mStateMgr.executeState();
    bool bOffGround = false;
    if (mTimer_0x9F4 != 0) {
        mField_0x9DC = 0.f;

        if (dBgS_ObjGndChk::CheckPos(position + mVec3_c::Ey * 500.f)) {
            if (dBgS_ObjGndChk::GetGroundHeight() - position.y > 5.f) {
                bOffGround = true;
                mSph.ClrTgSet();
            } else if (sLib::calcTimer(&mTimer_0x9F4) == 0) {
                mSph.SetTgInfo_0x2(8);
                mSph.OnTgSet();
                bOffGround = false;
            }
        }
    }
    if (mSph.ChkCoHit()) {
        mObjAcch.OnLineCheck();
    } else {
        mObjAcch.OffLineCheck();
    }

    if (!bOffGround) {
        if (!checkCarryType()) {
            calcVelocity();
            position += velocity;
            position += mStts.GetCcMove();
            mField_0x9DC += position.y - mOldPosition.y;
            adjustRoll();
        }
        mActorCarryInfo.bushTpFunc(mObjAcch);
    }

    if (mStateMgr.isState(StateID_Wait) || mStateMgr.isState(StateID_Slope)) {
        attemptDestroy();
    }
    mSph.SetC(getCenter());
    dCcS::GetInstance()->Set(&mSph);

    mActorCarryInfo.fn_800511E0(this);
    mField_0x8F0.modifyMtx();
    calcRoll();

    mMdl.setLocalMtx(mWorldMtx);
    mMdl.calc(false);
    poscopy2 = position + mVec3_c(0.f, 28.f, 0.f);
    poscopy3 = poscopy2;
    poscopy3.y += 48.f;
    mbField_0x9EB = 0;

    return SUCCEEDED;
}

extern "C" void fn_800A0550(dAcBase_c *);

void dAcOtubo_c::registerInEvent() {
    if (EventManager::isCurrentEvent("HoleIn") || EventManager::isCurrentEvent("HoleOut")) {
        fn_800A0550(this);
    }
}

int dAcOtubo_c::draw() {
    drawModelType1(&mMdl);

    static mQuat_c quat(0.f, 30.f, 0.f, 40.f);

    if (yoffset >= 0.f) {
        drawShadow(mShdw, nullptr, mWorldMtx, &quat, -1, -1, -1, -1, -1, position.y - mObjAcch.GetGroundH());
    }
    return SUCCEEDED;
}

extern "C" void fn_8002A450(const mVec3_c &, u8, u8, const mVec3_c &, int, f32, f32);

void dAcOtubo_c::initializeState_Wait() {}
void dAcOtubo_c::executeState_Wait() {
    if (mObjAcch.ChkGroundLanding()) {
        if (!mbField_0x9EF || !EventManager::isInEvent()) {
            if (mField_0x9F6 == 2) {
                fn_8002A450(position, polyAttr0, polyAttr1, mField_0x1B4, 0, 1.0f, mField_0x1B0);
            }
            if (mbField_0x9F3) {
                playSound(0xA46);
                mbField_0x9F3 = false;
            }
            if (checkOnLava()) {
                if (mField_0x9F6 != 2) {
                    fn_8002A450(position, polyAttr0, polyAttr1, mField_0x1B4, 0, 1.0f, mField_0x1B0);
                }
                playSound(0x9A3);
            }
        }
    } else if (mObjAcch.ChkGndHit()) {
        mObjAcch.SetRoofNone();
        if (mbField_0x9EE) {
            mbField_0x9EE = false;
        }

        addPickupTarget();
        if (checkSlope()) {
            mStateMgr.changeState(StateID_Slope);
            return;
        }

        adjustSpeed();
        if (forwardSpeed < 1.f) {
            mbMovingForward = 0;
            sLib::chase(&forwardSpeed, 0.f, 0.05f);
            mField_0x9D8 = 2.f;
        }

        if (mSph.ChkAtSet()) {
            mSph.ClrAtSet();
        }

        playRollSound();
        if (!mbField_0x9EF) {
            if (dBgS::GetInstance()->ChkMoveBG(mObjAcch.GetGnd(), true)) {
                clearActorProperty(0x1);
            } else {
                setActorProperty(0x1);
            }
        }
        mField_0x9DC = 0.f;
    }
    fn_272_2670();
}
void dAcOtubo_c::finalizeState_Wait() {}

void dAcOtubo_c::initializeState_Grab() {
    velocity = mVec3_c::Zero;
    angle = mAng3_c::Zero;
    mbMovingForward = 0;
    mSph.SetCo_0x400();
    clearActorProperty(0x1);
    if (mbField_0x9ED) {
        mTimer_0x9F7 = 5;
        mbField_0x9EE = true;
    }
    mTimer_0x9F5 = 15;
    mField_0x9DC = 0.f;
}
void dAcOtubo_c::executeState_Grab() {
    if (mbField_0x9ED && sLib::calcTimer(&mTimer_0x9F7) == 0) {
        dAcNpcCeLady_c *lady = mCeLady.get();
        dAcNpcCeFriend_c *ceFriend = mCeFriend.get();
        if (ceFriend && ceFriend->fn_11_17C0(this)) {
            mActorCarryInfo.fn_80050EA0(this);
        } else if (lady && lady->fn_12_1C20(this)) {
            mActorCarryInfo.fn_80050EA0(this);
        }
    }

    if (mActorCarryInfo.checkCarryType(5) && sLib::calcTimer(&mTimer_0x9F5) == 0 &&
        (mObjAcch.ChkGndHit() || mObjAcch.ChkWallHit(nullptr) || mObjAcch.ChkRoofHit())) {
        destroy();
    } else if (dAcPy_c::LINK->getCurrentAction() == 66 /* Put Down Medium */) { // TODO (Link Action ID)
        mStateMgr.changeState(StateID_Put);
    } else {
        if (mActorCarryInfo.isCarried != 4) {
            bool isCarried = false;
            if (mActorCarryInfo.isCarried == 1 && mActorCarryInfo.carryType == 5) {
                isCarried = true;
            }

            if (isCarried) {
                mField_0x9F6 = 0;
            } else {
                mField_0x9F6 = mActorCarryInfo.isCarried;
            }
        }

        if (!checkCarryType()) {
            mStateMgr.changeState(StateID_Wait);
        }
    }
}
void dAcOtubo_c::finalizeState_Grab() {
    mField_0x9DC = 0.f;
    if (forwardSpeed > 0.f) {
        mbMovingForward = 1;
    }
    mField_0x9D4 = cM::rndF(40.f);
    mSph.ClrCo_0x400();
    if ((u8)mActorCarryInfo.isCarried == 2) {
        mSph.OnAtSet();
    }
    mObjAcch.ClrRoofNone();
    mObjAcch.mField_0x0D4 = 50.f;
}

void dAcOtubo_c::initializeState_Put() {}
void dAcOtubo_c::executeState_Put() {
    if (dAcPy_c::LINK->getCurrentAction() == 66 /* Put Down Medium */) { // TODO (Link Action ID)
        mQuat_0x98C.slerpTo(mQuat_0x97C, 0.5f, mQuat_0x98C);
        mQuat_0x98C.normalise();
    }
    if (!checkCarryType()) {
        mStateMgr.changeState(StateID_Wait);
    }
}
void dAcOtubo_c::finalizeState_Put() {
    if (dBgS_ObjGndChk::CheckPos(position + mVec3_c::Ey * 10.f)) {
        mbField_0x9F3 = nw4r::math::FAbs(position.y - dBgS_ObjGndChk::GetGroundHeight()) < 100.f;
    }
    if (!(dBgS_ObjGndChk::CheckPos(position + mVec3_c::Ey * 10.f) &&
          nw4r::math::FAbs(position.y - dBgS_ObjGndChk::GetGroundHeight()) < 100.f)) {
        mSph.OnAtSet();
    }
    forwardSpeed = 0.f;
}

void dAcOtubo_c::initializeState_Slope() {
    cM3dGPla pla;
    dBgS::GetInstance()->GetTriPla(mObjAcch.GetGnd(), &pla);
    if (cM::isZero(forwardSpeed)) {
        angle.y = pla.GetAngleY();
    }
    mAng plaAng = pla.GetAngleY();
    mField_0x9C4 = (plaAng - angle.y);
    mAng other = labs((s16)(angle.y - mField_0x9C4));
    mField_0x9C6 = nw4r::math::FSqrt(other.degree2() / 180.f) * 910.f;
}
void dAcOtubo_c::executeState_Slope() {
    if (mObjAcch.ChkGroundLanding()) {
        fn_8002A450(position, polyAttr0, polyAttr1, mField_0x1B4, 0, 1.0f, mField_0x1B0);
    } else if (mObjAcch.ChkGndHit()) {
        mField_0x9DC = 0.f;
        addPickupTarget();
        if (checkRollHitMaybe()) {
            adjustAngle();
        } else {
            mbMovingForward = 0;
            forwardSpeed = 0.f;
            velocity = mVec3_c::Zero;
            cM3dGPla pla;
            dBgS::GetInstance()->GetTriPla(mObjAcch.GetGnd(), &pla);
            angle.y = cM::atan2s(pla.GetN().x, pla.GetN().z);
        }
        adjustSpeed();
        if (mSph.ChkAtSet()) {
            mSph.ClrAtSet();
        }
        playRollSound();
        if (!checkSlope()) {
            mStateMgr.changeState(StateID_Wait);
            return;
        }
    }

    fn_272_2670();
}
void dAcOtubo_c::finalizeState_Slope() {}

void dAcOtubo_c::initializeState_Rebirth() {
    SpecialItemDropMgr *mgr = SpecialItemDropMgr::GetInstance();
    mgr->giveSpecialDropItem(getParams2UpperByte(), roomid, &position, 0, rotation.y, -1);
    mField_0x9AC = position;
    obj_pos = &mField_0x9AC;
    mField_0x9F6 = 0;
    rotation = rot_copy;

    mQuat_0x98C.set(1.f, 0.f, 0.f, 0.f);

    forwardSpeed = 0.f;
    velocity.y = 0.f;
    mField_0x9DC = 0.f;

    mSph.ClrCoSet();
    mSph.ClrTgSet();
    setObjectProperty(0x200);
    mActorCarryInfo.fn_80050EA0(this);

    int item_drop_table = getParams2UpperByte();
    switch (item_drop_table) {
        default:              break;
        case SPECIAL_ITEM_11: mField_0x9FC = 2; break;
        case SPECIAL_ITEM_12: mField_0x9FC = 0; break;
        case SPECIAL_ITEM_13: mField_0x9FC = 1; break;
    }
    mTimer_0x9E8 = 150;
    clearActorProperty(0x1);
}
void dAcOtubo_c::executeState_Rebirth() {
    setPostion(pos_copy);
    mOldPosition = pos_copy;
    u8 count = -1;
    switch (mField_0x9FC) {
        case 2: count = dAcItem_c::getTotalBombCount(); break;
        case 0: count = dAcItem_c::getTotalArrowCount(); break;
        case 1: count = dAcItem_c::getTotalSeedCount(); break;
    }
    if (checkObjectProperty(0x2) && count <= 3) {
        if (sLib::calcTimer(&mTimer_0x9E8) == 0) {
            mStateMgr.changeState(StateID_Wait);
        }
    } else {
        mTimer_0x9E8 = 150;
    }
}
void dAcOtubo_c::finalizeState_Rebirth() {
    mSph.OnCoSet();
    mSph.OnTgSet();
    obj_pos = &position;
    clearObjectProperty(0x200);
    setActorProperty(0x1);
}

extern "C" void fn_80027510(void *, bool);
extern "C" void fn_80027560(void *, bool, int);
extern "C" void *fn_800298B0(u16, mVec3_c *, mVec3_c *, u32, u32, u32, u32, u32);
extern "C" u16 PARTICLE_RESOURCE_ID_MAPPING_109_, PARTICLE_RESOURCE_ID_MAPPING_209_;
extern "C" void *ENVIRONMENT;
extern "C" void fn_80022BE0(void *, const mVec3_c &);

void dAcOtubo_c::destroy() {
    dAcNpcCeLady_c *lady = mCeLady.get();
    bool boolParam = true;
    if (lady) {
        lady->fn_12_1E00(this, &boolParam);
    }
    if (!boolParam) {
        return;
    }
    fn_80022BE0(ENVIRONMENT, position);
    mActorCarryInfo.fn_80050EA0(this);

    void *fx_thing = fn_800298B0(PARTICLE_RESOURCE_ID_MAPPING_209_, &poscopy2, nullptr, 0, 0, 0, 0, 0);
    if (fx_thing) {
        fn_80027510(fx_thing, mSubtype != 0);
    }
    fx_thing = fn_800298B0(PARTICLE_RESOURCE_ID_MAPPING_109_, &position, nullptr, 0, 0, 0, 0, 0);
    if (fx_thing) {
        fn_80027560(fx_thing, mSubtype != 0, 0);
    }

    playSound(0xA47); // TODO (Sound ID)

    if (mSceneflag < 0xFF && !checkSceneflag()) {
        SceneflagManager::sInstance->setFlag(roomid, mSceneflag);
    }

    if (mbField_0x9F0) {
        mStateMgr.changeState(StateID_Rebirth);
    } else {
        deleteRequest();
    }
}

void dAcOtubo_c::calcRoll() {
    bool onLog = checkOnLog_0xE4E();
    if (mbMovingForward || onLog) {
        if (onLog) {
            velocity = position - mOldPosition;
            forwardSpeed = EGG::Math<f32>::sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
            angle.y = cM::atan2s(velocity.x, velocity.z);
        }
        f32 vel_mag = VEC3Len(velocity);
        f32 f1 = mAng(vel_mag * (mField_0x9D4 + 200.f)).radian();
        f32 f2 = mAng(vel_mag * 182.0f * 0.2f).radian();
        f32 f0 = mAng(angle.y - rotation.y).radian();

        mQuat_c q0, q1, q2, q3;
        q1.setAxisRotation(mVec3_c::Ey, f0);
        q0.setAxisRotation(mVec3_c::Ey, -f0);
        q3.setAxisRotation(mVec3_c::Ex, f1);
        q2.setAxisRotation(mVec3_c::Ey, f2);

        mQuat_0x98C = q1 * q3 * q2 * q0 * mQuat_0x98C;

        if (onLog) {
            velocity = mVec3_c::Zero;
            forwardSpeed = 0.f;
        }
    }

    mMtx_c mtx0, mtx1, mtx2;
    mtx0.fromQuat(mQuat_0x99C * mQuat_0x98C);
    mtx1.transS(0.f, 28.f, 0.f);
    mWorldMtx += mtx1;
    mWorldMtx += mtx0;
    mtx2.transS(0.f, -28.f, 0.f);
    mWorldMtx += mtx2;
}

void dAcOtubo_c::adjustRoll() {
    if (!mObjAcch.ChkGndHit()) {
        return;
    }
    if (cM::isZero(forwardSpeed)) {
        return;
    }

    s32 adj = forwardSpeed * 182;

    mVec3_c vel = velocity;
    vel.y = 0.f;

    vel.normalize();
    vel.rotY(0x4000);
    vel *= mField_0x9D8 * mField_0x9C8.sin();

    mAng old_9C8 = mField_0x9C8;
    mField_0x9C8 += adj;

    if (old_9C8.sin() * mField_0x9C8.sin() < 0.f) {
        mField_0x9D8 *= 0.75f;
    }

    position += vel;
}

void dAcOtubo_c::fn_272_2670() {
    if (mbField_0x9EF && EventManager::isInEvent()) {
        mField_0x9DC = 0.f;
        return;
    }

    if (mSph.ChkTgHit()) {
        if (mSph.ChkTgAtHitType(AT_TYPE_BELLOWS | AT_TYPE_WIND)) {
            mField_0x9DC = 0.f;
            if (mTimer_0x9F4 == 0) {
                if (!mbField_0x9EB) {
                    mField_0x9D0 = cM::rndFX(0.25f) + 1.f;
                }
                mbField_0x9EB = true;
                fn_272_2E60(mSph.GetTgAtHitDir());
                return;
            }
        }
    } else {
        if (mbField_0x9F1 && mField_0x8F0.fn_80051780(mSph)) {
            velocity += mField_0x9B8;
            mbField_0x9F1 = false;
        }
        static const s16 unk = {0}; // needed for rodata ordering
        if (!cM::isZero(mField_0x9CA)) {
            angle.y = (*(s16 *)((u8 *)this + 0x9CA)); // HACK to force load again
            mField_0x9CA = 0;
        } else {
            mQuat_c q;
            q.set(1.f, 0.f, 0.f, 0.f);
            mQuat_0x99C.slerpTo(q, 0.2f, mQuat_0x99C);
        }
    }

    if (mObjAcch.ChkGndHit()) {
        if (yoffset >= 0.f && !checkCarryType()) {
            mField_0x9DC = 0.f;
        } else if (checkInvalidGround()) {
            forwardSpeed = 0.0f;
            mbMovingForward = 0;
        }
    }

    if (mSph.ChkCoHit()) {
        if (fn_272_38C0()) {
            fn_272_3020();
        }
    } else if (cM::isZero(mField_0x9E0)) {
        mField_0x9E0 = 0.1f;
    }

    // the ordering is weird here
    f32 groundH = mObjAcch.GetGroundH();
    f32 waterH = mObjAcch.GetWtrGroundH();
    bool noSound = mbSubmerged;
    mEff_0x91C.fn_8002B120(waterH, groundH);

    if (checkSubmerged()) {
        forwardAccel = -0.8f;
        forwardMaxSpeed = -7.f;
        mField_0x9DC = 0.f;
        cLib::addCalcPosXZ(&velocity, mVec3_c::Zero, 0.05f, 1.0f, 0.2f);
        forwardSpeed = EGG::Math<f32>::sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
        if (!noSound) {
            playSound(0x9A0); // TODO (Sound ID) - Fall Water S
        }
        mbSubmerged = true;
    } else {
        forwardAccel = -4.f;
        forwardMaxSpeed = -40.f;
        mbSubmerged = false;
        if (checkCarryType()) {
            mStateMgr.changeState(StateID_Grab);
        }
    }
}

void dAcOtubo_c::attemptDestroy() {
    if (mbField_0x9EF && mSph.ChkCoHit() && mSph.GetCoActor()->profile_name == fProfile::B_MG) {
        destroy();
        return;
    }

    if (mObjAcch.ChkWallHit(nullptr) && sLib::absDiff(mAcchCir.GetWallAngleY(), angle.y) > mAng::deg2short(70.f)) {
        attemptDestroyOnWall(&sUnk1, &sUnk0);
        angle.y = mAcchCir.GetWallAngleY();
        forwardSpeed *= 0.5f;
        return;
    }

    if (mSph.ChkTgHit() && ((mSph.ChkTgAtHitType(AT_TYPE_BEETLE) && mSph.ChkTgBit14()) ||
                            !mSph.ChkTgAtHitType(AT_TYPE_BEETLE | AT_TYPE_BELLOWS | AT_TYPE_WHIP | AT_TYPE_WIND))) {
        destroy();
        return;
    }

    if (mObjAcch.ChkGndHit() && yoffset >= 0.f && !checkCarryType()) {
        if ((mField_0x9DC < -100.f && !mbField_0x9EE) || fn_272_3660()) {
            destroy();
            return;
        }
        mField_0x9DC = 0.f;
    }
    if (!mObjAcch.ChkGndHit() && mSph.ChkCoHit()) {
        if (mActorCarryInfo.isCarried != 1 && forwardSpeed > 0.f) {
            if (mSph.GetCoActor()->unkByteTargetFiRelated == 4) {
                destroy();
                return;
            }
        }
    }
    if (mSph.ChkCoHit()) {
        dAcObjBase_c *obj = mSph.GetCoActor();
        if (obj->isPlayer() &&
            static_cast<dAcPy_c *>(obj)->getCurrentAction() == 0xC /* ROLL */) { // TODO (Player Action ID)
            destroy();
        }
    }

    if (mObjAcch.ChkRoofHit()) {
        destroy();
    } else if (!mbField_0x9EF && checkYOffsetField_0x100() && getParams_0x3000() != 1) {
        killNoItemDrop();
    } else if (fn_272_38A0()) {
        destroy();
    }
}

void dAcOtubo_c::attemptDestroyOnWall(u32 *, const u8 *unk) {
    if (*unk && sLib::absDiff(mAcchCir.GetWallAngleY(), angle.y) > mAng::deg2short(70.f) && 15.f < forwardSpeed) {
        destroy();
    }
}

mVec3_c dAcOtubo_c::getCenter() const {
    mMtx_c m;
    m.ZXYrotS(rotation.x, rotation.y, rotation.z);
    mVec3_c dir;
    PSMTXMultVecSR(m, mVec3_c::Ey, dir);

    return position + dir * 28.f;
}

void dAcOtubo_c::fn_272_2E60(const mVec3_c &vel) {
    if (mField_0x8F0.fn_80051780(mSph)) {
        if (mSph.ChkCoHit()) {
            position += mStts.GetCcMove();
            mField_0x8F0.fn_800051630();
        }
        forwardSpeed = 0.f;
        velocity = mVec3_c::Zero;
        mbField_0x9F1 = 1;
        mField_0x9B8 = vel * 2.f;
    } else if (mSph.ChkTgAtHitType(AT_TYPE_BELLOWS)) {
        velocity.x += vel.x * 0.06f;
        velocity.y += vel.y * 0.2f;
        velocity.z += vel.z * 0.06f;
        mField_0x9CA = cM::atan2s(vel.x, vel.z);
        forwardSpeed = mField_0x9D0 * EGG::Math<f32>::sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
    }

    mbMovingForward = 1;
}

void dAcOtubo_c::fn_272_3020() {
    static const s16 ang_inc = {0x2000}; // needed for rodata ordering

    mbField_0x9EB = 1;
    static mVec3_c sRot = mVec3_c::Ey;

    mQuat_c quat;
    mVec3_c rot = mVec3_c::Ey + mVec3_c(0.f, 0.f, 1.f) * mField_0x9E0;

    rot.normalize();
    rot.rotY(mField_0x9CC);
    mField_0x9CC += 0x2000;

    quat.makeVectorRotation(sRot, rot);
    sLib::chase(&mField_0x9E0, 0.f, 0.005f);
    mQuat_0x99C.slerpTo(quat, 0.5f, mQuat_0x99C);
}

void dAcOtubo_c::addPickupTarget() {
    if (cM::isZero(forwardSpeed)) {
        AttentionManager *ins = AttentionManager::GetInstance();
        ins->addPickUpTarget(*this, 120.f);
        ins->addUnk3Target(*this, 1, 500.f, -200.f, 200.f);
    }
}

void dAcOtubo_c::adjustAngle() {
    cM3dGPla pla;
    dBgS::GetInstance()->GetTriPla(mObjAcch.GetGnd(), &pla);

    // ??
    f32 mult = 1.f;
    velocity.x += pla.mNormal.x * mult;
    velocity.z += pla.mNormal.z * mult;

    velocity.y = -(velocity.x * pla.GetN().x + velocity.z * pla.GetN().z) / pla.GetN().y;
    forwardSpeed = nw4r::math::FSqrt(velocity.x * velocity.x + velocity.z * velocity.z);
    forwardSpeed = cM::minMaxLimit(forwardSpeed, -30.f, 30.f);

    mAng a = mAng::fromVec(pla.GetN()); // Probably mean to be angle from Ey
    if (sLib::absDiff(a, angle.y) < mAng::deg2short(90.f)) {
        sLib::addCalcAngle(angle.y.ref(), pla.GetAngleY(), 5, 0x71C, 0x100);
    } else {
        angle.y = mAng::fromVec(velocity);
    }
}

void dAcOtubo_c::adjustSpeed() {
    cM3dGPla pla;
    dBgS::GetInstance()->GetTriPla(mObjAcch.mGnd, &pla);
    f32 step = 0.4f;
    mAng gndAngle = mAng::angle(mVec3_c::Ey, pla.GetN());
    step *= gndAngle.cos();

    if (gndAngle < mAng::deg2short(5) ||
        sLib::absDiff(cM::atan2s(pla.mNormal.x, pla.mNormal.z), angle.y) > mAng::deg2short(90)) {
        f32 stepSize = yoffset;
        sLib::chase(&forwardSpeed, 0.f, step + stepSize * -0.05f);
    } else {
        if (!mbMovingForward) {
            step *= -1.f;
        }
        forwardSpeed = cM::minMaxLimit(forwardSpeed + step, 0.f, 30.f);
    }

    mbMovingForward = forwardSpeed > 0.f;
}

bool dAcOtubo_c::checkSlope() {
    cM3dGPla pla;
    dBgS::GetInstance()->GetTriPla(mObjAcch.mGnd, &pla);
    mAng gndAngle = mAng::angle(mVec3_c::Ey, pla.GetN());

    return sLib::absDiff(gndAngle, 0) > mAng::deg2short(5);
}

bool dAcOtubo_c::fn_272_3660() {
    int poly_code = dBgS::GetInstance()->GetSpecialCode(mObjAcch.GetGnd());

    return mField_0x9F6 == 2 && !mStateMgr.isState(StateID_Grab) && !checkSubmerged() && poly_code != POLY_ATTR_LAVA;
}

bool dAcOtubo_c::checkInvalidGround() {
    int poly_code = dBgS::GetInstance()->GetSpecialCode(mObjAcch.GetGnd());

    return poly_code == POLY_ATTR_LAVA || poly_code == POLY_ATTR_SAND_MED || poly_code == POLY_ATTR_SAND_DEEP_INSTANT ||
           poly_code == POLY_ATTR_SAND_DEEP_SLOW;
}

bool dAcOtubo_c::checkOnLava() {
    int poly_code = dBgS::GetInstance()->GetSpecialCode(mObjAcch.GetGnd());

    return poly_code == POLY_ATTR_LAVA;
}

bool dAcOtubo_c::checkCarryType() const {
    return mActorCarryInfo.checkCarryType(1) || mActorCarryInfo.checkCarryType(7) || mActorCarryInfo.checkCarryType(5);
}

bool dAcOtubo_c::checkSubmerged() {
    return mObjAcch.ChkWaterIn() && position.y + 28.f < mObjAcch.mWtr.GetGroundH();
}

bool dAcOtubo_c::fn_272_38A0() {
    return mField_0x9DC < -30000.f;
}

bool dAcOtubo_c::fn_272_38C0() {
    return mSph.ChkCoBit4();
}

bool dAcOtubo_c::checkOnLog_0xE4E() const {
    if (mObjAcch.ChkGndHit() && dBgS::GetInstance()->ChkMoveBG(mObjAcch.mGnd, true)) {
        const dAcObjBase_c *obj = dBgS::GetInstance()->GetActorPointer(mObjAcch.mGnd);
        if (obj && obj->profile_name == fProfile::OBJ_LOG) {
            return static_cast<const dAcOlog_c *>(obj)->getField_0xE4E() >= 4;
        }
    }
    return false;
}

bool dAcOtubo_c::checkRollHitMaybe() {
    if (15.f < forwardSpeed) {
        return true;
    }
    if (mObjAcch.ChkWallHit(nullptr)) {
        mVec3_c a = mVec3_c::Ez * 60.f;
        mVec3_c start = getCenter();
        a.rotY(mAng::fromVec(velocity));
        mVec3_c end = start + a;
        if (dBgS_ObjLinChk::LineCross(&start, &end, this)) {
            return false;
        }
    }
    return true;
}

void dAcOtubo_c::playRollSound() {
    if (!(forwardSpeed > 0.f)) {
        return;
    }

    if (checkSubmerged()) {
        return;
    }

    FUN_8002d770(0xA48, forwardSpeed);
}

void float_order() {
    const f32 arr[] = {-30000.f, -0.05f, 30.f};
}
