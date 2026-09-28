#include "d/a/obj/d_a_obj_girahimu_knife.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/b/d_a_b_girahimu2.h"
#include "d/a/d_a_player.h"
#include "d/a/e/d_a_en_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_sphere.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/math/math_types.h"
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"

static dCcD_SrcSph sSrcSph = {
    /* mObjInf */
    {/* mObjAt */ {AT_TYPE_DAMAGE, 0xD, {9, 0, 0}, 2, 0, 0, 0, CUT_DIR_NONE, 0},
     /* mObjTg */ {~(AT_TYPE_COMMON0 & ~AT_TYPE_WIND), 0x111, {9, 0x18, 0x40F}, 0, CUT_DIR_NONE},
     /* mObjCo */ {0x25}},
    /* mSphInf */
    {30.0f}
};

SPECIAL_ACTOR_PROFILE(OBJ_GH_KNIFE, dAcObjGirahimuKnife_c, fProfile::OBJ_GH_KNIFE, 0x115, 0, 2);

s32 dAcObjGirahimuKnife_c::sSomething0;
s32 dAcObjGirahimuKnife_c::sSomething0_1;
mAng dAcObjGirahimuKnife_c::sSomething1;
s16 dAcObjGirahimuKnife_c::sSomething2;

STATE_DEFINE(dAcObjGirahimuKnife_c, Wait);
STATE_DEFINE(dAcObjGirahimuKnife_c, SpinWait);
STATE_DEFINE(dAcObjGirahimuKnife_c, SpinWaitPreAttack);
STATE_DEFINE(dAcObjGirahimuKnife_c, SpinFreeWait);
STATE_DEFINE(dAcObjGirahimuKnife_c, FreeWait);
STATE_DEFINE(dAcObjGirahimuKnife_c, Attack);
STATE_DEFINE(dAcObjGirahimuKnife_c, AttackEnd);
STATE_DEFINE(dAcObjGirahimuKnife_c, Return);
STATE_DEFINE(dAcObjGirahimuKnife_c, Hit);
STATE_DEFINE(dAcObjGirahimuKnife_c, CircleWait);

bool dAcObjGirahimuKnife_c::createHeap() {
    nw4r::g3d::ResFile res(nullptr);
    res = nw4r::g3d::ResFile(getOarcResFile("GirahimKnife"));
    TRY_CREATE(mMdl.create(res.GetResMdl("GirahimKnifeA"), &mAllocator, 0x20, 1, nullptr));
    return true;
}

int dAcObjGirahimuKnife_c::create() {
    CREATE_ALLOCATOR(dAcObjGirahimuKnife_c);
    init();
    changeState(StateID_Wait);
    return SUCCEEDED;
}

void dAcObjGirahimuKnife_c::init() {
    sSomething2 = 1;
    field_0x793 = true;
    field_0x750.set(0.0f, 0.0f, 0.0f);
    mAcceleration = 0.0f;
    field_0x792 = true;
    mStts.SetRank(12);
    mMaxSpeed = -6000.0f;
    mBoundingBox.Set(mVec3_c(-100.0f, -100.0f, -100.0f), mVec3_c(100.0f, 100.0f, 100.0f));
    field_0x75C.set(1.2f, 0.0f, 1.2f);
    field_0x774 = 20.0f;

    mCollider.addCc(mSph1, sSrcSph);
    mCollider.addCc(mSph0, sSrcSph);
    mCollider.SetStts(mStts);

    mSph1.SetR(15.0f);
    mSph0.SetR(30.0f);

    mSph1.OnAtSet();
    mSph1.ClrTgSet();

    mSph0.ClrAtSet();
    mSph0.OnTgSet();

    mSph0.SetTg_0x50(2);
    dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_685_, mPosition, nullptr, nullptr, nullptr, nullptr, 0, 0);
}

