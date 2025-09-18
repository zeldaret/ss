#include "d/a/obj/d_a_obj_sun_light.h"

#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_SUN_LIGHT, dAcOsunLight_c, fProfile::OBJ_SUN_LIGHT, 0x0219, 0, 3);

STATE_DEFINE(dAcOsunLight_c, Wait);

bool dAcOsunLight_c::createHeap() {
    mBrres = nw4r::g3d::ResFile(CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres"));
    dStage_c::bindStageResToFile(&mBrres);
    dStage_c::bindSkyCmnToResFile(&mBrres);
    nw4r::g3d::ResMdl mdl = mBrres.GetResMdl("StageF000Light");
    TRY_CREATE(mModel.create(mdl, &mAllocator, 0x120));
    nw4r::g3d::ResAnmTexSrt srt = mBrres.GetResAnmTexSrt("StageF000Light");
    TRY_CREATE(mTexAnm.create(mdl, srt, &mAllocator, nullptr, 1));
    return true;
}

int dAcOsunLight_c::create() {
    CREATE_ALLOCATOR(dAcOsunLight_c);

    mModel.setAnm(mTexAnm);
    mStateMgr.changeState(StateID_Wait);
    mModel.setPriorityDraw(0x1C, 9);
    mBoundingBox.Set(mVec3_c(-200.0f, -100.0f, -200.0f), mVec3_c(200.0f, 600.0f, 500.0f));
    return SUCCEEDED;
}

int dAcOsunLight_c::doDelete() {
    return SUCCEEDED;
}

int dAcOsunLight_c::actorExecute() {
    mStateMgr.executeState();
    if (isDay()) {
        mTexAnm.play();
    }
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);
    return SUCCEEDED;
}

int dAcOsunLight_c::draw() {
    if (isDay()) {
        drawModelType1(&mModel);
    }
    return SUCCEEDED;
}

void dAcOsunLight_c::initializeState_Wait() {}

void dAcOsunLight_c::executeState_Wait() {}

void dAcOsunLight_c::finalizeState_Wait() {}

bool dAcOsunLight_c::isDay() {
    return dScGame_c::currentSpawnInfo.getTimeOfDay() == SpawnInfo::DAY;
}
