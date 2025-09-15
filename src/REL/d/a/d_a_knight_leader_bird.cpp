#include "d/a/d_a_knight_leader_bird.h"

#include "d/a/d_a_bird_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_fanm.h"
#include "m/m_vec.h"

SPECIAL_ACTOR_PROFILE(KNIGHT_LEADER_BIRD, dAcKnightLeaderBird_c, fProfile::KNIGHT_LEADER_BIRD, 0x8E, 0, 2);

STATE_DEFINE(dAcKnightLeaderBird_c, Wait);

const char *sBirdLordMdlResName = "BirdLord";
const char *sBirdLordAnmResName = "BirdAnm";

const char *sBirdLordMdlName = "BirdLord";
const char *sBirdLordAnmName = "Glide";
// unused, can't be static
const char *sBirdLordAnmName2 = "Flap";
const char *sBirdLordAnmName3 = "Keep";

bool dAcKnightLeaderBird_c::createHeap() {
    void *mdl = getOarcResFile(sBirdLordMdlResName);
    void *anm = getOarcResFile(sBirdLordAnmResName);
    if (!mMdl.create3(*this, mdl, anm, sBirdLordMdlName, sBirdLordAnmName, 0x120)) {
        return false;
    }
    loadMdlNodeIds();
    return true;
}

int dAcKnightLeaderBird_c::create() {
    1.0f; // float order

    CREATE_ALLOCATOR(dAcKnightLeaderBird_c);
    dAcBirdBase_c::resetBird();
    field_0xF40 = 0.15f;
    mStateMgr.changeState(StateID_Wait);
    boundingBox.Set(mVec3_c(-300.0f, 0.0f, -300.0f), mVec3_c(300.0f, 200.0f, 300.0f));

    return SUCCEEDED;
}

int dAcKnightLeaderBird_c::doDelete() {
    return SUCCEEDED;
}

int dAcKnightLeaderBird_c::actorExecute() {
    mStateMgr.executeState();
    mMdl.play();

    fn_80176D30();
    fn_80176F80();
    updateMatrixBird();
    
    updateSpineMtx();

    fn_80177EA0();
    fn_80178000();
    fn_801780C0();
    fn_80177F30();

    return SUCCEEDED;
}

int dAcKnightLeaderBird_c::preDraw() {
    // @bug boolean conversion...
    return dAcObjBase_c::preDraw() != false;
}

int dAcKnightLeaderBird_c::draw() {
    dAcBirdBase_c::draw();
    return SUCCEEDED;
}


void dAcKnightLeaderBird_c::updateSpineMtx() {
    static const char *sBirdLordSpineName = "Spine";
    u32 id = mMdl.getModel().getNodeID(sBirdLordSpineName);
    if (!mMdl.getModel().getNodeWorldMtx(id, mSpineWorldMtx)) {
        mSpineWorldMtx = mWorldMtx;
        return;
    }

    mSpineWorldMtx.YrotM(0x4000);
}

void dAcKnightLeaderBird_c::initializeState_Wait() {
    forwardSpeed = 0.0f;
    velocity.set(0.0f, 0.0f, 0.0f);
    forwardAccel = 0.0f;
    changeBirdAction(sBirdLordAnmName, BIRD_ACTION_GLIDE, m3d::PLAY_MODE_4, 8.0f);
}
void dAcKnightLeaderBird_c::executeState_Wait() {}
void dAcKnightLeaderBird_c::finalizeState_Wait() {}