int dAcObjGirahimuKnife_c::actorExecute() {
    field_0x768.set(field_0x75C);
    if (--field_0x6D0 <= 0) {
        field_0x6D0 = 0;
    }

    executeState();

    mVelocity.y -= mAcceleration;
    mPosition += mVelocity;

    mSph1.SetAtVec(mVelocity);

    mSph1.SetC(mPosition);
    mSph0.SetC(mPosition);
    mCollider.registerColliders();

    updateMdlMatrix();
    startEmitters();

    if (mPosition.y <= -1000.0f) {
        deleteRequest();
    }

    mVec3_c compare(1.2f, 1.2f, 1.2f);
    if (field_0x768 == compare && field_0x75C != compare) {
        startSound(SE_OGhKf_DISAPPEAR);
        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_685_, mPosition, nullptr, nullptr, nullptr, nullptr, 0, 0
        );
    }
    return SUCCEEDED;
}

int dAcObjGirahimuKnife_c::draw() {
    static mSphere_c sph(mVec3_c(0.0f, 60.0f, 0.0f), 100.0f);
    f32 f = mPosition.y;
    drawModelType1(&mMdl);
    fn_8002edb0(mShadow, mMdl, &sph, -1, -1, f);
    return SUCCEEDED;
}

void dAcObjGirahimuKnife_c::initializeState_Wait() {
    field_0x6F6.x = cM::rndInt(64000);
    field_0x6F6.z = cM::rndInt(64000);

    field_0x778 = cM::rndInt(32000);
    field_0x774 = 20.0f;
}
void dAcObjGirahimuKnife_c::executeState_Wait() {
    if (!dAcPy_c::GetLink()->isAttacking()) {
        mSph0.ClrTgSet();
    } else {
        mSph0.OnTgSet();
    }

    if (mState == 4) {
        changeState(StateID_SpinWait);
    } else if (mState == 5) {
        changeState(StateID_SpinFreeWait);
    } else if (mState == 6) {
        changeState(StateID_CircleWait);
    } else if (mState == 8) {
        changeState(StateID_FreeWait);
    } else {
        fn_239_2B10();
        fn_239_1DB0(field_0x6D8, field_0x744);
        field_0x778 += 0x500;
        fn_239_1C40();
    }
}
void dAcObjGirahimuKnife_c::finalizeState_Wait() {
    mSph0.OnTgSet();
    field_0x714.toQuat(field_0x704);
    mSph0.SetTg_0x4C(0);
}

void dAcObjGirahimuKnife_c::initializeState_Attack() {
    mSph1.OnAtSet();
    mSph1.ClrCoSet();
    mSph0.ClrCoSet();

    mAcceleration = 0.0f;
    field_0x78C = true;

    if (mState != 6) {
        sSomething2 = 1;
    }

    if (sSomething2 == 0 || !field_0x793) {
        mSph1.ClrAtSet();
    }
}
void dAcObjGirahimuKnife_c::executeState_Attack() {
    holdSound(SE_OGhKf_FLY_LV);

    if (sSomething2 == 0 || !field_0x793) {
        mSph1.ClrAtSet();
    }
    fn_239_2B10();
    sLib::chase(&field_0x774, 0.0f, 2.0f);
    field_0x778 += 0x700;
    if (lineCheck()) {
        changeState(StateID_AttackEnd);
        return;
    }

    if (field_0x790) {
        mVec3_c v0 = dAcPy_c::GetLink()->mPosition;
        if (field_0x791) {
            mVec3_c mod(field_0x744.x, field_0x744.y, 0.0f);
            mod.rotY(dAcPy_c::GetLink()->mRotation.y + 0x8000);
            v0 += mod;
        } else {
            v0.y += 100.0f;
        }

        mVec3_c diff = v0 - mPosition;
        if (diff.dot(mVelocity) > 0.0f) {
            sLib::chaseAngle(mAngle.y.ref(), diff.atan2sX_Z(), 0x100);
            mVec3_c vel(0.0f, mVelocity.y, 60.0f);
            vel.rotY(mAngle.y);
            mVelocity.set(vel);
        }
    }
    mVec3_c norm = mVelocity;
    norm.normalizeRS();
    mQuat_c q;
    q.slerp(mVec3_c(0.0f, 0.0f, 1.0f), norm, 1.0f);
    field_0x704.slerpTo(q, 0.5f, field_0x704);
    field_0x714.fromQuat(field_0x704);
}
void dAcObjGirahimuKnife_c::finalizeState_Attack() {}

