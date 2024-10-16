#include "d/a/obj/d_a_obj_sun_light.h"

#include "toBeSorted/arc_managers/current_stage_arc_manager.h"
#include "toBeSorted/room_manager.h"
#include "toBeSorted/scgame.h"


SPECIAL_ACTOR_PROFILE(OBJ_SUN_LIGHT, dAcOsunLight_c, fProfile::OBJ_SUN_LIGHT, 0x0219, 0, 3);

STATE_DEFINE(dAcOsunLight_c, Wait);

bool dAcOsunLight_c::createHeap() {
    mBrres = CurrentStageArcManager::sInstance->getData("g3d/stage.brres");
    RoomManager::bindStageResToFile(&mBrres);
    RoomManager::bindSkyCmnToResFile(&mBrres);
    nw4r::g3d::ResMdl mdl = mBrres.GetResMdl("StageF000Light");
    TRY_CREATE(mModel.create(mdl, &heap_allocator, 0x120));
    nw4r::g3d::ResAnmTexSrt srt = mBrres.GetResAnmTexSrt("StageF000Light");
    TRY_CREATE(mTexAnm.create(mdl, srt, &heap_allocator, nullptr, 1));
    return true;
}

int dAcOsunLight_c::create() {
    CREATE_ALLOCATOR(dAcOsunLight_c);

    mModel.setAnm(mTexAnm);
    mStateMgr.changeState(StateID_Wait);
    mModel.setPriorityDraw(0x1C, 9);
    setBoundingBox(mVec3_c(-200.0f, -100.0f, -200.0f), mVec3_c(200.0f, 600.0f, 500.0f));
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
    // return !ScGame::currentSpawnInfo.isNight();
    // TODO TempFix?
    return !(u8)ScGame::currentSpawnInfo.night;
}
