#include "d/a/e/d_a_e_hb_leaf.h"

const f32 dAcEhb_leaf_c::unkFloat0 = 0.0f;
const f32 dAcEhb_leaf_c::unkFloat1 = -0.0f;

SPECIAL_ACTOR_PROFILE(E_HB_LEAF, dAcEhb_leaf_c, fProfile::E_HB_LEAF, 0x100, 0, 3);

bool dAcEhb_leaf_c::createHeap() {
    // This is ever so slightly weird but our m_anmMdl really doesn't take ResFile by value
    // but only ever a single pointer.
    void *fp = getOarcResFile("Degubaba");
    TRY_CREATE(mModel.create(fp, "degubaba_leaf", "shake2", &heap_allocator, 0x123));
    nw4r::g3d::ResFile f(fp);
    nw4r::g3d::ResMdl mdl = f.GetResMdl("degubaba_leaf");
    nw4r::g3d::ResAnmTexPat anm = f.GetResAnmTexPat("degubaba_leaf");
    TRY_CREATE(mAnm.create(mdl, anm, &heap_allocator, nullptr, 1));
    mModel.getModel().setAnm(mAnm);
    return true;
}

int dAcEhb_leaf_c::create() {
    CREATE_ALLOCATOR(dAcEhb_leaf_c);

    boundingBox.Set(mVec3_c(-100.0f, -100.0f, -100.0f), mVec3_c(100.0f, 100.0f, 100.0f));

    mRotation.y = cM::rndF(65535.0f);
    someRot = mRotation.y.mVal;

    switch (getParam1()) {
        case 0:
        case 7:  mType = 0; break;
        case 1:  mType = 1; break;
        case 2:  mType = 2; break;
        case 3:
        case 4:
        case 5:  mRotation.z.set(-0x8000); break;
        default: break;
    }

    if (mType == 0) {
        u32 param2 = getParam2();
        if (param2 == 1) {
            mType = 1;
        } else if (param2 == 2) {
            mType = 2;
        }
    }

    if (mType == 2) {
        mTimeAreaStruct.field_0x08 = 1;
        mTimeAreaStruct.field_0x00 = 1.0f;
    }

    if (mType != 0) {
        clearActorProperty(0x1);
    }

    mStartingPos = position;

    return SUCCEEDED;
}

int dAcEhb_leaf_c::doDelete() {
    return SUCCEEDED;
}

int dAcEhb_leaf_c::actorExecute() {
    if (mType != 0) {
        doCheck();
        if (mTimeAreaStruct.isNearZero()) {
            return SUCCEEDED;
        }
    }

    updateMatrix();
    if (mType != 0) {
        Mtx m;
        PSMTXScale(m, mTimeAreaStruct.field_0x00, mTimeAreaStruct.field_0x00, mTimeAreaStruct.field_0x00);
        PSMTXConcat(mWorldMtx.m, m, mWorldMtx.m);
    }
    mModel.getModel().setLocalMtx(mWorldMtx);
    mModel.getModel().calc(false);
    someRot++;

    return SUCCEEDED;
}

int dAcEhb_leaf_c::draw() {
    if (!isHidden()) {
        drawModelType1(&mModel.getModel());
    }

    return SUCCEEDED;
}

void dAcEhb_leaf_c::setAnm(const char *anm, f32 pos) {
    mModel.setAnm(anm, m3d::PLAY_MODE_4, pos);
}