void dAcObjGirahimuKnife_c::initializeState_AttackEnd() {
    field_0x6D0 = 0x3C;
    mSph1.ClrAtSet();
    mSph0.ClrTgSet();
}
void dAcObjGirahimuKnife_c::executeState_AttackEnd() {
    mVelocity.set(0.0f, 0.0f, 0.0f);
    mAcceleration = 0.0f;
    if (field_0x6D0 <= 0 && sLib::chase(&field_0x75C.y, 0.0f, 0.1f)) {
        deleteRequest();
    }
}
void dAcObjGirahimuKnife_c::finalizeState_AttackEnd() {}

void dAcObjGirahimuKnife_c::initializeState_Return() {
    mSph1.SetAt_0xE(0);
    mAcceleration = 1.0f;

    mVec3_c v0 = field_0x6D8;
    mVec3_c v1 = mPosition - field_0x6D8;
    mVec3_c pos(cM::rndFX(50.0f), 100.0f + cM::rndFX(50.0f), -100.0f);
    pos.rotY(v1.atan2sX_Z());

    v0 += pos;
    f32 speed = (v1.absXZ() / 500.0f);
    speed *= 4.0f;
    calcVelocity(v0, speed);
    mSph0.ClrTgSet();
}
void dAcObjGirahimuKnife_c::executeState_Return() {
    mSph1.SetAtFlag2(2);
    mSph1.OnAtSet();
    mSph0.ClrTgSet();
    mSph1.SetR(50.0f);
    if (lineCheck()) {
        changeState(StateID_AttackEnd);
    } else if (mSph1.ChkAtHit()) {
        deleteRequest();
    } else {
        mVec3_c norm = mVelocity;
        norm.normalizeRS();
        mQuat_c q;
        q.slerp(mVec3_c(0.0f, 0.0f, 1.0f), norm, 1.0f);
        field_0x704.slerpTo(q, 0.5f, field_0x704);
        field_0x714.fromQuat(field_0x704);
    }
}
void dAcObjGirahimuKnife_c::finalizeState_Return() {}

void dAcObjGirahimuKnife_c::initializeState_Hit() {
    mAcceleration = 1.0f;
    mSph1.ClrAtSet();
    if (mState == 8) {
        mSph1.ClrAtSet();
        mSph0.ClrTgSet();
    }
}
void dAcObjGirahimuKnife_c::executeState_Hit() {
    if (lineCheck()) {
        changeState(StateID_AttackEnd);
        return;
    }
    mVec3_c norm = mVelocity;
    norm.normalizeRS();
    mQuat_c q;
    f32 f = 20.0f; // Forces rodata pool
    f = 1.0f;
    q.slerp(mVec3_c(0.0f, 0.0f, 1.0f), norm, f);
    field_0x704.slerpTo(q, 0.5f, field_0x704);
    field_0x714.fromQuat(field_0x704);
}
void dAcObjGirahimuKnife_c::finalizeState_Hit() {}

