#include "d/a/d_a_fish_base.h"

#include "c/c_lib.h"
#include "d/a/d_a_fish_mgr_base.h"
#include "d/a/d_a_player.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_vec.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "s/s_Math.h"

STATE_VIRTUAL_DEFINE(dAcFishBase_c, Swim);
STATE_VIRTUAL_DEFINE(dAcFishBase_c, Escape);

bool dAcFishBase_c::createHeap() {
    void *data = getOarcResFile(getResFileName());
    TRY_CREATE(mMdl.create(data, getMdlName(), getAnmName(), &mAllocator, 0x120, 1, nullptr));

    return true;
}

int dAcFishBase_c::create() {
    mAcceleration = 0.0f;
    mMaxSpeed = -40.0f;
    field_0xA7C = 0;
    field_0xA7E = 0;
    field_0xA80 = cM::rndF(65536.0f);
    field_0xA82 = 0;
    field_0xA84 = 0;
    field_0xA60.set(0.0f, 0.0f, 0.0f);
    field_0xA86 = 0;
    field_0xA88 = 0;
    field_0xA8A = 0;
    field_0xA8C = 0;
    field_0xA90 = 0.0f;

    field_0xA78 = 0;
    field_0xA79 = 0;
    field_0xA7A = 0;

    return SUCCEEDED;
}

int dAcFishBase_c::actorExecute() {
    vt_0x8C();
    fn_8018D5A0();
    return SUCCEEDED;
}

int dAcFishBase_c::doDelete() {
    return dAcObjBase_c::doDelete();
}

int dAcFishBase_c::draw() {
    return dAcObjBase_c::draw();
}

void dAcFishBase_c::vt_0x8C() {
    mSph1.SetC(mPosition);
    dCcS::GetInstance()->Set(&mSph1);
    fn_8018D4D0();
}

dAcFishMgrBase_c *dAcFishBase_c::getFishMgr() {
    return static_cast<dAcFishMgrBase_c *>(mActorNode.get());
}

extern "C" s32 fn_800406D0(s32, s32);
extern "C" f32 fn_800408B0(f32, f32);
extern "C" s32 fn_800408F0(s32, s32);

void dAcFishBase_c::fn_8018C250() {
    dAcFishBase_c *fish = getFishMgr()->getFish();
    if (fish == nullptr) {
        field_0xA58 = 1;
    }

    if (field_0xA58 != 0) {
        fn_8018C760();
    } else {
        field_0xA60 = fish->getPosition() + field_0xA4C;
    }

    field_0xA8A = 0;
    field_0xA8C = vt_0xB8() + cM::rndF(vt_0xBC());
    field_0xA86 = cLib::targetAngleY(mPosition, field_0xA60);
    field_0xA88 = -cLib::targetAngleX(mPosition, field_0xA60);
    if (field_0xA88 > 0x1800) {
        field_0xA88 = 0x1800;
    } else if (field_0xA88 < -0x1800) {
        field_0xA88 = -0x1800;
    }

    if (field_0xA58 == 0 && mPosition.distance(fish->getPosition()) > vt_0xFC()) {
        field_0xA90 = vt_0xDC();
        field_0xA7E = vt_0xF4();
    } else {
        s32 t1, t2;
        t1 = vt_0xD8();
        t2 = vt_0xDC();
        field_0xA90 = fn_800406D0(t1, t2);
        if (field_0xA58 != 0) {
            t1 = vt_0xE0();
            t2 = vt_0xE4();
        } else {
            t1 = vt_0xF4();
            t2 = vt_0xF8();
        }
        field_0xA7E = fn_800406D0(t1, t2);

        if (field_0xA58 != 0) {
            field_0xA7E *= (s16)(getFishMgr()->getField_0x144() / 1000.0f);
        }
    }

    field_0xA78 = 0;
    field_0xA79 = 0;
}

void dAcFishBase_c::fn_8018C5A0() {
    fn_8018C870();
    field_0xA8A = 0;
    field_0xA86 = cLib::targetAngleY(getPosition(), field_0xA60) + (s16)cM::rndFX(8192.0f);
    field_0xA88 = -cLib::targetAngleX(getPosition(), field_0xA60);
    if (field_0xA88 > 0x1800) {
        field_0xA88 = 0x1800;
    } else if (field_0xA88 < -0x1800) {
        field_0xA88 = -0x1800;
    }

    field_0xA90 = fn_800408B0(vt_0x104(), vt_0x108());
    if (field_0xA78 != 0 || field_0xA79 != 0) {
        field_0xA7E = fn_800406D0(vt_0xE0(), vt_0xE4());
    } else {
        field_0xA7E = fn_800406D0(vt_0x114(), vt_0x118());
    }

    field_0xA78 = 0;
    field_0xA79 = 0;
}

