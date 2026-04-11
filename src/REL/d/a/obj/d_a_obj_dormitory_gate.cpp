#include "d/a/obj/d_a_obj_dormitory_gate.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/c/c_m3d_g_aab.h"
#include "d/d_stage.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_DORMITORY_GATE, dAcODormitoryGate_c, fProfile::OBJ_DORMITORY_GATE, 0x17A, 0, 3);

STATE_DEFINE(dAcODormitoryGate_c, Wait);
STATE_DEFINE(dAcODormitoryGate_c, End);

static const char *sGateMdls[4] = {
    "StageF000Gate",
    "StageF000GodDoor",
    "StageF000Shutter",
    "StageF400Gate",
};
static const char *sGateDZB[4] = {
    "dzb/StageF000Gate.dzb",
    "dzb/StageF000GodDoor.dzb",
    "dzb/StageF000Shutter.dzb",
    "dzb/StageF400Gate.dzb",
};
static const char *sGatePLC[4] = {
    "dat/StageF000Gate.plc",
    "dat/StageF000GodDoor.plc",
    "dat/StageF000Shutter.plc",
    "dat/StageF400Gate.plc",
};
bool dAcODormitoryGate_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres"));
    dStage_c::bindStageResToFile(&mResFile);
    dStage_c::bindSkyCmnToResFile(&mResFile);

    nw4r::g3d::ResMdl resMdl = mResFile.GetResMdl(sGateMdls[mSubtype]);
    if (mSubtype == TYPE_GATE || mSubtype == TYPE_GATE_F400) {
        mNumDoors = 2;
    } else {
        mNumDoors = 1;
    }

    // TODO: Rename - Opens the Doors
    fn_534_C80();
    fn_534_D40();
    for (int i = 0; i < mNumDoors; ++i) {
        if (!mMdls[i].create(resMdl, &mAllocator, 0x120, 1, nullptr)) {
            return false;
        }
        cBgD_t *dzb = (cBgD_t *)CurrentStageArcManager::GetInstance()->getData(sGateDZB[mSubtype]);
        PLC *plc = (PLC *)CurrentStageArcManager::GetInstance()->getData(sGatePLC[mSubtype]);
        if (mSubtype != TYPE_GODDOOR) {
            mMdls[i].setPriorityDraw(0x1C, -1);
        }

        // TODO: Return Type
        s32 tmp = !(BOOL)mCollision[i].Set(dzb, plc, 1, &mDoorMtx[i], &mScale);
        if (tmp != 0) {
            return false;
        }
    }

    if (mSubtype == TYPE_GODDOOR) {
        nw4r::g3d::ResMdl mdl = mResFile.GetResMdl(sGateMdls[mSubtype]);
        nw4r::g3d::ResAnmClr anmClr = mResFile.GetResAnmClr("StageF000GodDoor");
        if (!mAnmMatClr.create(mdl, anmClr, &mAllocator, nullptr, 1)) {
            return false;
        }
        mMdls[0].setAnm(mAnmMatClr);
        mAnmMatClr.setRate(0.f, 0);
        mScnCallback.attach(mMdls[0]);
    }
    return true;
}

int dAcODormitoryGate_c::create() {
    mSubtype = static_cast<Type_e>(getFromParams(8, 0xFF));
    if (mSubtype == 1 && StoryflagManager::sInstance->getFlag(STORYFLAG_GODDESS_STATUE_ACCESS)) {
        return FAILED;
    }

    if (mSubtype == TYPE_GATE_F400) {
        mStoryflag = getFromParams(16, 0x7FF);
    }

    CREATE_ALLOCATOR_SIZE(dAcODormitoryGate_c, 0x2000);

    for (int i = 0; i < mNumDoors; ++i) {
        dBgS::GetInstance()->Regist(&mCollision[i], this);
    }

    mStateMgr.changeState(StateID_Wait);

    mVec3_c min, max;
    switch (mSubtype) {
        case TYPE_GATE:
        case TYPE_GATE_F400:
            min = mVec3_c(-400.f, -0.f, -50.f);
            max = mVec3_c(400.f, 600.f, 250.f);
            break;
        case TYPE_SHUTTER:
            min = mVec3_c(-250.f, -0.f, -50.f);
            max = mVec3_c(250.f, 400.f, 50.f);
            break;
        case TYPE_GODDOOR:
            min = mVec3_c(-50.f, -0.f, -100.f);
            max = mVec3_c(500.f, 500.f, 100.f);
            break;
    }
    mBoundingBox.Set(min, max);

    if (mSubtype == TYPE_GODDOOR) {
        mPosition.copyFrom(mMdls[0].getResMdl().GetResNode(0).GetTranslate());
    }
    mWorldMtx.transS(mPosition);
    mWorldMtx.ZXYrotM(mRotation);
    mIsOpen = checkOpenFlag();

    return SUCCEEDED;
}

void dAcODormitoryGate_c::initializeState_Wait() {}
void dAcODormitoryGate_c::executeState_Wait() {}
void dAcODormitoryGate_c::finalizeState_Wait() {}
void dAcODormitoryGate_c::initializeState_End() {}
void dAcODormitoryGate_c::executeState_End() {}
void dAcODormitoryGate_c::finalizeState_End() {}