void dAcObjGirahimuKnife_c::initializeState_SpinWait() {
    field_0x6F6.x = cM::rndInt(64000);
    field_0x6F6.z = cM::rndInt(64000);
    field_0x778 = cM::rndInt(32000);
    mSph0.SetTg_0x4C(-1);
    field_0x774 = 10.0f;
}
void dAcObjGirahimuKnife_c::executeState_SpinWait() {
    mSph1.ClrAtSet();
    mSph0.SetR(50.0f);
    field_0x78F = true;

    if (checkCutDir(dAcPy_c::GetLink()->getSpecificAttackDirection())) {
        mSph0.OnTgSet();
    } else {
        mSph0.ClrTgSet();
    }

    fn_239_2B10();
    if (mSph0.ChkTgHit()) {
        dAcEnBase_c *pEn = FindEnemy(fProfile::B_GIRAHIMU2);
        if (pEn != nullptr) {
            static_cast<dAcGirahimu2_c *>(pEn)->setField_0xD52(120);
        }
    }
    fn_239_1DB0(field_0x6D8, field_0x744);
    field_0x778 += 0x500;
    fn_239_1C40();
    if (field_0x792) {
        holdSound(SE_OGhKf_SPIN_LV);
    }
}
void dAcObjGirahimuKnife_c::finalizeState_SpinWait() {
    mSph0.OnTgSet();
    field_0x714.toQuat(field_0x704);
    mSph0.SetTg_0x4C(0);
}

void dAcObjGirahimuKnife_c::initializeState_SpinWaitPreAttack() {
    mSph0.SetTg_0x4C(-1);
    field_0x744.x *= 1.5f;
    field_0x744.z = 50.0f;
    field_0x792 = false;
}
void dAcObjGirahimuKnife_c::executeState_SpinWaitPreAttack() {
    if (!dAcPy_c::GetLink()->isAttacking()) {
        mSph0.ClrTgSet();
    } else {
        mSph0.OnTgSet();
    }

    field_0x778 += 0x500;
    if (field_0x78E) {
        mSph1.ClrCoSet();
        mSph0.ClrCoSet();
        if (sLib::chase(&field_0x75C.y, 0.0f, 0.1f)) {
            deleteRequest();
        }
    } else {
        sLib::chase(&field_0x75C.y, 1.2f, 0.1f);
    }
    mSph1.ClrAtSet();
    mSph0.SetR(50.0f);
    fn_239_2B10();
    fn_239_1DB0(field_0x6D8, field_0x744);
    fn_239_1C40();
}
void dAcObjGirahimuKnife_c::finalizeState_SpinWaitPreAttack() {
    mSph0.OnTgSet();
    field_0x714.toQuat(field_0x704);
    mSph0.SetTg_0x4C(0);
}

void dAcObjGirahimuKnife_c::initializeState_SpinFreeWait() {
    sSomething0++;
    field_0x778 = cM::rndInt(32000);
    mSph0.SetTg_0x4C(-1);
    // NOTE - This is needed to force the rodata pool loading despite not emitting anything
    field_0x774 = 20.0f;
    field_0x774 = 0.0f;
    sSomething1 += mAng(0x1000);
    field_0x77C = 120.0f;
    field_0x780 = sSomething1;
}
void dAcObjGirahimuKnife_c::executeState_SpinFreeWait() {
    mSph1.ClrAtSet();
    mSph0.SetR(50.0f);
    field_0x78F = true;
    mSph0.ClrTgSet();
    fn_239_2B10();
    fn_239_2110(field_0x6D8, 150.0f, 0x4000);
    fn_239_1C40();
}
void dAcObjGirahimuKnife_c::finalizeState_SpinFreeWait() {
    sSomething1 = mAng(0);
    mSph0.OnTgSet();
    field_0x714.toQuat(field_0x704);
    mSph0.SetTg_0x4C(0);
}

void dAcObjGirahimuKnife_c::initializeState_FreeWait() {
    field_0x6F6.x = cM::rndInt(64000);
    field_0x6F6.z = cM::rndInt(64000);
    field_0x778 = cM::rndInt(32000);
    field_0x774 = 10.0f;
}
void dAcObjGirahimuKnife_c::executeState_FreeWait() {
    mSph1.ClrAtSet();
    mSph0.SetR(50.0f);
    field_0x78F = true;
    fn_239_2B10();
    fn_239_1DB0(field_0x6D8, field_0x744);
    field_0x778 += 0x500;
    fn_239_1C40();
}
void dAcObjGirahimuKnife_c::finalizeState_FreeWait() {
    mSph0.OnTgSet();
    field_0x714.toQuat(field_0x704);
    mSph0.SetTg_0x4C(0);
}