void dAcFishBase_c::fn_8018C760() {
    dAcFishMgrBase_c *mgr = getFishMgr();
    f32 f1 = mgr->getField_0x144();
    f32 f2 = mgr->getField_0x148();

    if (field_0xA78 != 0) {
        mMtx_c mtx;
        mtx.YrotS(cLib::targetAngleY(field_0xA6C, getPosition()));
        mVec3_c v(0.0f, 0.0f, 500.0f);
        mtx.multVec(v, field_0xA60);
        field_0xA60.x += getPosition().x;
        field_0xA60.z += getPosition().z;
    } else {
        field_0xA60.x = mStartingPos.x + cM::rndFX(f1);
        field_0xA60.z = mStartingPos.z + cM::rndFX(f1);
    }

    field_0xA60.y = mStartingPos.y + cM::rndF(f2);
    fn_8018CCA0();
}

void dAcFishBase_c::fn_8018C870() {
    f32 f1 = vt_0x120();
    mVec3_c v1 = mPosition;
    getXZCirclePoint(v1, mAngle.y + 0x4000, f1);

    mVec3_c v2 = mPosition;
    getXZCirclePoint(v2, mAngle.y + -0x4000, f1);

    bool b1 = false, b2 = false;
    if (dBgS_ObjLinChk::LineCross(&mPosition, &v1, this)) {
        b1 = true;
    }
    if (dBgS_ObjLinChk::LineCross(&mPosition, &v2, this)) {
        b2 = true;
    }

    mMtx_c mtx;
    if ((field_0xA78 != 0 || field_0xA79 != 0) && b1 && b2) {
        mtx.YrotS(cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition));
    } else if ((field_0xA78 != 0 || field_0xA79 != 0) && b1 && !b2) {
        mtx.YrotS(cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition) + fn_800408F0(1820, 8192));
    } else if ((field_0xA78 != 0 || field_0xA79 != 0) && !b1 && b2) {
        mtx.YrotS(cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition) + fn_800406D0(-8192, -1820));
    } else if ((field_0xA78 != 0 || field_0xA79 != 0) && !b1 && !b2) {
        if (cM::rndF(1.0f) < 0.5f) {
            mtx.YrotS(cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition) + fn_800408F0(1820, 8192));
        } else {
            mtx.YrotS(cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition) + fn_800406D0(-8192, -1820));
        }
    } else {
        mtx.YrotS(cLib::targetAngleY(dAcPy_c::GetLink()->mPosition, mPosition));
    }

    dAcFishMgrBase_c *mgr = getFishMgr();
    mVec3_c v3(0.0f, 0.0f, 500.0f);
    mtx.multVec(v3, field_0xA60);
    field_0xA60.x += getPosition().x;
    field_0xA60.z += getPosition().z;
    field_0xA60.y = mStartingPos.y + cM::rndF(mgr->getField_0x148());

    if ((field_0xA78 || field_0xA79) && b1 && b2) {
        f32 f3 = field_0xA60.y - dAcPy_c::GetLink()->getPosition().y;
        if (nw4r::math::FAbs(f3) < 50.0f) {
            if (f3 > 0.0f) {
                field_0xA60.y += 50.0f;
            } else {
                field_0xA60.y -= 50.0f;
            }
        }
    }

    fn_8018CCA0();
}

