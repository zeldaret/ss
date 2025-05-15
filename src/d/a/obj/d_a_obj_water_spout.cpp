#include "d/a/obj/d_a_obj_water_spout.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_fanm.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(OBJ_WATER_SPOUT, dAcOwaterSpout_c, fProfile::OBJ_WATER_SPOUT, 0x1DA, 0, 6);

STATE_DEFINE(dAcOwaterSpout_c, Wait);

bool dAcOwaterSpout_c::createHeap() {
    void *data = getOarcResFile("FX_WaterColumn");
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("FX_WaterColumn");
    TRY_CREATE(mMdl.create(data, "FX_WaterColumn", "FX_WaterColumn", &heap_allocator, 0x32C));
    nw4r::g3d::ResAnmTexSrt anmSrt = mResFile.GetResAnmTexSrt("FX_WaterColumn");
    TRY_CREATE(mAnmSrt.create(mdl, anmSrt, &heap_allocator, nullptr, 1));
    nw4r::g3d::ResAnmClr anmClr = mResFile.GetResAnmClr("FX_WaterColumn");
    TRY_CREATE(mAnmClr.create(mdl, anmClr, &heap_allocator, nullptr, 1));
    return true;
}

int dAcOwaterSpout_c::create() {
    CREATE_ALLOCATOR(dAcOwaterSpout_c);
    mMdl.setAnm("FX_WaterColumn", m3d::PLAY_MODE_4);
    mMdl.getModel().setAnm(mAnmSrt);
    mMdl.getModel().setAnm(mAnmClr);
    updateMatrix();
    mMdl.getModel().setLocalMtx(mWorldMtx);
    mStateMgr.changeState(StateID_Wait);
    if (params == 1) {
        mScale.y *= 4.0f;
    }
    mMdl.getModel().setScale(mScale);
    boundingBox.Set(mVec3_c(-400.0f, -250.0f, -400.0f), mVec3_c(400.0f, 250.0f, 400.0f));
    return SUCCEEDED;
}

int dAcOwaterSpout_c::doDelete() {
    return SUCCEEDED;
}

int dAcOwaterSpout_c::actorExecute() {
    mStateMgr.executeState();
    mMdl.play();
    mAnmSrt.play();
    mAnmClr.play();
    return SUCCEEDED;
}

int dAcOwaterSpout_c::draw() {
    drawModelType1(&mMdl.getModel());
    return SUCCEEDED;
}

void dAcOwaterSpout_c::initializeState_Wait() {
    if (params == 1) {
        return;
    }
    dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_213_, position, nullptr, &mScale, nullptr, nullptr, 0, 0);
}
void dAcOwaterSpout_c::executeState_Wait() {
    if (mMdl.getAnm().isStop()) {
        deleteRequest();
    }
}
void dAcOwaterSpout_c::finalizeState_Wait() {}
