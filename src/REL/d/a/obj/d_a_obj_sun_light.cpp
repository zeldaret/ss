#include <d/a/obj/d_a_obj_sun_light.h>
#include <toBeSorted/arc_managers/current_stage_arc_manager.h>
#include <toBeSorted/spawn_struct.h>

SPECIAL_ACTOR_PROFILE(OBJ_SUN_LIGHT, dAcOsunLight_c, fProfile::OBJ_SUN_LIGHT, 0x0219, 0, 3);

STATE_DEFINE(dAcOsunLight_c, Wait);

extern "C" void fn_801B42D0(nw4r::g3d::ResFile *);
extern "C" void fn_801B4320(nw4r::g3d::ResFile *);

bool dAcOsunLight_c::createHeap() {
    // TODO stack order shuffle
    mBrres = CurrentStageArcManager::sInstance->getData("g3d/stage.brres");
    fn_801B42D0(&mBrres);
    fn_801B4320(&mBrres);
    nw4r::g3d::ResMdl mdl = mBrres.GetResMdl("StageF000Light");
    // This whole code is a bit weird again
    bool success1 = createModel(mdl);
    if (!success1) {
        return success1;
    }
    nw4r::g3d::ResAnmTexSrt srt = mBrres.GetResAnmTexSrt("StageF000Light");
    bool success2 = mTexAnm.create(mdl, srt, &heap_allocator, nullptr, 1);
    return success2 ? true : success2;
}

int dAcOsunLight_c::create() {
    if (!initAllocatorWork1Heap(-1, "dAcOsunLight_c::m_allocator", 0x20)) {
        return 2;
    }

    mModel.setAnm(mTexAnm);
    mStateMgr.changeState(StateID_Wait);
    mModel.setPriorityDraw(0x1C, 9);
    setBoundingBox(mVec3_c(-200.0f, -100.0f, -200.0f), mVec3_c(200.0f, 600.0f, 500.0f));
    return 1;
}

int dAcOsunLight_c::doDelete() {
    return 1;
}

int dAcOsunLight_c::actorExecute() {
    mStateMgr.executeState();
    if (isDay()) {
        mTexAnm.play();
    }
    updateMatrix();
    mModel.setLocalMtx(&worldMatrix.nw4rm);
    return 1;
}

int dAcOsunLight_c::draw() {
    if (isDay()) {
        drawModelType1(&mModel);
    }
    return 1;
}

void dAcOsunLight_c::initializeState_Wait() {}

void dAcOsunLight_c::executeState_Wait() {}

void dAcOsunLight_c::finalizeState_Wait() {}

bool dAcOsunLight_c::isDay() {
    return !SpawnStruct::sInstance.mNight;
}