// NONMATCHING
void dAcObjGirahimuKnife_c::initializeState_CircleWait() {
    mSph0.SetTg_0x4C(-1);
    field_0x774 = 0.0f;
    // Dont think about it
    mAng som = sSomething1;
    mAng newval = som + 0x1000;
    if (field_0x784 == 7) {
        newval = som - 0x800;
    }
    sSomething1 = newval;
    field_0x77C = 200.0f;

    field_0x780 = sSomething1;
    mSph0.OnTgSet();
    field_0x790 = true;
    mSph1.ClrCoSet();
    mSph0.ClrCoSet();
}
void dAcObjGirahimuKnife_c::executeState_CircleWait() {
    mSph1.ClrAtSet();
    mSph0.SetR(50.0f);
    field_0x78F = true;
    fn_239_2B10();
    if (field_0x784 == 7) {
        fn_239_1F50(dAcPy_c::GetLink()->mPosition, 50.0f);
    } else {
        fn_239_2110(dAcPy_c::GetLink()->mPosition, 150.0f, 0);
    }
    fn_239_1C40();
}
void dAcObjGirahimuKnife_c::finalizeState_CircleWait() {
    mSph0.OnTgSet();
    field_0x714.toQuat(field_0x704);
    mSph0.SetTg_0x4C(0);
    sSomething1 = mAng(0);
}

void dAcObjGirahimuKnife_c::fn_239_1C40() {
    if (field_0x78E != 0) {
        mSph1.ClrCoSet();
        mSph0.ClrCoSet();
        if (sLib::chase(&field_0x75C.y, 0.0f, 0.1f) != false) {
            deleteRequest();
        }
    } else {
        sLib::chase(&field_0x75C.y, 1.2f, 0.1f);
    }

    if (field_0x792) {
        field_0x6F6.x = field_0x6F6.x + 0x1700;
        field_0x6F6.y = field_0x6F0.y + 0x4000;
        field_0x6F6.z = field_0x6F6.z + 0x1700;
        field_0x714.ZXYrotS(field_0x6F6);
    } else {
        mVec3_c v = dAcPy_c::GetLink()->mPosition;
        v.y += 100.0f;
        mVec3_c norm = v - mPosition;
        norm.normalizeRS();

        mQuat_c q;
        q.slerp(mVec3_c(0.0f, 0.0f, 1.0f), norm, 1.0f);
        field_0x704.slerpTo(q, 0.5f, field_0x704);
        field_0x714.fromQuat(field_0x704);
    }
}

// NONMATCHING - UGH
void dAcObjGirahimuKnife_c::fn_239_1DB0(const mVec3_c &i_base, const mVec3_c &i_target) {
    mVec3_c base = i_base;
    mVec3_c target = i_target;
    target.rotY(field_0x6F0.y);

    mVec3_c toGoal = 0.05f * (base + target - mPosition);
    toGoal += mVelocity;
    toGoal *= 0.9f;

    mVelocity.set(toGoal);
    mPosition += mVelocity;
}

// NONMATCHING - UGH
void dAcObjGirahimuKnife_c::fn_239_1F50(const mVec3_c &i_target, f32 add) {
    mVec3_c target = i_target;
    mVec3_c front(0.0f, 0.0f, field_0x77C);

    mMtx_c m;
    m.YrotS(dAcPy_c::GetLink()->mRotation.y);
    m.XrotM(field_0x780);
    m.multVec(front, front);
    mVec3_c goal = target + front;
    goal.y += add;
    mPosition += mVelocity = (mVelocity + (goal - mPosition) * 0.05f) * 0.9f;
}

