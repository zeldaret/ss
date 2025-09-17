#include "d/a/obj/d_a_obj_ring.h"

#include "d/a/d_a_player.h"
#include "d/d_linkage.h"
#include "m/m_angle.h"
#include "nw4r/g3d/res/g3d_resfile.h"

SPECIAL_ACTOR_PROFILE(OBJ_RING, dAcOring_c, fProfile::OBJ_RING, 0x00f2, 0, 0x103);

STATE_DEFINE(dAcOring_c, Move);

bool dAcOring_c::createHeap() {
    nw4r::g3d::ResFile f(getOarcResFile("PRing"));
    nw4r::g3d::ResMdl mdl = f.GetResMdl("PeehatRing");
    TRY_CREATE(mModel.create(mdl, &heap_allocator, 0x20, 1, nullptr));
    return true;
}

int dAcOring_c::create() {
    CREATE_ALLOCATOR(dAcOring_c);

    field_0x388 = getArgFromParams();

    boundingBox.Set(mVec3_c(-100.0f, -100.0f, -100.0f), mVec3_c(100.0f, 100.0f, 100.0f));
    updateMatrix();
    mModel.setScale(mScale);
    mModel.setLocalMtx(mWorldMtx);
    forwardAccel = -5.0f;
    forwardMaxSpeed = -40.0f;
    field_0x38C = dAcPy_c::LINK->mPosition.y;
    mStateMgr.changeState(StateID_Move);
    return SUCCEEDED;
}

int dAcOring_c::doDelete() {
    return SUCCEEDED;
}

int dAcOring_c::actorExecute() {
    mStateMgr.executeState();
    calcVelocity();
    mPosition += velocity;
    mPosition += mStts.mCcMove;
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);
    return SUCCEEDED;
}

int dAcOring_c::draw() {
    drawModelType1(&mModel);
    return SUCCEEDED;
}

void dAcOring_c::initializeState_Move() {}

void dAcOring_c::executeState_Move() {
    if (GetLinkage().checkConnection(dLinkage_c::CONNECTION_7)) {
        return;
    }
    mRotation.addX(0x1000);
    if (field_0x38C >= mPosition.y) {
        deleteRequest();
    }
}

void dAcOring_c::finalizeState_Move() {}
