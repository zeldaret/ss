#include <d/a/obj/d_a_obj_ring.h>
#include <d/d_player.h>

SPECIAL_ACTOR_PROFILE(OBJ_RING, dAcOring_c, fProfile::OBJ_RING, 0x00f2, 0, 0x103);

STATE_DEFINE(dAcOring_c, Move);

bool dAcOring_c::createHeap() {
    nw4r::g3d::ResFile f = getOarcResFile("PRing");
    nw4r::g3d::ResMdl mdl = f.GetResMdl("PeehatRing");
    // This matches but in a really weird way. Maybe an inline function?
    bool success = mModel.create(mdl, &heap_allocator, 0x20, 1, nullptr);
    bool rc = true;
    if (success == false) {
        rc = success;
    }
    return rc;
}

int dAcOring_c::create() {
    if (!initAllocatorWork1Heap(-1, "dAcOring_c::m_allocator", 0x20)) {
        return 2;
    }

    field_0x388 = getArgFromParams();

    setBoundingBox(mVec3_c(-100.0f, -100.0f, -100.0f), mVec3_c(100.0f, 100.0f, 100.0f));
    updateMatrix();
    mModel.setScale(scale);
    mModel.setLocalMtx(&worldMatrix.nw4rm);
    forwardAccel = -5.0f;
    forwardMaxSpeed = -40.0f;
    field_0x38C = dPlayer::LINK->position.y;
    mStateMgr.changeState(StateID_Move);
    return 1;
}

int dAcOring_c::doDelete() {
    return 1;
}

int dAcOring_c::actorExecute() {
    mStateMgr.executeState();
    calcVelocity();
    position += velocity;
    position += posIncrements;
    updateMatrix();
    mModel.setLocalMtx(&worldMatrix.nw4rm);
    return 1;
}

int dAcOring_c::draw() {
    drawModelType1(&mModel);
    return 1;
}

void dAcOring_c::initializeState_Move() {}

u32 dAcOring_c::ROT_PER_TICK = 0x1000;

void dAcOring_c::executeState_Move() {
    bool isCarried = false;
    if (mActorCarryInfo.isCarried == 1 && mActorCarryInfo.carryType == 7) {
        isCarried = true;
    }

    if (isCarried) {
        return;
    }
    rotation.x.mVal += ROT_PER_TICK;
    if (field_0x38C >= position.y) {
        deleteRequest();
    }
}

void dAcOring_c::finalizeState_Move() {}
