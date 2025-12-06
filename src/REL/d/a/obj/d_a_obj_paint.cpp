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
#include "rvl/MTX/mtx.h"
#include "toBeSorted/arc_managers/oarc_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_PAINT, dAcOpaint_c, fProfile::OBJ_PAINT, 0x163, 0, 0);

STATE_DEFINE(dAcOpaint_c, Wait);

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
        mSubtype = 0;
    } else {
        mSubtype = 1;
    }
    dAcObjBase_c::updateMatrix();
    CREATE_ALLOCATOR(dAcOpaint_c);
    mMdl.setLocalMtx(mWorldMtx);
    dBgS::GetInstance()->Regist(&mBgW, this);
    field_0x5AA = false;
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
            if (distance < _a + 700.f && !field_0x5AA) {
                if (mSubtype == 0) {
                    mRotationZRelated = rotationZRelated0;
                } else {
                    mRotationZRelated = rotationZRelated1;
                }
            }
            field_0x5AA = true;
            startSound(SE_Paint_LOOSE);
        }
        field_0x5A8 = 3;
        mPosition.y += 10.f;

        if (field_0x5AA) {
            mRotationZOffset = mAng(0x100);
        }
    }

    mRotationZOffset -= mAng((mRotation.z - mRotationZRelated) * (0.03f + (0.01f * _a)));
    mRotationZOffset *= 0.89f + (0.01f * _a);

    mRotation.z = mRotation.z + mRotationZOffset;

    if (field_0x5A8 > 0) {
        field_0x5A8--;
    } else if (field_0x5A8 != -1) {
        field_0x5A8 = 0xFF;
        mPosition.y -= 10.f;
    }
}

void dAcOpaint_c::finalizeState_Wait() {}

void dAcOpaint_c::updateMatrix() {
    PSMTXTrans(mWorldMtx, mPosition.x, mPosition.y, mPosition.z);
    mWorldMtx.XrotM(mRotation.x);
    mWorldMtx.YrotM(mRotation.y);
    mMtx_c mtx;
    PSMTXTrans(mtx, 100.0f, 100.0f, 0.0f);
    PSMTXConcat(mWorldMtx, mtx, mWorldMtx);
    mWorldMtx.ZrotM(mRotation.z);
    mMtx_c mtx2;
    PSMTXTrans(mtx2, -100.0f, -100.0f, 0.0f);
    PSMTXConcat(mWorldMtx, mtx2, mWorldMtx);
    mMdl.setLocalMtx(mWorldMtx);
}

const float dAcOpaint_c::lbl_613_rodata_2C = 1000.f;

const char *dAcOpaint_c::getOarcName() {
    return "Paint";
}

const char *dAcOpaint_c::getModelName() {
    switch (mSubtype) {
        case 0:  return "PaintA";
        case 1:  return "PaintB";
        default: return nullptr;
    }
}
