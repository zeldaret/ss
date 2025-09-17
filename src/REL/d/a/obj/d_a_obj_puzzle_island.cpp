#include "d/a/obj/d_a_obj_puzzle_island.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_stage.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

SPECIAL_ACTOR_PROFILE(OBJ_PUZZLE_ISLAND, dAcOpuzzleIsland_c, fProfile::OBJ_PUZZLE_ISLAND, 0x1D6, 0, 3);

void dAcOpuzzleIsland_c::rideCallback(dBgW *unknown, dAcObjBase_c *actor, dAcObjBase_c *interactor) {
    if (interactor->isActorPlayer() && actor != nullptr &&
        StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_ISLE_OF_SONGS_DISCOVERED) == 0) {
        StoryflagManager::sInstance->setFlag(STORYFLAG_ISLE_OF_SONGS_DISCOVERED);
    }
    return;
}

bool dAcOpuzzleIsland_c::createHeap() {
    void *data = getOarcResFile("IslPuz");
    mResFile = nw4r::g3d::ResFile(data);
    dStage_c::bindStageResToFile(&mResFile);
    dStage_c::bindSkyCmnToResFile(&mResFile);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("IslPuz");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x120, 1, nullptr));
    void *dzb = dAcObjBase_c::getOarcFile("IslPuz", "dzb/IslPuz.dzb");
    void *plc = dAcObjBase_c::getOarcFile("IslPuz", "dat/IslPuz.plc");
    updateMatrix();
    mMdl.setScale(mScale);
    mMdl.setLocalMtx(mWorldMtx);
    TRY_CREATE(!mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mBgW.Lock();
    return true;
}

int dAcOpuzzleIsland_c::create() {
    CREATE_ALLOCATOR(dAcOpuzzleIsland_c);
    mBgW.SetCrrFunc(dBgS_MoveBGProc_Typical);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mMdl.setPriorityDraw(0x1C, 9);
    mVec3_c min, max;
    mMdl.getBounds(&min, &max);
    boundingBox.Set(min, max);
    mCullingDistance = 500000.0f;
    mBgW.SetRideCallback(rideCallback);
    return SUCCEEDED;
}

int dAcOpuzzleIsland_c::doDelete() {
    return SUCCEEDED;
}

int dAcOpuzzleIsland_c::actorExecute() {
    mBgW.Move();
    return SUCCEEDED;
}

int dAcOpuzzleIsland_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}
