#include "d/a/obj/d_a_obj_F302_light.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/flag/sceneflag_manager.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"
#include "toBeSorted/time_area_mgr.h"

SPECIAL_ACTOR_PROFILE(OBJ_F302_LIGHT, dAcOF302Light_c, fProfile::OBJ_F302_LIGHT, 0x1B2, 0, 3);

STATE_DEFINE(dAcOF302Light_c, Wait);
STATE_DEFINE(dAcOF302Light_c, SwitchOn);

const char *dAcOF302Light_c::sResAnmClrNames[2] = {
    "Teniobj_0_APR",
    "Teniobj_0_LP",
};

bool dAcOF302Light_c::createHeap() {
    void *data = CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres");
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("Teniobj_0");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x120));

    for (s32 i = 0; i < 2; i++) {
        nw4r::g3d::ResAnmClr resAnmClr = mResFile.GetResAnmClr(sResAnmClrNames[i]);
        TRY_CREATE(mAnmMatClr[i].create(mdl, resAnmClr, &mAllocator, nullptr, 1));
        mAnmMatClr[i].setRate(0.f, 0);
    }
    return true;
}

int dAcOF302Light_c::actorCreate() {
    field_0x440 = getFromParams(0, 0xFF);
    CREATE_ALLOCATOR(dAcOF302Light_c);
    forwardAccel = 0.f;
    forwardMaxSpeed = -40.f;
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    if (field_0x440 < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(roomid, field_0x440)) {
        mStateMgr.changeState(StateID_Wait);
    } else {
        mStateMgr.changeState(StateID_SwitchOn);
    }
    boundingBox.Set(mVec3_c(-660.f, -40.f, -660.f), mVec3_c(660.f, 0.f, 660.f));
    return SUCCEEDED;
}

int dAcOF302Light_c::actorPostCreate() {
    return SUCCEEDED;
}

int dAcOF302Light_c::doDelete() {
    return SUCCEEDED;
}

int dAcOF302Light_c::actorExecute() {
    mStateMgr.executeState();

    for (s32 i = 0; i < 2; i++) {
        mAnmMatClr[i].play();
    }
    return SUCCEEDED;
}

int dAcOF302Light_c::actorExecuteInEvent() {
    switch (mEvent.getCurrentEventCommand()) {
        case 'act0': {
            mEvent.advanceNext();
            break;
        }
        case 'act1': {
            mAnmMatClr[0].setRate(1.f, 0);
            if (mAnmMatClr[0].isStop(0)) {
                if (mStateMgr.isState(StateID_Wait)) {
                    mStateMgr.changeState(StateID_SwitchOn);
                }
                mEvent.advanceNext();
            }
        }
    }
    mStateMgr.executeState();

    for (s32 i = 0; i < 2; i++) {
        mAnmMatClr[i].play();
    }
    return SUCCEEDED;
}

int dAcOF302Light_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOF302Light_c::initializeState_Wait() {
    mMdl.setAnm(mAnmMatClr[0]);
    mAnmMatClr[0].setRate(0.f, 0);
}

void dAcOF302Light_c::executeState_Wait() {}
void dAcOF302Light_c::finalizeState_Wait() {}

void dAcOF302Light_c::initializeState_SwitchOn() {
    mMdl.setAnm(mAnmMatClr[1]);
    mTimeArea.check(roomid, mPosition, 0, 30.f, 0.1f);
    if (dTimeAreaMgr_c::GetInstance()->fn_800B9B60(roomid, mPosition)) {
        mAnmMatClr[1].setRate(1.f, 0);
        mAnmMatClr[1].setFrame(40.f, 0);
    } else {
        mAnmMatClr[1].setRate(0.f, 0);
    }
}

void dAcOF302Light_c::executeState_SwitchOn() {
    mTimeArea.check(roomid, mPosition, 0, 30.f, 0.1f);
    if (dTimeAreaMgr_c::GetInstance()->fn_800B9B60(roomid, mPosition)) {
        mAnmMatClr[1].setRate(1.f, 0);
    } else {
        if (mAnmMatClr[1].getFrame(0) == 0.f) {
            mAnmMatClr[1].setRate(0.f, 0);
        }
    }
}

void dAcOF302Light_c::finalizeState_SwitchOn() {}
