#include <c/c_math.h>
#include <d/a/obj/d_a_obj_triforce.h>
#include <m/m_vec.h>
#include <toBeSorted/cc/d_cc_mgr.h>

SPECIAL_ACTOR_PROFILE(OBJ_TRIFORCE, dAcOtriforce_c, fProfile::OBJ_TRIFORCE, 0x15D, 0, 4);

// clang-format off
dCcD_SrcSph dAcOtriforce_c::sCcSrc = {
    {{{0}, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {{0xFEB77DFF}, 0x111, 0x60407, 0, 0}, 
    {0xE9}},
    {150.0f}};
// clang-format on

const f32 dAcOtriforce_c::sScale = 6.5f;
const f32 dAcOtriforce_c::sAmpNeg = -3.0f;
const u32 dAcOtriforce_c::sStartingOffsetRange = 0x10000;

// const f32 dAcOtriforce_c::sAmpPos = 23.0f;

bool dAcOtriforce_c::createHeap() {
    mResFile = getOarcResFile("TriForce");
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("TriForce");
    TRY_CREATE(mMdl.create(mdl, &heap_allocator, 0x324));
    nw4r::g3d::ResAnmTexSrt anm = mResFile.GetResAnmTexSrt("TriForce");
    TRY_CREATE(mAnm.create(mdl, anm, &heap_allocator, nullptr, 1));
    mMdl.setAnm(mAnm);
    return true;
}

int dAcOtriforce_c::create() {
    if (!initAllocatorWork1Heap(-1, "dAcOtriforce_c::m_allocator", 0x20)) {
        return FAILED;
    }

    mCCdStruct.setField0x38ToMagicValue();
    mCollision.init(sCcSrc);
    mCollision.initUnk(mCCdStruct);
    mCollision.clearCoFlag();
    mStartingOffset = cM::rndInt(sStartingOffsetRange);
    mEffects.init(this);
    mCollision.setSomeDefendValue(0x2000);
    updateMatrix();
    scale.x = sScale;
    scale.y = sScale;
    scale.z = sScale;
    mMdl.setLocalMtx(worldMatrix);
    return SUCCEEDED;
}

int dAcOtriforce_c::doDelete() {
    return 1;
}

extern const u16 lbl_8057AB3E;
extern "C" void fn_80029AE0(EffectsStruct *, u16, mMtx_c *, void *, void *);

int dAcOtriforce_c::actorExecute() {
    int zero = 0;
    mVec3_c offset = mVec3_c::Ey * (mStartingOffset.sin() * getBounceScalar());
    position = mStartingPos + offset;
    mStartingOffset.mVal += 0x16C;

    mCollision.setC(position.x, position.y + 90.0f + zero, position.z);

    ColliderManager::getColliderManager()->addCollider(&mCollision);
    updateMatrix();
    Mtx m;
    PSMTXScale(m, scale.x, scale.y, scale.z);
    PSMTXConcat(worldMatrix.m, m, worldMatrix.m);
    mMdl.setLocalMtx(worldMatrix);
    mAnm.play();
    fn_80029AE0(&mEffects, lbl_8057AB3E, &worldMatrix, nullptr, nullptr);
    return 1;
}

int dAcOtriforce_c::draw() {
    drawModelType1(&mMdl);
    return 1;
}