void dAcFishBase_c::fn_8018CCA0() {
    // NONMATCHING
    dBgS_LinChk chk;
    chk.Set(&mPosition, &field_0xA60, this);
    if (dBgS::GetInstance()->LineCross(&chk)) {
        mVec3_c end = chk.GetLinEnd();
        f32 dist = mPosition.distance(end);
        if ((mStateMgr.getStateID()->isEqual(StateID_Swim) && dist < vt_0xEC()) ||
            (mStateMgr.getStateID()->isEqual(StateID_Escape) && dist < vt_0x120())) {
            mVec3_c t1 = field_0xA60 - mPosition;
            t1.rotY(0x8000);
            field_0xA60 = t1 + mPosition;
        }
    }
    dAcFishMgrBase_c *mgr = getFishMgr();
    f32 f2 = mgr->getPosition().absXZTo(field_0xA60);
    if (f2 > mgr->getField_0x144()) {
        // TODO FPR Regswaps
        f2 = mgr->getField_0x144() / f2;

        field_0xA60.x -= mgr->getPosition().x;
        field_0xA60.z -= mgr->getPosition().z;

        field_0xA60.x *= f2;
        field_0xA60.z *= f2;

        field_0xA60.x += mgr->getPosition().x;
        field_0xA60.z += mgr->getPosition().z;
    }

    if (field_0xA60.y < mgr->getPosition().y) {
        field_0xA60.y = mgr->getPosition().y + 100.0f;
    } else if (field_0xA60.y > mgr->getPosition().y + mgr->getField_0x148()) {
        field_0xA60.y = mgr->getPosition().y + mgr->getField_0x148() - 100.0f;
    }

    f32 f3 = mgr->getField_0x14C() - field_0xA60.y;
    bool b1 = field_0xA60.y > mPosition.y;

    if ((f3 < vt_0xD4() && b1) || mgr->getField_0x151()) {
        field_0xA60.y = mPosition.y;
    }
}

bool dAcFishBase_c::fn_8018CFC0(f32 arg) {
    mVec3_c t1 = mPosition;
    getXZCirclePoint(t1, mAngle.y, arg);
    dAcFishMgrBase_c *mgr = getFishMgr();
    mVec3_c t2 = mgr->getPosition() - t1;
    return t2.absXZ() > mgr->getField_0x144();
}

bool dAcFishBase_c::fn_8018D0D0(f32 arg) {
    dAcFishBase_c *fish = nullptr;
    while ((fish = static_cast<dAcFishBase_c *>(fManager_c::searchBaseByProfName(fProfile::FISH, fish))) != nullptr) {
        if (fish->field_0xA82 != 0 && field_0xA94 < arg) {
            return true;
        }
    }

    while ((fish = static_cast<dAcFishBase_c *>(fManager_c::searchBaseByProfName(fProfile::EEL, fish))) != nullptr) {
        if (fish->field_0xA82 != 0 && field_0xA94 < arg) {
            return true;
        }
    }

    return false;
}

void dAcFishBase_c::fn_8018D190(f32 arg, f32 unused) {
    vt_0x120();
    mVec3_c t1 = mPosition;
    getXZCirclePoint(t1, mAngle.y, arg);
    if (dBgS_ObjLinChk::LineCross(&mPosition, &t1, this)) {
        field_0xA78 = 1;
        field_0xA6C.x = dBgS_ObjLinChk::GetInstance().GetLinEnd().x;
        field_0xA6C.y = dBgS_ObjLinChk::GetInstance().GetLinEnd().y;
        field_0xA6C.z = dBgS_ObjLinChk::GetInstance().GetLinEnd().z;
    } else {
        field_0xA78 = 0;
        if (mObjAcch.ChkWallHit(nullptr) || mSph2.ChkCoHit() || mSph1.ChkCoHit()) {
            field_0xA78 = 1;
            mMtx_c mtx;
            mtx.YrotS(mRotation.y);
            mtx.XrotM(mRotation.x);
            mMtx_c mtx2;
            mtx2.transS(mVec3_c(0.0f, 0.0f, 100.0f));
            MTXConcat(mtx, mtx2, mtx);
            mVec3_c t;
            mtx.getTranslation(t);
            field_0xA6C = mPosition + t;
        } else if (fn_8018CFC0(arg)) {
            field_0xA79 = 1;
        } else {
            field_0xA79 = 0;
        }
    }
}

void dAcFishBase_c::vt_0xB4() {
    mMtx_c mtx;
    mtx.YrotS(mRotation.y);
    mtx.XrotM(mRotation.x);
    mVec3_c v(0.0f, 0.0f, mSpeed);
    MTXMultVec(mtx, v, mVelocity);

    if (field_0xA7A != 0) {
        mPosition.x += mVelocity.x;
        mPosition.z += mVelocity.z;
    } else {
        mPosition += mVelocity;
    }
    mPosition += mStts.GetCcMove();
}

