#include "d/a/npc/d_a_npc_rival_lod.h"

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_fanm.h"
#include "s/s_State.hpp"

SPECIAL_ACTOR_PROFILE(NPC_RIVAL_LOD, dAcNpcRivalLOD_c, fProfile::NPC_RIVAL_LOD, 0x32, 0, 3);

STATE_DEFINE(dAcNpcRivalLOD_c, Wait);

bool dAcNpcRivalLOD_c::createHeap() {
    void *oarc = getOarcFile("RivalLOD", "g3d/model.brres");
    TRY_CREATE(mMdl.create(oarc, "RivalLOD", "RivalLOD_wait", &mAllocator, 0x120));
    return true;
}
int dAcNpcRivalLOD_c::create() {
    if (!initAllocatorWork1Heap(-1, "dAcNpcRivalLOD_c::m_allocator", 0x20)) {
        return FAILED;
    }
    0.0f; // float order, cool
    mAcceleration = -1.0f;
    mMaxSpeed = -40.0f;
    setState(StateID_Wait);
    mBoundingBox.Set(mVec3_c(-110.0f, -30.0f, -130.0f), mVec3_c(130.0f, 190.0f, 110.0f));
    return SUCCEEDED;
}

int dAcNpcRivalLOD_c::doDelete() {
    return SUCCEEDED;
}

int dAcNpcRivalLOD_c::actorExecute() {
    executeState();
    updateMatrix();
    mMdl.getModel().setLocalMtx(mWorldMtx);
    return SUCCEEDED;
}

int dAcNpcRivalLOD_c::draw() {
    drawModelType1(&mMdl.getModel());
    return SUCCEEDED;
}

void dAcNpcRivalLOD_c::initializeState_Wait() {
    mMdl.setAnm("RivalLOD_wait", m3d::PLAY_MODE_4, 4.0f);
}
void dAcNpcRivalLOD_c::executeState_Wait() {
    mMdl.play();
}
void dAcNpcRivalLOD_c::finalizeState_Wait() {}