// NONMATCHING - UGH
void dAcObjGirahimuKnife_c::fn_239_2110(const mVec3_c &i_target, f32 add, s16 ang) {
    if (field_0x792) {
        field_0x780 += mAng(0x400);
    }
    mVec3_c target = i_target;
    mVec3_c front(0.0f, 0.0f, field_0x77C);

    mMtx_c m;
    m.YrotS(field_0x6F0.y + field_0x780);
    m.multVec(front, front);

    m.YrotS(field_0x6F0.y);
    m.XrotM(ang);
    m.YrotM(-field_0x6F0.y);
    m.multVec(front, front);

    mVec3_c goal = target + front;
    goal.y += add;
    mVec3_c v = (mVelocity + (goal - mPosition) * 0.05f) * 0.9f;
    mVelocity.set(v);
    mPosition += mVelocity;
}

void dAcObjGirahimuKnife_c::setTarget(const mVec3_c &t) {
    field_0x744.set(t);
}

void dAcObjGirahimuKnife_c::fn_239_2360(bool b0, bool b1, f32 zVel) {
    if (b0 && !isWaiting()) {
        return;
    }

    field_0x790 = b1;
    field_0x791 = b1;

    changeState(StateID_Attack);
    field_0x78F = true;
    mVec3_c diff = dAcPy_c::GetLink()->mPosition - field_0x6D8;
    mAng toPlayer = diff.atan2sX_Z();
    mAngle.y = toPlayer;
    mVelocity.set(0.0f, 0.0f, zVel);
    mVelocity.rotY(toPlayer);
    field_0x6D6 = 0xFF;
}

void dAcObjGirahimuKnife_c::fn_239_2460(bool b0, bool b1, f32 velMult) {
    if (b0 && !isWaiting()) {
        return;
    }

    if (!isCircleWait()) {
        field_0x790 = true;
    } else {
        field_0x790 = false;
    }
    changeState(StateID_Attack);

    mSph0.SetR(100.f);

    field_0x78F = b1;
    mVec3_c playerOffset = dAcPy_c::GetLink()->mPosition;
    playerOffset.y += 100.0f;
    mVec3_c diff = playerOffset - mPosition;
    mAngle.y = diff.atan2sX_Z();

    diff.normalizeRS();
    diff *= velMult;
    mVelocity.set(diff);
    field_0x6D6 = 0xFF;
}

bool dAcObjGirahimuKnife_c::isWaiting() {
    if (getStateID().isEqual(StateID_Wait) || getStateID().isEqual(StateID_SpinWait) ||
        getStateID().isEqual(StateID_SpinFreeWait) || getStateID().isEqual(StateID_FreeWait) ||
        getStateID().isEqual(StateID_CircleWait)) {
        return true;
    }
    return false;
}

bool dAcObjGirahimuKnife_c::isCircleWait() {
    return getStateID().isEqual(StateID_CircleWait);
}

bool dAcObjGirahimuKnife_c::lineCheck() {
    mVec3_c start, end;
    start = mPosition;
    mMtx_c m0 = field_0x714;

    mVec3_c offset(0.0f, 0.0f, -72.0f);
    m0.multVec(offset, offset);
    offset += mPosition;

    start = offset;
    end = mPosition;

    dBgS_ObjLinChk linChk;
    linChk.Set(&start, &end, nullptr);
    if (dBgS::GetInstance()->LineCross(&linChk)) {
        mPosition.set(linChk.GetLinEnd());
        mVelocity.set(0.0f, 0.0f, 0.0f);
        return true;
    }

    return false;
}

void dAcObjGirahimuKnife_c::updateMdlMatrix() {
    mRotation.z = 0x4000;
    mWorldMtx.transS(mPosition.x, mPosition.y + field_0x774 * mAng(field_0x778).sin(), mPosition.z);
    mWorldMtx.concat(field_0x714);
    mWorldMtx.ZXYrotM(mRotation);
    mWorldMtx.scaleM(field_0x75C);
    mMdl.setLocalMtx(mWorldMtx);
}

