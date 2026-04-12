#include "d/a/obj/d_a_obj_flag.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_d.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

SPECIAL_ACTOR_PROFILE(OBJ_FLAG, dAcOFlag_c, fProfile::OBJ_FLAG, 0x14E, 0, 3);

STATE_DEFINE(dAcOFlag_c, Wait);

static dCcD_SrcCps sSrcCps = {
    // TODO
};

bool dAcOFlag_c::createHeap() {
    const char *arcName = getArcName();
    const char *mdlName = getMdlName();
    const char *anmName = getAnmTexSrtName();

    mResFile = nw4r::g3d::ResFile(getOarcResFile(arcName));
    nw4r::g3d::ResMdl resMdl = mResFile.GetResMdl(mdlName);
    u32 bufferOption = 0x1003120;
    if (anmName) {
        bufferOption = 0x1003324;
    }
    if (!mMdl.create(resMdl, &mAllocator, bufferOption, 1, nullptr)) {
        return false;
    }

    s32 type = getFromParams(0, 0xF);
    if (anmName) {
        nw4r::g3d::ResAnmTexSrt texSrt = mResFile.GetResAnmTexSrt(anmName);
        if (!mAnmTexSrt.create(resMdl, texSrt, &mAllocator, nullptr, 1)) {
            return false;
        }
        mMdl.setAnm(mAnmTexSrt);
        if (type == 2) {
            mAnmTexSrt.setFrame(0.f, 0);
        } else if (type == 3) {
            mAnmTexSrt.setFrame(1.f, 0);
        }
        mAnmTexSrt.setRate(0.f, 0);
    }

    mpJoints = new mVec3_c[mNumJoints];
    if (!mpJoints) {
        return false;
    }

    for (s32 i = 0; i < mNumJoints; ++i) {
        mpJoints[i] = mVec3_c::Zero;
    }

    if (type == 1 || type == 2 || type == 3) {
        mpJoints1 = new mVec3_c[15];
        if (!mpJoints1) {
            return false;
        }
        mpJoints2 = new mVec3_c[15];
        if (!mpJoints2) {
            return false;
        }
        mpJoints3 = new mVec3_c[15];
        if (!mpJoints3) {
            return false;
        }
        for (int i = 0; i < 15; ++i) {
            mpJoints2[i] = mVec3_c::Zero;
            mpJoints3[i] = mVec3_c::Zero;
        }
    }
    mMdl.setCallback(&mCallback);
    mMdl.enableCallbackOp(nw4r::g3d::ScnObj::EXECOP_CALC_MAT);
    mMdl.enableCallbackTiming(nw4r::g3d::ScnObj::CALLBACK_TIMING_C);

    return true;
}

int dAcOFlag_c::actorCreate() {
    s32 type = getFromParams(0, 0xF);
    if (isSail()) {
        mNumJoints = 104;
    } else {
        mNumJoints = 21;
    }

    CREATE_ALLOCATOR_SIZE(dAcOFlag_c, 0x4000);

    mStts.SetRank(0);
    mCps.Set(sSrcCps);
    mCps.SetR(sSrcCps.mCpsInf.mRadius * mScale.x);
    mCps.SetStts(mStts);

    mAcceleration = -9.8f;
    mMaxSpeed = 0.f;
    mStateMgr.changeState(StateID_Wait);

    if (type == 1) {
        mBoundingBox.Set(mVec3_c(-800.f, -100.f, -800.f), mVec3_c(800.f, 410.f, 800.f));
    } else if (isPurpleFlag()) {
        mBoundingBox.Set(mVec3_c(-250.f, -500.f, -250.f), mVec3_c(250.f, 500.f, 250.f));
    }

    mWorldMtx.transS(mPosition);
    mWorldMtx.YrotM(mRotation.y);
    field_0x530 = cM::rndInt(100);

    switch (type) {
        case 1: {
            field_0x548 = 0;
            if (mScale.x < 0.5f) {
                mCullingDistance += 6000.f;
            } else {
                mCullingDistance += 3000.f;
            }
        } break;
        case 2:
        case 3: {
            field_0x548 = 1;
        } break;
        case 4: {
            if (mScale.x < 1.f) {
                mCullingDistance *= 100.f;
            }
            mMdl.setPriorityDraw(0x1C, -1);
        } break;
        case 5: {
            mCullingDistance *= 5.f;
            mMdl.setPriorityDraw(0x1C, -1);
        }
    }
    return SUCCEEDED;
}

void dAcOFlag_c::initializeState_Wait() {}
void dAcOFlag_c::executeState_Wait() {}
void dAcOFlag_c::finalizeState_Wait() {}
