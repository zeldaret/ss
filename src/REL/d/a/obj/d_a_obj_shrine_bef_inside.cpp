#include "d/a/obj/d_a_obj_shrine_bef_inside.h"

#include "d/col/bg/d_bg_s.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"

static const char *const AcOshrineBefInside__MODELS[] = {
    "StageF402Bef",
    "StageF402Bef_s",
};

SPECIAL_ACTOR_PROFILE(OBJ_SHRINE_BEF_INSIDE, dAcOshrineBefInside_c, fProfile::OBJ_SHRINE_BEF_INSIDE, 0x209, 0, 3);

bool dAcOshrineBefInside_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres"));

    for (int modelIndex = 0; modelIndex < 2; modelIndex++) {
        nw4r::g3d::ResMdl mdl = mResFile.GetResMdl(AcOshrineBefInside__MODELS[modelIndex]);
        TRY_CREATE(mModel[modelIndex].create(mdl, &heap_allocator, 0x120));
    }

    cBgD_t *dzb = (cBgD_t *)CurrentStageArcManager::GetInstance()->getData("dzb/StageF402Bef.dzb");
    PLC *plc = (PLC *)CurrentStageArcManager::GetInstance()->getData("dat/StageF402Bef.plc");

    updateMatrix();

    for (int i = 0; i < 2; i++) {
        mModel[i].setLocalMtx(mWorldMtx);
    }

    TRY_CREATE(!mCollision.Set(dzb, plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mCollision.Lock();

    return true;
}

int dAcOshrineBefInside_c::create() {
    CREATE_ALLOCATOR(dAcOshrineBefInside_c);
    dBgS::GetInstance()->Regist(&mCollision, this);
    mModel[0].setPriorityDraw(0x1C, 0x9);
    mModel[1].setPriorityDraw(0x22, 0x9);
    boundingBox.Set(mVec3_c(-2000.0f, 7400.0f, 7200.0f), mVec3_c(4400.0f, 10000.0f, 15800.0f));
    return SUCCEEDED;
}

int dAcOshrineBefInside_c::doDelete() {
    return SUCCEEDED;
}

int dAcOshrineBefInside_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOshrineBefInside_c::draw() {
    for (int i = 0; i < 2; i++) {
        drawModelType1(&mModel[i]);
    }
    return SUCCEEDED;
}
