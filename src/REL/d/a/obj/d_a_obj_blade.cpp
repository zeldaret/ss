#include "d/a/obj/d_a_obj_blade.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/d_stage.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_BLADE, dAcOblade_c, fProfile::OBJ_BLADE, 0x1E4, 0, 4);

bool dAcOblade_c::createHeap() {
    void *data = CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres");
    mResFile = nw4r::g3d::ResFile(data);
    dStage_c::bindStageResToFile(&mResFile);
    dStage_c::bindSkyCmnToResFile(&mResFile);
    TRY_CREATE(mMdl.create(data, "StageF000Blade", "StageF000Blade", &mAllocator, 0x120));
    return true;
}

int dAcOblade_c::create() {
    CREATE_ALLOCATOR(dAcOblade_c);
    mMdl.setAnm("StageF000Blade", m3d::PLAY_MODE_4);
    updateMatrix();
    mMdl.getModel().setLocalMtx(mWorldMtx);
    boundingBox.Set(mVec3_c(-0.0f, -0.0f, -0.0f), mVec3_c(0.0f, 0.0f, 0.0f));
    return SUCCEEDED;
}

int dAcOblade_c::doDelete() {
    return SUCCEEDED;
}

int dAcOblade_c::actorExecute() {
    mMdl.play();
    return SUCCEEDED;
}

int dAcOblade_c::draw() {
    drawModelType1(&mMdl.getModel());
    return SUCCEEDED;
}
