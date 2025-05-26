#include "d/a/obj/d_a_obj_island_LOD.h"
#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/d_stage.h"
#include "f/f_base.h"
#include "toBeSorted/blur_and_palette_manager.h"

const char* const dAcOislandLOD_c::resFilenames[5] = {
    "IslLODA",
    "IslLODB",
    "IslLODC",
    "IslLODD",
    "IslLODE",
};

const char* const dAcOislandLOD_c::resMdlNames[5] = {
    "IslLODA",
    "IslLODB",
    "IslLODC",
    "IslLODD",
    "IslLODE",
};

SPECIAL_ACTOR_PROFILE(OBJ_ISLAND_LOD, dAcOislandLOD_c, fProfile::OBJ_ISLAND_LOD, 0x211, 0, 3);

bool dAcOislandLOD_c::createHeap() {
    lodModelIndex = params & 0xf;
    void* data = getOarcResFile(dAcOislandLOD_c::resFilenames[lodModelIndex]);
    resFile = nw4r::g3d::ResFile(data);
    dStage_c::bindStageResToFile(&resFile);
    dStage_c::bindSkyCmnToResFile(&resFile);
    nw4r::g3d::ResMdl mdl = resFile.GetResMdl(dAcOislandLOD_c::resMdlNames[lodModelIndex]);
    TRY_CREATE(mMdl.create(mdl, &heap_allocator, 0x120, 1, nullptr));
    return true;
}

int dAcOislandLOD_c::create() {
    CREATE_ALLOCATOR(dAcOislandLOD_c);
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mVec3_c min, max;
    mMdl.getBounds(&min, &max);
    boundingBox.Set(min, max);
    mCullingDistance = 500000.0;
    return SUCCEEDED;
}

int dAcOislandLOD_c::doDelete() {
    return SUCCEEDED;
}
int dAcOislandLOD_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOislandLOD_c::draw() {
    BlurAndPaletteManager& blurAndPaletteManager = BlurAndPaletteManager::GetInstance();
    if (blurAndPaletteManager.fn_80024770(7) && !blurAndPaletteManager.fn_800247A0(7)) {
        return SUCCEEDED;
    }
    drawModelType1(&mMdl);
    return SUCCEEDED;
}
