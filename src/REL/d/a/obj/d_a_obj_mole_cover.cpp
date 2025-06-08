#include "d/a/obj/d_a_obj_mole_cover.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_stage.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_MOLE_COVER, dAcOmoleCover_c, fProfile::OBJ_MOLE_COVER, 0x20F, 0, 3);

bool dAcOmoleCover_c::createHeap() {
    void *data = CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres");
    mResFile = nw4r::g3d::ResFile(data);
    dStage_c::bindStageResToFile(&mResFile);
    dStage_c::bindSkyCmnToResFile(&mResFile);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("StageF000MallCover");
    TRY_CREATE(mMdl.create(mdl, &heap_allocator, 0x120, 1, nullptr));
    void *dzb = CurrentStageArcManager::GetInstance()->getData("dzb/StageF000MallCover.dzb");
    void *plc = CurrentStageArcManager::GetInstance()->getData("dat/StageF000MallCover.plc");
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    TRY_CREATE(!mBgW.Set((cBgD_t *)dzb, (PLC *)plc, 1, &mWorldMtx, &mScale));
    mBgW.Lock();
    return true;
}

int dAcOmoleCover_c::create() {
    CREATE_ALLOCATOR(dAcOmoleCover_c);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mMdl.setPriorityDraw(0x1C, 9);
    boundingBox.Set(mVec3_c(-1900.0f, 1100.0f, -5500.0f), mVec3_c(100.0f, 1700.0f, -1800.0f));
    mCullingDistance = FLOAT_MAX;
    return SUCCEEDED;
}

int dAcOmoleCover_c::doDelete() {
    return SUCCEEDED;
}

int dAcOmoleCover_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOmoleCover_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}
