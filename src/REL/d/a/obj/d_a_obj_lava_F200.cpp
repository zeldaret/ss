#include "d/a/obj/d_a_obj_lava_F200.h"

#include "d/d_stage.h"
#include "nw4r/g3d/res/g3d_resfile.h"

SPECIAL_ACTOR_PROFILE(OBJ_LAVA_F200, dAcOlavaF200_c, fProfile::OBJ_LAVA_F200, 0x0214, 0, 0);

bool dAcOlavaF200_c::createHeap() {
    mBrres = nw4r::g3d::ResFile(getOarcResFile("LavaF200"));
    dStage_c::bindStageResToFile(&mBrres);

    nw4r::g3d::ResMdl mdl0 = mBrres.GetResMdl("LavaF200");
    TRY_CREATE(mModels[0].create(mdl0, &mAllocator, 0x32C));
    nw4r::g3d::ResAnmTexSrt anmTex0 = mBrres.GetResAnmTexSrt("LavaF200");
    TRY_CREATE(mTexAnms[0].create(mdl0, anmTex0, &mAllocator, nullptr, 1));
    nw4r::g3d::ResAnmClr anmClr0 = mBrres.GetResAnmClr("LavaF200");
    TRY_CREATE(mClrAnms[0].create(mdl0, anmClr0, &mAllocator, nullptr, 1));

    nw4r::g3d::ResMdl mdl1 = mBrres.GetResMdl("LavaF200_s");
    TRY_CREATE(mModels[1].create(mdl1, &mAllocator, 0x32C));
    nw4r::g3d::ResAnmTexSrt anmTex1 = mBrres.GetResAnmTexSrt("LavaF200_s");
    TRY_CREATE(mTexAnms[1].create(mdl1, anmTex1, &mAllocator, nullptr, 1));
    nw4r::g3d::ResAnmClr anmClr1 = mBrres.GetResAnmClr("LavaF200_s");
    TRY_CREATE(mClrAnms[1].create(mdl1, anmClr1, &mAllocator, nullptr, 1));

    return true;
}

int dAcOlavaF200_c::create() {
    CREATE_ALLOCATOR(dAcOlavaF200_c);

    mModels[0].setAnm(mTexAnms[0]);
    mModels[1].setAnm(mTexAnms[1]);
    mModels[0].setAnm(mClrAnms[0]);
    mModels[1].setAnm(mClrAnms[1]);

    updateMatrix();
    for (int i = 0; i < 2; i++) {
        mModels[i].setLocalMtx(mWorldMtx);
    }

    mModels[0].setPriorityDraw(0x1C, 9);
    mModels[1].setPriorityDraw(0x22, 9);
    mBoundingBox.Set(mVec3_c(-0.0f, -0.0f, -0.0f), mVec3_c(0.0f, 0.0f, 0.0f));

    return SUCCEEDED;
}

int dAcOlavaF200_c::doDelete() {
    return SUCCEEDED;
}

int dAcOlavaF200_c::actorExecute() {
    mTexAnms[0].play();
    mTexAnms[1].play();
    mClrAnms[0].play();
    mClrAnms[1].play();
    return SUCCEEDED;
}

int dAcOlavaF200_c::draw() {
    for (int i = 0; i < 2; i++) {
        drawModelType1(&mModels[i]);
    }
    return SUCCEEDED;
}
