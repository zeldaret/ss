#include "d/a/obj/d_a_obj_trap_rock_1.h"

#include "d/col/bg/d_bg_s.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "m/m_vec.h"
#include "nw4r/math/math_arithmetic.h"
#include "s/s_Math.h"

SPECIAL_ACTOR_PROFILE(OBJ_TRAP_ROCK_1, dAcOtrapRock1_c, fProfile::OBJ_TRAP_ROCK_1, 0x26B, 0, 2);

STATE_DEFINE(dAcOtrapRock1_c, TrapWait);
STATE_DEFINE(dAcOtrapRock1_c, TrapAction);
STATE_DEFINE(dAcOtrapRock1_c, TrapReturn);

bool dAcOtrapRock1_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(getOarcResFile("TrapRockRoll"));
    nw4r::g3d::ResMdl m = mResFile.GetResMdl("TrapRockRoll");

    TRY_CREATE(mMdl.create(m, &mAllocator, 0x120));
    mMdl.setPriorityDraw(0x1C, 0x09);
    void *dzb = getOarcFile("TrapRockRoll", "dzb/TrapRockRoll.dzb");
    void *plc = getOarcFile("TrapRockRoll", "dat/TrapRockRoll.plc");
    updateMatrix();
    return !mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, nullptr);
}

int dAcOtrapRock1_c::create() {
    mActivationSceneFlag = mParams & 0xFF;
    mReturnSceneFlag = (mParams >> 8) & 0xFF;
    CREATE_ALLOCATOR(dAcOtrapRock1_c);

    mBgW.SetCrrFunc(dBgS_MoveBGProc_Typical);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mAcceleration = 0.0f;
    mMaxSpeed = -40.0f;
    mStateMgr.changeState(StateID_TrapWait);
    mVec3_c min, max;
    mMdl.getBounds(&min, &max);
    static mVec3_c offset = mVec3_c(50.0f, 50.0f, 50.0f);
    min -= offset;
    max += offset;
    mBoundingBox.Set(min, max);

    return SUCCEEDED;
}

int dAcOtrapRock1_c::doDelete() {
    return SUCCEEDED;
}

int dAcOtrapRock1_c::actorExecute() {
    mStateMgr.executeState();
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mBgW.Move();
    return SUCCEEDED;
}

int dAcOtrapRock1_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOtrapRock1_c::initializeState_TrapWait() {}
void dAcOtrapRock1_c::executeState_TrapWait() {
    if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mActivationSceneFlag)) {
        mStateMgr.changeState(StateID_TrapAction);
    }
}
void dAcOtrapRock1_c::finalizeState_TrapWait() {}
void dAcOtrapRock1_c::initializeState_TrapAction() {
    mFrameCounter = 0;
    field_0x59E = 0x2000;
    field_0x5A0 = 0x2000;
    field_0x5A5 = 1;
    field_0x5A2 = 0x2D8;
    startSound(SE_TrapRc1_TRAP_ACTION);
}

void dAcOtrapRock1_c::executeState_TrapAction() {
    if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mReturnSceneFlag)) {
        mStateMgr.changeState(StateID_TrapReturn);
    } else if (field_0x59E == 0 || mFrameCounter > 4) {
        // After 5 frames, move mRotation.x to 0x4000, then stay until return
        bool reachedPoint = sLib::chaseAngle(&mRotation.x.mVal, 0x4000, 0x14);
        if (reachedPoint) {
            return;
        }
    } else {
        f32 ratio;
        if (mFrameCounter == 0 && mRotation.x < 0x4000) {
            ratio = 0.1f;
        } else {
            f32 b = field_0x5A0;
            f32 r = (mRotation.x - 0x4000) / b;
            ratio = nw4r::math::FAbs(r);
        }
        if (ratio > 1.0f) {
            ratio = 1.0f;
        } else if (ratio < 0.1f) {
            ratio = 0.1f;
        }
        s16 newAng = field_0x5A5 * (1.0f - ratio) * field_0x5A2;
        static const u16 sSomeValue = 0x3C;
        if (field_0x5A5 > 0) {
            if ((u16)newAng <= sSomeValue - 1) {
                newAng = sSomeValue;
            }
        } else if ((u16)(newAng + sSomeValue - 1) <= sSomeValue - 1) {
            newAng = -sSomeValue;
        }

        mRotation.x += (int)newAng;
        if (field_0x59E > 0 && mRotation.x > field_0x59E + 0x4000 ||
            field_0x59E < 0 && mRotation.x < field_0x59E + 0x4000) {
            mRotation.x = field_0x59E + 0x4000;
            u8 r6 = field_0x5A5;
            field_0x5A5 = r6 - (r6 * 2);
            field_0x5A0 = field_0x59E;
            field_0x59E = field_0x59E - (field_0x59E * 2);
            if (mFrameCounter >= 1) {
                field_0x59E = field_0x59E >> 1;
                field_0x5A2 = field_0x5A2 >> 2;
            } else {
                field_0x5A2 = field_0x5A2 * 2 / 3;
            }
            mFrameCounter++;
        }
    }
}
void dAcOtrapRock1_c::finalizeState_TrapAction() {}
void dAcOtrapRock1_c::initializeState_TrapReturn() {}
void dAcOtrapRock1_c::executeState_TrapReturn() {
    bool reachedReturnPoint = sLib::chaseAngle(&mRotation.x.mVal, 0, 0x222);
    if (reachedReturnPoint) {
        mStateMgr.changeState(StateID_TrapWait);
    }
}
void dAcOtrapRock1_c::finalizeState_TrapReturn() {}
