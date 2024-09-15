#include <d/a/obj/d_a_obj_ring.h>
#include <d/d_player.h>

SPECIAL_ACTOR_PROFILE(OBJ_RING, dAcOring_c, fProfile::OBJ_RING, 0x00f2, 0, 0x103);

STATE_DEFINE(dAcOring_c, Move);

bool dAcOring_c::createHeap() {
    nw4r::g3d::ResFile f = getOarcResFile("PRing");
    nw4r::g3d::ResMdl mdl = f.GetResMdl("PeehatRing");
    // This matches but in a really weird way. Maybe an inline function?
    TRY_CREATE(mModel.create(mdl, &heap_allocator, 0x20, 1, nullptr));
    return true;
}

int dAcOring_c::create() {
    CREATE_ALLOCATOR(dAcOring_c);

    field_0x388 = getArgFromParams();

    setBoundingBox(mVec3_c(-100.0f, -100.0f, -100.0f), mVec3_c(100.0f, 100.0f, 100.0f));
    updateMatrix();
    mModel.setScale(scale);
    mModel.setLocalMtx(worldMatrix);
    forwardAccel = -5.0f;
    forwardMaxSpeed = -40.0f;
    field_0x38C = dPlayer::LINK->position.y;
    mStateMgr.changeState(StateID_Move);
    return SUCCEEDED;
}

int dAcOring_c::doDelete() {
    return SUCCEEDED;
}

int dAcOring_c::actorExecute() {
    mStateMgr.executeState();
    calcVelocity();
    position += velocity;
    position += posIncrements;
    updateMatrix();
    mModel.setLocalMtx(worldMatrix);
    return SUCCEEDED;
}

int dAcOring_c::draw() {
    drawModelType1(&mModel);
    return SUCCEEDED;
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
