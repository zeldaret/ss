#include "d/a/obj/d_a_obj_tenijima.h"

#include "d/col/bg/d_bg_s.h"
#include "d/flag/sceneflag_manager.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_TENIJIMA, dAcOtenijima_c, fProfile::OBJ_TENIJIMA, 0x261, 0, 0);

bool dAcOtenijima_c::createHeap() {
    const char *mdlName = getModelName();
    mResFile = nw4r::g3d::ResFile(CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres"));
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl(mdlName);
    bool result;
    TRY_CREATE2(mModel.create(mdl, &mAllocator, 0x120));

    const char *dzbName = getDzbName();
    const char *plcName = getPlcName();
    if (!mIsOpenType1) {
        void *dzb = CurrentStageArcManager::GetInstance()->getData(dzbName);
        void *plc = CurrentStageArcManager::GetInstance()->getData(plcName);
        result = !mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale);
    }
    if (!result) {
        return result;
    }
    return true;
}

int dAcOtenijima_c::create() {
    mIsOpened = false;
    u32 type = getFromParams(8, 0xF);
    u16 flag = getFromParams(0, 0xFF);
    mIsOpenType1 = false;
    if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, flag)) {
        mIsOpened = true;
    }
    if (type == 1) {
        mIsOpenType1 = true;
    } else {
        mIsOpenType1 = false;
    }
    updateMatrix();
    CREATE_ALLOCATOR(dAcOtenijima_c);
    mModel.setLocalMtx(mWorldMtx);
    if (!mIsOpenType1) {
        dBgS::GetInstance()->Regist(&mBgW, this);
        mBgW.Lock();
        mModel.setPriorityDraw(0x1C, 9);
    }
    mCullingDistance = 1000000.0f;
    return SUCCEEDED;
}

int dAcOtenijima_c::doDelete() {
    return SUCCEEDED;
}

int dAcOtenijima_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOtenijima_c::draw() {
    drawModel(&mModel);
    return SUCCEEDED;
}

const char *dAcOtenijima_c::getModelName() {
    if (!mIsOpened) {
        return mIsOpenType1 ? "StageD301_F301_6_00" : "StageTeniIsl_CL";
    } else {
        return mIsOpenType1 ? "StageD301_F301_6_01" : "StageTeniIsl_OP";
    }
}

const char *dAcOtenijima_c::getDzbName() {
    return mIsOpened ? "dzb/StageTeniIsl_OP.dzb" : "dzb/StageTeniIsl_Cl.dzb";
}

const char *dAcOtenijima_c::getPlcName() {
    return mIsOpened ? "dat/StageTeniIsl_OP.plc" : "dat/StageTeniIsl_Cl.plc";
}