void dAcObjGirahimuKnife_c::startEmitters() {
    if (!field_0x792 && cM::isZero(VEC3LenSq(mVelocity))) {
        return;
    }
    mGlowEmitterId = PARTICLE_RESOURCE_ID_MAPPING_686_;
    mGlowEmitter.holdEffect(mGlowEmitterId, mWorldMtx, nullptr, nullptr);
    if (field_0x78C) {
        mTrailEmitterId = PARTICLE_RESOURCE_ID_MAPPING_1_;
        mWorldMtx.transM(0.0f, 0.0f, -30.0f);
        mColor c0(0, 0, 0, 0xFF);
        mColor c1(0, 0, 0, 0xFF);
        mTrailEmitter.holdEffect(mTrailEmitterId, mWorldMtx, &c0, &c1);
        mWorldMtx.transM(0.0f, 0.0f, 30.0f);
    }
}

void dAcObjGirahimuKnife_c::fn_239_2B10() {
    someEnemyDamageCollisionStuffMaybe(mCollider, nullptr);
    cCcD_Obj *pTgCc = mCollider.findTgHit();
    if (pTgCc == &mSph0) {
        switch (mSph0.GetTgAtHitType()) {
            case AT_TYPE_SWORD:
            case AT_TYPE_0x800000: {
                if (getStateID().isEqual(StateID_CircleWait)) {
                    if (checkCutDir(mSph0.GetTgAtCutDir())) {
                        field_0x78D = true;
                        sSomething2 = 0;
                        changeState(StateID_Return);
                    } else if (field_0x792) {
                        mVec3_c force;
                        getReflectForce(mSph0, 20.0f, force);
                        force.rotY(dAcPy_c::GetLink()->mRotation.y);
                        mVelocity.set(force);
                    } else {
                        mVec3_c force;
                        getReflectForce(mSph0, 100.0f + cM::rndF(20.0f), force);
                        force.rotY(dAcPy_c::GetLink()->mRotation.y);
                        mVelocity.set(force);
                        changeState(StateID_Hit);
                    }
                } else if (isWaiting()) {
                    mVec3_c force;
                    getReflectForce(mSph0, 100.0f + cM::rndF(20.0f), force);
                    force.rotY(dAcPy_c::GetLink()->mRotation.y);
                    mVelocity.set(force);
                    changeState(StateID_Hit);
                } else if (checkCutDir(mSph0.GetTgAtCutDir())) {
                    field_0x78D = true;
                    sSomething2 = 0;
                    changeState(StateID_Return);
                } else {
                    mVec3_c force;
                    getReflectForce(mSph0, 100.0f + cM::rndF(20.0f), force);
                    force.rotY(dAcPy_c::GetLink()->mRotation.y);
                    mVelocity.set(force);
                    if (mState == 8) {
                        field_0x793 = false;
                    } else {
                        sSomething2 = 0;
                    }
                    changeState(StateID_Hit);
                }
            } break;
            case AT_TYPE_BELLOWS: {
                mVec3_c force(0.0f, 0.0f, 100.0f + cM::rndF(20.0f));
                force.rotY(dAcPy_c::GetLink()->mRotation.y);
                mVelocity.set(force);
                changeState(StateID_Hit);
            } break;
        }
    }

    cCcD_Obj *pAtCc = mCollider.findAtHit();
    if (pAtCc != nullptr) {
        if (pAtCc->GetAtFlag0x8()) {
            if (mState == 8) {
                mVec3_c force = mPosition - dAcPy_c::GetLink()->mPosition;
                force.y = 0.0f;
                force.normalizeRS();
                adjustVelocity(force, 10.0f, 0.2f);
                changeState(StateID_Hit);
            } else {
                field_0x78D = true;
                changeState(StateID_Return);
            }
        } else if (pAtCc->GetAtFlag0x2()) {
            const dAcPy_c *pPlayer = dAcPy_c::GetLink();
            s16 angY = cLib::targetAngleY(pPlayer->mPosition, mPosition);
            s16 diff = pPlayer->mRotation.y - angY;
            mVec3_c force(0.0f, 0.0f, 1.0f);
            if (diff >= 0) {
                force.rotY(dAcPy_c::GetLink()->mRotation.y + -0x2000);
            } else {
                force.rotY(dAcPy_c::GetLink()->mRotation.y + 0x2000);
            }
            adjustVelocity(force, 10.0f, 1.0f);
            changeState(StateID_Hit);
        }
    }
}

