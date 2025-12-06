#include "d/a/obj/d_a_obj_paint.h"

#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/arc_managers/oarc_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_PAINT, dAcOpaint_c, fProfile::OBJ_PAINT, 0x163, 0, 0);

STATE_DEFINE(dAcOpaint_c, Wait);

mAng dAcOpaint_c::rotationZRelatedBatreaux = mAng::d2s(35.0025f);
mAng dAcOpaint_c::rotationZRelatedGroose = mAng::d2s(40.0015f);

bool dAcOpaint_c::createHeap() {
    const char *oarcName = getOarcName();
    const char *modelName = getModelName();
    void *data = getOarcResFile(oarcName);
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl(modelName);
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x120, 1, nullptr));

    void *dzb = OarcManager::GetInstance()->getDzbFromArc(oarcName, modelName);
    void *plc = OarcManager::GetInstance()->getPlcFromArc(oarcName, modelName);
    TRY_CREATE(!mBgW.Set((cBgD_t *)dzb, (PLC *)plc, 1, &mWorldMtx, &mScale));
    return SUCCEEDED;
}

int dAcOpaint_c::create() {
    if (getFromParams(0, 3) == 0) {
        mSubtype = Batreaux;
    } else {
        mSubtype = Groose;
    }
    dAcObjBase_c::updateMatrix();
    CREATE_ALLOCATOR(dAcOpaint_c);
    mMdl.setLocalMtx(mWorldMtx);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mPaintingSwayed = false;
    mStateMgr.changeState(StateID_Wait);
    return SUCCEEDED;
}

int dAcOpaint_c::doDelete() {
    return SUCCEEDED;
}

int dAcOpaint_c::actorExecute() {
    mStateMgr.executeState();
    updateMatrix();
    mBgW.Move();
    return SUCCEEDED;
}

int dAcOpaint_c::draw() {
    drawModelType1(&mMdl);
    static mQuat_c rot(0.0f, 0.0f, 0.0f, 100.0f);
    fn_8002edb0(mShadow, mMdl, &rot, -1, -1, 0.0f);
    return SUCCEEDED;
}

void dAcOpaint_c::initializeState_Wait() {}

void dAcOpaint_c::executeState_Wait() {
    s32 _a = 0;
    if (dAcPy_c::GetLink() != nullptr && dAcPy_c::GetLink()->checkFlags0x350(0x2000)) {
        mVec3_c deltaPosition = dAcPy_c::GetLink()->mPosition - mPosition;
        f32 distance = deltaPosition.absXZ();
        if (1000.f - distance > 0.f) {
            if (distance < _a + 700.f && !mPaintingSwayed) {
                if (mSubtype == Batreaux) {
                    mRotationZRelated = rotationZRelatedBatreaux;
                } else {
                    mRotationZRelated = rotationZRelatedGroose;
                }
                mPaintingSwayed = true;
                startSound(SE_Paint_LOOSE);
            }
            mPaintingOffsetTimer = 3;
            mPosition.y += 10.f;

            if (mPaintingSwayed) {
                mRotationZOffset = mAng(0x100);
            }
        }
    }

    mRotationZOffset -= mAng((mRotation.z - mRotationZRelated) * (0.03f + (0.01f * _a)));
    mRotationZOffset *= 0.89f + (0.01f * _a);

    mRotation.z = mRotation.z + mRotationZOffset;

    if (mPaintingOffsetTimer > 0) {
        mPaintingOffsetTimer--;
    } else if (mPaintingOffsetTimer != -1) {
        mPaintingOffsetTimer = -1;
        mPosition.y -= 10.f;
    }
}

void dAcOpaint_c::finalizeState_Wait() {}

void dAcOpaint_c::updateMatrix() {
    mWorldMtx.transS(mPosition);
    mWorldMtx.XrotM(mRotation.x);
    mWorldMtx.YrotM(mRotation.y);

    mMtx_c mtx;
    mtx.transS(100.0f, 100.0f, 0.0f);

    mWorldMtx += mtx;
    mWorldMtx.ZrotM(mRotation.z);

    mMtx_c mtx2;
    mtx2.transS(-100.0f, -100.0f, 0.0f);
    mWorldMtx += mtx2;

    mMdl.setLocalMtx(mWorldMtx);
}

const float dAcOpaint_c::lbl_613_rodata_2C = 1000.f;

const char *dAcOpaint_c::getOarcName() {
    return "Paint";
}

const char *dAcOpaint_c::getModelName() {
    switch (mSubtype) {
        case Batreaux: return "PaintA";
        case Groose:   return "PaintB";
        default:       return nullptr;
    }
}
