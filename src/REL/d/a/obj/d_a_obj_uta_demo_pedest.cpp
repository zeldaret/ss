#include "d/a/obj/d_a_obj_uta_demo_pedest.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/d_stage.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"

static const char *const sMdlNames[] = {
    "StageF010rPedest",
    "StageWindF001r",
};

SPECIAL_ACTOR_PROFILE(OBJ_UTA_DEMO_PEDEST, dAcOutaDemoPedest_c, fProfile::OBJ_UTA_DEMO_PEDEST, 0x212, 0, 6);

bool dAcOutaDemoPedest_c::createHeap() {
    mModelType = getModelType();
    if (mModelType == 0xF) {
        mModelType = 0;
    }
    if (mModelType > 1) {
        mModelType = 0;
    }

    mRes = nw4r::g3d::ResFile(CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres"));
    dStage_c::bindStageResToFile(&mRes);
    dStage_c::bindSkyCmnToResFile(&mRes);
    nw4r::g3d::ResMdl mdl = mRes.GetResMdl(sMdlNames[mModelType]);
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x120));

    return true;
}

int dAcOutaDemoPedest_c::create() {
    CREATE_ALLOCATOR(dAcOutaDemoPedest_c);

    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.setPriorityDraw(0x1C, 9);
    mVec3_c l, u;
    mMdl.getBounds(&l, &u);
    boundingBox.Set(l, u);

    return SUCCEEDED;
}

int dAcOutaDemoPedest_c::doDelete() {
    return SUCCEEDED;
}

int dAcOutaDemoPedest_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOutaDemoPedest_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}
