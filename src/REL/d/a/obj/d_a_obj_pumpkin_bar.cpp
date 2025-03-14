#include "d/a/obj/d_a_obj_pumpkin_bar.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_stage.h"
#include "d/flag/storyflag_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_PUMPKIN_BAR, dAcOpumpkinBar_c, fProfile::OBJ_PUMPKIN_BAR, 0x1ED, 0, 3);

void dAcOpumpkinBar_c::barCollisionRideCallback(dBgW *collider, dAcObjBase_c *actor, dAcObjBase_c *interactor) {
    if (interactor->isActorPlayer() && actor != 0 && !StoryflagManager::sInstance->getCounterOrFlag(282)) {
        StoryflagManager::sInstance->setFlag(282);
    }
}

bool dAcOpumpkinBar_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(getOarcResFile("IslBar"));
    dStage_c::bindStageResToFile(&mResFile);
    dStage_c::bindSkyCmnToResFile(&mResFile);

    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("IslBar");
    TRY_CREATE(mModel.create(mdl, &heap_allocator, 0x120));

    void *dzb = getOarcFile("IslBar", "dzb/IslBar.dzb");
    void *plc = getOarcFile("IslBar", "dat/IslBar.plc");
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);
    TRY_CREATE(!mBarCollision.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mBarCollision.Lock();

    dzb = getOarcFile("IslBar", "dzb/IslBar_Bird.dzb");
    plc = getOarcFile("IslBar", "dat/IslBar_Bird.plc");
    TRY_CREATE(!mBarBirdCollision.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mBarBirdCollision.Lock();
    return true;
}

int dAcOpumpkinBar_c::create() {
    CREATE_ALLOCATOR(dAcOpumpkinBar_c);
    dBgS::GetInstance()->Regist(&mBarCollision, this);
    dBgS::GetInstance()->Regist(&mBarBirdCollision, this);
    mBarBirdCollision.SetField_0x22(2);
    mModel.setPriorityDraw(0x1C, 0x9);
    // 100000.0 is in the rodata and the instructions need it.
    // This also works if a separate variable is used but I
    // think it's more likely that mCullingDistance got set twice.
    mCullingDistance = 100000.0f;
    boundingBox.Set(mVec3_c(-2790.0f, -1450.0f, -8270.0f), mVec3_c(2880.0f, 2120.0f, 1370.0f));
    mCullingDistance = 500000.0f;
    mBarCollision.SetRideCallback(barCollisionRideCallback);
    return SUCCEEDED;
}

int dAcOpumpkinBar_c::doDelete() {
    return SUCCEEDED;
}

int dAcOpumpkinBar_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOpumpkinBar_c::draw() {
    drawModelType1(&mModel);
    return SUCCEEDED;
}

dAcOpumpkinBar_c::~dAcOpumpkinBar_c() {}
