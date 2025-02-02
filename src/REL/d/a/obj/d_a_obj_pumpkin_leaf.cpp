#include "d/a/obj/d_a_obj_pumpkin_leaf.h"

#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_Math.h"

SPECIAL_ACTOR_PROFILE(OBJ_PUMPKIN_LEAF, dAcOPumpkinLeaf_c, fProfile::OBJ_PUMPKIN_LEAF, 0x0135, 0, 3);

STATE_DEFINE(dAcOPumpkinLeaf_c, Wait);

bool dAcOPumpkinLeaf_c::createHeap() {
    mBrres = static_cast<nw4r::g3d::ResFile>(getOarcResFile("Pumpkin"));
    nw4r::g3d::ResMdl mdl = mBrres.GetResMdl("Leaf");
    TRY_CREATE(mModel.create(mdl, &heap_allocator, 0x120, 1, nullptr));
    return true;
}

int dAcOPumpkinLeaf_c::create() {
    if (!initAllocatorWork1Heap(0x1000, "dAcOPumpkinLeaf_c::m_allocator", 0x20)) {
        return FAILED;
    }

    forwardAccel = -1.0f;
    forwardMaxSpeed = -40.0f;
    updateMatrix();
    mStateMgr.changeState(StateID_Wait);
    boundingBox.Set(mVec3_c(-50.0f, -10.0f, -50.0f), mVec3_c(50.0f, 50.0f, 50.0f));
    dAcObjBase_c::create(
        "PmpknBd", getRoomId(), 0, &position, &rotation, &mScale, getParams2_ignoreLower(), -1, viewclip_index
    );
    return SUCCEEDED;
}

int dAcOPumpkinLeaf_c::doDelete() {
    return SUCCEEDED;
}

int dAcOPumpkinLeaf_c::actorExecute() {
    mModel.setLocalMtx(mWorldMtx);
    return SUCCEEDED;
}

int dAcOPumpkinLeaf_c::draw() {
    drawModelType1(&mModel);
    return SUCCEEDED;
}

void dAcOPumpkinLeaf_c::initializeState_Wait() {}
void dAcOPumpkinLeaf_c::executeState_Wait() {}
void dAcOPumpkinLeaf_c::finalizeState_Wait() {}
