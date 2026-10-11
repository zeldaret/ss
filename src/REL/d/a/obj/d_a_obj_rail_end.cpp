#include "d/a/obj/d_a_obj_rail_end.h"

#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

static dCcD_SrcSph sSrcSph = {
    {
     /* mObjAt */ {AT_TYPE_NONE, 0x2000003E, {4, 0, 0}, 0, 1, 0, 0, CUT_DIR_NONE, 0},
     /* mObjTg */ {~AT_TYPE_COMMON0, 0x11, {0, 6, 0x407}, 0, CUT_DIR_NONE},
     /* mObjCo */ {0x10E9},
     },
    {100.f}
};

const f32 dAcORailEnd_c::lbl_525_rodata_0 = 0.0f;

SPECIAL_ACTOR_PROFILE(OBJ_RAIL_END, dAcORailEnd_c, fProfile::OBJ_RAIL_END, 0x281, 0, 3);

dAcORailEnd_c::dAcORailEnd_c() {}
dAcORailEnd_c::~dAcORailEnd_c() {}

bool dAcORailEnd_c::createHeap() {
    void *data = getOarcResFile("RailEnd");
    if (data == nullptr) {
        return false;
    }

    nw4r::g3d::ResFile file(data);
    if (!file.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl mdl = file.GetResMdl("RailEnd");
    if (!mdl.IsValid()) {
        return false;
    }

    if (!mMdl.create(mdl, &mAllocator, 0x20, 1, nullptr)) {
        return false;
    }
    mMdl.setLocalMtx(mWorldMtx);
    return true;
}

int dAcORailEnd_c::create() {
    updateMatrix();
    CREATE_ALLOCATOR(dAcORailEnd_c);

    mVec3_c min, max;
    mMdl.getBounds(&min, &max);
    mBoundingBox.Set(min, max);

    mStts.SetRank(13);
    mSph.Set(sSrcSph);
    mSph.SetStts(mStts);

    mVec3_c center(0.0f, 0.0f, 240.0f);
    mWorldMtx.multVec(center, center);
    mSph.cM3dGSph::Set(&center, 70.0f);
    return SUCCEEDED;
}

int dAcORailEnd_c::doDelete() {
    return SUCCEEDED;
}

int dAcORailEnd_c::actorExecute() {
    mMdl.setLocalMtx(mWorldMtx);
    dCcS::GetInstance()->Set(&mSph);
    return SUCCEEDED;
}

int dAcORailEnd_c::draw() {
    mMdl.entry();
    return SUCCEEDED;
}
