#include <d/a/obj/d_a_obj_spike.h>
#include <toBeSorted/cc/d_cc_mgr.h>

SPECIAL_ACTOR_PROFILE(OBJ_SPIKE, dAcOspike_c, fProfile::OBJ_SPIKE, 0x1D9, 0, 2);

// clang-format off
dCcD_SrcAabb dAcOspike_c::sCcSrc = {
    {{{0x400}, 0x2003F, 0, 1, 0, 0, 0, 0, 0}, 
    {{0xFEB77DFF}, 0x111, 0x60407, 0, 0}, 
    {0}},
    {-1.0f, -246.0f, -472.0f, 73.0f, 255.0f, 482.0f}};
// clang-format on

const mVec3_c dAcOspike_c::sVec1 = mVec3_c(-1.0f, -246.0f, -472.0f);
const mVec3_c dAcOspike_c::sVec2 = mVec3_c(73.0f, 255.0f, 482.0f);

STATE_DEFINE(dAcOspike_c, Wait);

bool dAcOspike_c::createHeap() {
    mResFile = getOarcResFile("SpikeD101");
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("SpikeD101");
    TRY_CREATE(mMdl.create(mdl, &heap_allocator, 0x120));
    return true;
}

int dAcOspike_c::create() {
    if (!initAllocatorWork1Heap(-1, "dAcOspike_c::m_allocator", 0x20)) {
        return FAILED;
    }

    mCCdStruct.setField0x38ToMagicValue();
    mCollision.init(sCcSrc);
    mCollision.initUnk(mCCdStruct);

    updateMatrix();
    mMdl.setLocalMtx(worldMatrix);
    mVec3_c tmp;
    PSMTXMultVecSR(worldMatrix.m, mVec3_c::Ex, tmp);
    mCollision.setAtVec(tmp);

    mMtx_c mtx;
    mtx.XrotS(rotation.x);
    mtx.ZrotM(rotation.z);
    mVec3_c tmp2, tmp3;
    PSMTXMultVecSR(mtx.m, sVec1, tmp2);
    PSMTXMultVecSR(mtx.m, sVec2, tmp3);

    if (tmp2.x > tmp3.x) {
        f32 copy = tmp2.x;
        tmp2.x = tmp3.x;
        tmp3.x = copy;
    }
    if (tmp2.y > tmp3.y) {
        f32 copy = tmp2.y;
        tmp2.y = tmp3.y;
        tmp3.y = copy;
    }
    if (tmp2.z > tmp3.z) {
        f32 copy = tmp2.z;
        tmp2.z = tmp3.z;
        tmp3.z = copy;
    }

    mCollision.setMinMax(tmp2, tmp3);
    mStateMgr.changeState(StateID_Wait);
    setBoundingBox(mVec3_c(-10.0f, -250.0f, -480.0f), mVec3_c(80.0f, 260.0f, 490.0f));
    return SUCCEEDED;
}

int dAcOspike_c::doDelete() {
    return 1;
}

int dAcOspike_c::actorExecute() {
    mStateMgr.executeState();
    mCollision.setWithYRot(position, rotation.y);
    ColliderManager::getColliderManager()->addCollider(&mCollision);
    return 1;
}

int dAcOspike_c::draw() {
    drawModelType1(&mMdl);
    return 1;
}

void dAcOspike_c::initializeState_Wait() {}
void dAcOspike_c::executeState_Wait() {}
void dAcOspike_c::finalizeState_Wait() {}
