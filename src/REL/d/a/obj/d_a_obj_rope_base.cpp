#include "d/a/obj/d_a_obj_rope_base.h"

#include "d/a/obj/d_a_obj_base.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"

SPECIAL_ACTOR_PROFILE(OBJ_ROPE_BASE, dAcOropeBase_c, fProfile::OBJ_ROPE_BASE, 0x216, 0, 3);

bool dAcOropeBase_c::createHeap() {
    void *data = getOarcResFile("PracticeWood");
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("RopeBase");
    TRY_CREATE(mMdl.create(mdl, &heap_allocator, 0x120, 1, nullptr));
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.calc(true);
    nw4r::g3d::ResNode resNode = mdl.GetResNode("rope_loc");
    mMdl.getNodeWorldMtxMultVecZero(resNode.GetID(), mRopePos);
    return true;
}

int dAcOropeBase_c::create() {
    CREATE_ALLOCATOR(dAcOropeBase_c);
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    boundingBox.Set(mVec3_c(-525.0f, -0.0f, -500.0f), mVec3_c(-325.0f, 700.0f, -300.0f));
    return SUCCEEDED;
}

int dAcOropeBase_c::doDelete() {
    return SUCCEEDED;
}

int dAcOropeBase_c::actorExecute() {
    if (checkObjectProperty(2)) {
        field_0x35C = true;
    } else {
        field_0x35C = false;
    }
    return SUCCEEDED;
}

int dAcOropeBase_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}
