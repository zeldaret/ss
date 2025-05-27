#include "d/a/obj/d_a_obj_distant_ship.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/flag/sceneflag_manager.h"
#include "f/f_base.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_DIS_SHIP, dAcOdistantShip_c, fProfile::OBJ_DIS_SHIP, 0x143, 0, 0);

bool dAcOdistantShip_c::createHeap() {
    char *modelName = getModelName();
    mBrres = (nw4r::g3d::ResFile)CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres");
    TRY_CREATE(mModel.create(mBrres.GetResMdl(modelName), &heap_allocator, 0x120, 1, 0));
    return SUCCEEDED;
}

int dAcOdistantShip_c::create() {
    mIsVisible = false;
    u8 flag = getFromParams(0, 0xFF);
    mSubtype = getFromParams(8, 0xF);

    if (SceneflagManager::sInstance->checkBoolFlag(roomid, flag)) {
        mIsVisible = true;
    }

    if (mIsVisible) {
        CREATE_ALLOCATOR(dAcOdistantShip_c);
        updateMatrix();
        mModel.setLocalMtx(mWorldMtx);
        boundingBox.Set(mVec3_c(-0.0f, -0.0f, -0.0f), mVec3_c(0.0f, 0.0f, 0.0f));
    }
    return SUCCEEDED;
}

int dAcOdistantShip_c::doDelete() {
    return SUCCEEDED;
}

int dAcOdistantShip_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOdistantShip_c::draw() {
    if (mIsVisible) {
        drawModelType1(&mModel);
    }
    return SUCCEEDED;
}

char *dAcOdistantShip_c::getModelName() {
    switch (mSubtype) {
        case 0:  return "StageF301_D300";
        case 1:  return "StageF301_3_D300";
        default: return nullptr;
    }
}
