#include "d/a/obj/d_a_obj_decoB.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "rvl/MTX/mtx.h"

SPECIAL_ACTOR_PROFILE(OBJ_DECOB, dAcODecoB_c, fProfile::OBJ_DECOB, 0x161, 0, 3);

STATE_DEFINE(dAcODecoB_c, Wait);

bool dAcODecoB_c::createHeap() {
    void *data = getOarcResFile("DecoB");
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("DecoB");
    return mMdl.create(mdl, &mAllocator, 0x120, 1, nullptr);
}

int dAcODecoB_c::create() {
    if (!initAllocatorWork1Heap(0x1000, "dAcODecoB_c::m_allocator", 0x20)) {
        return FAILED;
    }
    mAcceleration = 0.0f;
    mMaxSpeed = 0.0f;
    mStateMgr.changeState(StateID_Wait);
    mBoundingBox.Set(mVec3_c(-350.0f, 100.0f, -100.0f), mVec3_c(350.0f, -300.0f, 100.0f));
    return SUCCEEDED;
}

int dAcODecoB_c::doDelete() {
    return SUCCEEDED;
}

int dAcODecoB_c::actorExecute() {
    mStateMgr.executeState();
    PSMTXTrans(mWorldMtx, mPosition.x, mPosition.y, mPosition.z);
    mWorldMtx.ZXYrotM(mRotation);
    mMdl.setLocalMtx(mWorldMtx);
    return SUCCEEDED;
}

int dAcODecoB_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcODecoB_c::initializeState_Wait() {}

void dAcODecoB_c::executeState_Wait() {
    if (dAcPy_c::GetLink() != nullptr && dAcPy_c::GetLink()->checkFlags0x350(0x2000)) {
        mVec3_c deltaPosition = dAcPy_c::GetLink()->mPosition - mPosition;
        f32 distance = EGG::Math<f32>::sqrt(deltaPosition.squareMagXZ());
        distance = (2000.0f - distance) / 2000.0f;
        if (distance < 0.0f) {
            distance = 0.0f;
        }
        field_0x38E = distance * 2048.0f;
    }
    field_0x38E -= mAng(mRotation.x * 0.05f);
    field_0x38E *= 0.95f;
    mRotation.x = mRotation.x + field_0x38E;
}

const f32 dAcODecoB_c::lbl_611_rodata_30 = 2000.0f;

void dAcODecoB_c::finalizeState_Wait() {}