bool dAcObjGirahimuKnife_c::checkCutDir(s32 cutDir) {
    if (!field_0x78F) {
        return false;
    }
    if (dAcPy_c::GetLink()->isAttackingSpin() && dAcPy_c::GetLink()->isAttackingHorizontal()) {
        if (field_0x784 == 0 || field_0x784 == 6) {
            return true;
        }
    }
    if (dAcPy_c::GetLink()->isAttackingSpin() && dAcPy_c::GetLink()->isAttackingVertical()) {
        if (field_0x784 == 3 || field_0x784 == 7) {
            return true;
        }
    }

    switch (cutDir) {
        case CUT_DIR_U:
        case CUT_DIR_D:
            if (field_0x784 == 3) {
                return true;
            }
            break;

        case CUT_DIR_RD:
        case CUT_DIR_LU:
            if (field_0x784 == 2) {
                return true;
            }
            break;

        case CUT_DIR_R:
        case CUT_DIR_L:
            if (field_0x784 == 0) {
                return true;
            }
            break;

        case CUT_DIR_RU:
        case CUT_DIR_LD:
            if (field_0x784 == 1) {
                return true;
            }
            break;

        case CUT_DIR_STAB: break;
    }
    return false;
}

void dAcObjGirahimuKnife_c::setWait() {
    mState = 9;
    changeState(StateID_Wait);
}

void dAcObjGirahimuKnife_c::setSpinWaitPreAttack() {
    changeState(StateID_SpinWaitPreAttack);
}
void dAcObjGirahimuKnife_c::getReflectForce(cCcD_Obj &cc, f32 forceMult, mVec3_c &outforce) {
    mVec3_c force;
    switch (cc.GetTgAtCutDir()) {
        case CUT_DIR_U:  force.set(0.0f, -0.5f, 0.5f); break;
        case CUT_DIR_D:  force.set(0.0f, 0.5f, 0.5f); break;
        case CUT_DIR_LU: force.set(-0.5f, -0.5f, 0.5f); break;
        case CUT_DIR_RD: force.set(0.5f, 0.5f, 0.5f); break;
        case CUT_DIR_L:  force.set(-0.5f, 0.0f, 0.5f); break;
        case CUT_DIR_R:  force.set(0.5f, 0.0f, 0.5f); break;
        case CUT_DIR_LD: force.set(-0.5f, 0.5f, 0.5f); break;
        case CUT_DIR_RU: force.set(0.5f, -0.5f, 0.5f); break;
        default:         force.set(0.0f, 0.0f, 1.0f); break;
    }
    force.normalizeRS();
    force *= forceMult;
    outforce.set(force);
}

void dAcObjGirahimuKnife_c::adjustVelocity(const mVec3_c &force, f32 yVel, f32 forwardVel) {
    mVec3_c v0(-mVelocity.x, 0.0f, -mVelocity.z);

    s32 dirVel = v0.atan2sX_Z();
    s32 dirForce = force.atan2sX_Z();
    mAng diff = (s32)(dirVel - dirForce);

    f32 norm = nw4r::math::FAbs(diff.normal_c());
    if (norm <= 0.1f) {
        norm = 0.1f;
    }
    if (norm >= 1.0f) {
        norm = 1.0f;
    }

    f32 mag = 2.0f * v0.dot(force) / force.mag();
    mVelocity = force * mag - v0;
    mVelocity.x *= forwardVel + norm;
    mVelocity.y = yVel;
    mVelocity.z *= forwardVel + norm;
}
