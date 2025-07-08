#include "d/a/obj/d_a_obj_desert_debris.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_DESERT_DEBRIS, dAcOdesertDebris_c, fProfile::OBJ_DESERT_DEBRIS, 0x213, 0, 2);

bool dAcOdesertDebris_c::createHeap() {
    void *data = CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres");
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("StageF300_4Gareki");
    TRY_CREATE(mMdl.create(mdl, &heap_allocator, 0x120, 1, nullptr));
    void *dzb = CurrentStageArcManager::GetInstance()->getData("dzb/StageF300_4Gareki.dzb");
    void *plc = CurrentStageArcManager::GetInstance()->getData("dat/StageF300_4Gareki.plc");
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    TRY_CREATE(!mBgW.Set((cBgD_t *)dzb, (PLC *)plc, 1, &mWorldMtx, &mScale));
    mBgW.Lock();
    return mBgW.InitMapStuff(&heap_allocator);
}

int dAcOdesertDebris_c::create() {
    CREATE_ALLOCATOR(dAcOdesertDebris_c);
    dBgS::GetInstance()->Regist(&mBgW, this);
    dBgS::GetInstance()->RegistBg(&mBgW, this);
    mStoryFlag = getFromParams(0, 0x7FF);
    if (checkStoryFlag()) {
        return FAILED;
    }
    mMdl.setPriorityDraw(0x1C, 9);
    mVec3_c min, max;
    mMdl.getBounds(&min, &max);
    boundingBox.Set(min, max);
    mCullingDistance = 50000.0f;
    return SUCCEEDED;
}

int dAcOdesertDebris_c::doDelete() {
    dBgS::GetInstance()->UnRegist(&mBgW);
    return SUCCEEDED;
}

int dAcOdesertDebris_c::actorExecute() {
    if (checkStoryFlag()) {
        deleteRequest();
    }
    return SUCCEEDED;
}

int dAcOdesertDebris_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

bool dAcOdesertDebris_c::checkStoryFlag() {
    return mStoryFlag < 0x7FF && StoryflagManager::sInstance->getCounterOrFlag(mStoryFlag);
}
