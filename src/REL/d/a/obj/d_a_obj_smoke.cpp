#include "d/a/obj/d_a_obj_smoke.h"

static const char *const sSmokeNames1[] = {
    "SmokeF200",
    "SmokeF202",
};

static const char *const sSmokeNames2[] = {
    "SmokeF200",
    "SmokeF202",
};

SPECIAL_ACTOR_PROFILE(OBJ_SMOKE, dAcOsmoke_c, fProfile::OBJ_SMOKE, 0x01DB, 0, 4);

STATE_DEFINE(dAcOsmoke_c, Wait);

bool dAcOsmoke_c::createHeap() {
    mType = mParams & 3;
    mBrres = nw4r::g3d::ResFile(getOarcResFile(sSmokeNames1[mType]));
    nw4r::g3d::ResMdl mdl = mBrres.GetResMdl(sSmokeNames2[mType]);
    TRY_CREATE(mModel.create(mdl, &heap_allocator, 0x324));
    nw4r::g3d::ResAnmTexSrt srt = mBrres.GetResAnmTexSrt(sSmokeNames1[mType]);
    TRY_CREATE(mTexAnm.create(mdl, srt, &heap_allocator, nullptr, 1));
    return true;
}

int dAcOsmoke_c::create() {
    CREATE_ALLOCATOR(dAcOsmoke_c);

    mModel.setAnm(mTexAnm);
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);
    mStateMgr.changeState(StateID_Wait);
    mModel.setPriorityDraw(0x1C, 9);
    boundingBox.Set(mVec3_c(-0.0f, -0.0f, -0.0f), mVec3_c(0.0f, 0.0f, 0.0f));
    return SUCCEEDED;
}

int dAcOsmoke_c::doDelete() {
    return SUCCEEDED;
}

int dAcOsmoke_c::actorExecute() {
    mStateMgr.executeState();
    mTexAnm.play();
    return SUCCEEDED;
}

int dAcOsmoke_c::draw() {
    drawModelType1(&mModel);
    return SUCCEEDED;
}

void dAcOsmoke_c::initializeState_Wait() {}

void dAcOsmoke_c::executeState_Wait() {}

void dAcOsmoke_c::finalizeState_Wait() {}