void dAcFishBase_c::fn_8018D4D0() {
    mMtx_c mtx;
    mtx.YrotS(mRotation.y);
    mtx.XrotM(mRotation.x);
    mMtx_c mtx2;
    mtx2.transS(mVec3_c(0.0f, 0.0f, 200.0f));
    MTXConcat(mtx, mtx2, mtx);
    mVec3_c t;
    mtx.getTranslation(t);
    mSph2.SetC(mPosition + t);
    dCcS::GetInstance()->Set(&mSph2);
}

void dAcFishBase_c::fn_8018D5A0() {
    sLib::calcTimer(&field_0xA7C);
    sLib::calcTimer(&field_0xA7E);
    sLib::calcTimer(&field_0xA82);
    sLib::calcTimer(&field_0xA84);
    field_0xA80 += 1;
}

bool dAcFishBase_c::fn_8018D600() {
    // NINMATCHING
    dAcFishMgrBase_c *mgr = getFishMgr();
    // TODO
    f32 lim = mgr->getPosition().y + mgr->getField_0x148();
    if (mPosition.y >= lim && mAngle.x < 0) {
        return false;
    } else {
        return true;
    }
}

void dAcFishBase_c::fn_8018D660() {
    sLib::addCalcAngle(&mAngle.y.mVal, field_0xA86, 8, field_0xA8A);
    sLib::addCalcAngle(&field_0xA8A, field_0xA8C, 1, vt_0xC0());
}

void dAcFishBase_c::fn_8018D6E0() {
    sLib::addCalcAngle(&mAngle.y.mVal, field_0xA86, 8, field_0xA8A);
    sLib::addCalcAngle(&field_0xA8A, 3000, 1, 1000);
}

void dAcFishBase_c::fn_8018D730() {
    s16 invScale = 8;
    s16 maxStep = 20;
    if (field_0xA7A != 0) {
        invScale = 6;
        maxStep = 30;
    }
    sLib::addCalcAngle(&mAngle.x.mVal, field_0xA88, invScale, maxStep);
}

void dAcFishBase_c::fn_8018D760() {
    sLib::addCalcAngle(&mAngle.x.mVal, field_0xA88, 4, 40);
}

void dAcFishBase_c::initializeState_Swim() {
    vt_0xA8();
    field_0xA7E = 0;
    field_0xA84 = vt_0xE8();
}

void dAcFishBase_c::executeState_Swim() {
    if (field_0xA84 == 0) {
        fn_8018D190(vt_0xEC(), vt_0xF0());
        field_0xA84 = vt_0xE8();
    }

    if (field_0xA7E == 0 || field_0xA78 != 0 || field_0xA79 != 0) {
        fn_8018C250();
    }
    vt_0xC4();
    sLib::addCalcScaledDiff(&mSpeed, field_0xA90 * mScale.y, 0.1f, vt_0xCC());

    if (vt_0xB0() == 0) {
        mMdl.setRate(mSpeed * 0.3f);
    }

    if (fn_8018D600()) {
        field_0xA7A = 0;
    } else {
        field_0xA7A = 1;
        field_0xA88 = 0;
        field_0xA90 = vt_0xD8();
    }

    if ((field_0xA80 & 7) == 0 && (field_0xA94 < vt_0x100() || fn_8018D0D0(800.0f))) {
        mStateMgr.changeState(StateID_Escape);
        field_0xA82 = 20;
    }
}

void dAcFishBase_c::finalizeState_Swim() {}

void dAcFishBase_c::initializeState_Escape() {
    vt_0xAC();
    field_0xA7E = 0;
    field_0xA7C = fn_800406D0(vt_0x10C(), vt_0x110());
    field_0xA84 = vt_0x11C();
}

void dAcFishBase_c::executeState_Escape() {
    if (field_0xA84 == 0) {
        fn_8018D190(vt_0x120(), vt_0x124());
        field_0xA84 = vt_0x11C();
    }

    if (field_0xA7E == 0 || field_0xA78 != 0 || field_0xA79 != 0) {
        fn_8018C250();
    }
    vt_0xC8();
    sLib::addCalcScaledDiff(&mSpeed, field_0xA90 * mScale.y, 0.5f, vt_0xD0());

    if (vt_0xB0() == 0) {
        mMdl.setRate(mSpeed * 0.3f);
    }

    if (fn_8018D600()) {
        field_0xA7A = 0;
    } else {
        field_0xA7A = 1;
        field_0xA88 = 0;
        field_0xA90 = vt_0xD8();
    }

    if (field_0xA7C == 0) {
        mStateMgr.changeState(StateID_Escape);
    }
}

void dAcFishBase_c::finalizeState_Escape() {}
