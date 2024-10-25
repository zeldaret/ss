#include "d/a/obj/d_a_obj_bamboo_island.h"

#include "d/col/bg/d_bg_s.h"
#include "d/flag/storyflag_manager.h"
#include "toBeSorted/room_manager.h"

const f32 dAcObambooIsland_c::unusedFloat1 = 100000.0f;
const f32 dAcObambooIsland_c::unusedFloat2 = 0.0f;

static const char *const sBambooIslandNames[] = {
    "IslBamb",
    "IslBamb_s",
};

SPECIAL_ACTOR_PROFILE(OBJ_BAMBOO_ISLAND, dAcObambooIsland_c, fProfile::OBJ_BAMBOO_ISLAND, 0x01F9, 0, 3);

void dAcObambooIsland_c::rideCallback(dBgW *unknown, dAcObjBase_c *actor, dAcObjBase_c *interactor) {
    if (interactor->isActorPlayer() && actor != nullptr && StoryflagManager::sInstance->getCounterOrFlag(284) == 0) {
        StoryflagManager::sInstance->setFlag(284);
    }
}

bool dAcObambooIsland_c::createHeap() {
    mBrres = getOarcResFile("IslBamb");
    RoomManager::bindStageResToFile(&mBrres);
    RoomManager::bindSkyCmnToResFile(&mBrres);
    for (int i = 0; i < 2; i++) {
        nw4r::g3d::ResMdl mdl = mBrres.GetResMdl(sBambooIslandNames[i]);
        TRY_CREATE(mModels[i].create(mdl, &heap_allocator, 0x120));
    }
    cBgD_t *dzb = (cBgD_t *)getOarcFile("IslBamb", "dzb/IslBamb.dzb");
    PLC *plc = (PLC *)getOarcFile("IslBamb", "dat/IslBamb.plc");
    updateMatrix();
    for (int i = 0; i < 2; i++) {
        mModels[i].setLocalMtx(mWorldMtx);
    }
    TRY_CREATE(!mCollision.Set(dzb, plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mCollision.Lock();
    return true;
}

int dAcObambooIsland_c::create() {
    CREATE_ALLOCATOR(dAcObambooIsland_c);

    dBgS::GetInstance()->Regist(&mCollision, this);
    mModels[0].setPriorityDraw(0x1C, 9);
    mModels[1].setPriorityDraw(0x22, 9);
    boundingBox.Set(mVec3_c(-2400.0f, -1600.0f, -2300.0f), mVec3_c(2400.0f, 3900.0f, 2400.0f));
    mCullingDistance = 500000.0f;
    mCollision.SetRideCallback(rideCallback);
    return SUCCEEDED;
}

int dAcObambooIsland_c::doDelete() {
    return SUCCEEDED;
}

int dAcObambooIsland_c::actorExecute() {
    return SUCCEEDED;
}

int dAcObambooIsland_c::draw() {
    for (int i = 0; i < 2; i++) {
        drawModelType1(&mModels[i]);
    }
    return SUCCEEDED;
}
