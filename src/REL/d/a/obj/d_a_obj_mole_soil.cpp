#include <d/a/obj/d_a_obj_mole_soil.h>
#include <s/s_Math.h>

SPECIAL_ACTOR_PROFILE(OBJ_MOLE_SOIL, dAcOmoleSoil_c, fProfile::OBJ_MOLE_SOIL, 0x008B, 0, 6);

STATE_DEFINE(dAcOmoleSoil_c, Wait);
STATE_DEFINE(dAcOmoleSoil_c, Appear);
STATE_DEFINE(dAcOmoleSoil_c, DisAppear);

const f32 dAcOmoleSoil_c::sHalfScale = 0.5f;
const f32 dAcOmoleSoil_c::sFullScale = 1.0f;

bool dAcOmoleSoil_c::createHeap() {
    nw4r::g3d::ResFile file = getOarcResFile("MogumaMud");
    if (!file.mFile.IsValid()) {
        return false;
    }
    mBrres = file;
    nw4r::g3d::ResMdl mdl = mBrres.GetResMdl("MogumaMud");
    TRY_CREATE(mModel.create(mdl, &heap_allocator, 0x120, 1, nullptr));
    mModel.setPriorityDraw(0x1c, 9);
    return true;
}

bool intToBool(int a) {
    return a;
}

int dAcOmoleSoil_c::create() {
    CREATE_ALLOCATOR(dAcOmoleSoil_c);

    mScale = sHalfScale;
    scale.set(sHalfScale, sHalfScale, sHalfScale);
    mStateMgr.changeState(StateID_Wait);

    setBoundingBox(mVec3_c(-100.0f, -0.0f, -100.0f), mVec3_c(100.0f, 100.0f, 100.0f));
    return SUCCEEDED;
}

int dAcOmoleSoil_c::doDelete() {
    return SUCCEEDED;
}

int dAcOmoleSoil_c::actorExecute() {
    switch (getNextState()) {
    case 1:
        if (!mStateMgr.isState(StateID_Appear)) {
            mStateMgr.changeState(StateID_Appear);
        }
        break;
    case 2:
        if (!mStateMgr.isState(StateID_DisAppear)) {
            mStateMgr.changeState(StateID_DisAppear);
        }
        break;
    case 3:
        if (!mStateMgr.isState(StateID_Appear)) {
            mStateMgr.changeState(StateID_Appear);
        }
        break;
    }
    mStateMgr.executeState();
    updateMatrix();
    mModel.setLocalMtx(worldMatrix);
    return SUCCEEDED;
}

int dAcOmoleSoil_c::draw() {
    if (field_0x391) {
        drawModelType1(&mModel);
    }
    return SUCCEEDED;
}

void dAcOmoleSoil_c::initializeState_Wait() {
    mDesiredNextState = 0;
}
void dAcOmoleSoil_c::executeState_Wait() {}
void dAcOmoleSoil_c::finalizeState_Wait() {}

void dAcOmoleSoil_c::initializeState_Appear() {
    field_0x391 = true;
}
void dAcOmoleSoil_c::executeState_Appear() {
    f32 stepSize = 0.05f;
    if (sLib::chase(&mScale, sFullScale, stepSize)) {
        mStateMgr.changeState(StateID_Wait);
    } else {
        mModel.setScale(mScale, mScale, mScale);
    }
}
void dAcOmoleSoil_c::finalizeState_Appear() {
    mDesiredNextState = 0;
    mScale = sFullScale;
    mModel.setScale(sFullScale, sFullScale, sFullScale);
}

void dAcOmoleSoil_c::initializeState_DisAppear() {
    field_0x391 = true;
}

void dAcOmoleSoil_c::executeState_DisAppear() {
    // Nothing about this makes sense but this creates the necessary casts
    // and operations. Note that Ghidra constant-folds this despite
    // all the stack operations for the cast.
    int scalar = 0;
    f32 stepSize = 0.01f * scalar + 0.05f;
    if (sLib::chase(&mScale, sHalfScale, stepSize)) {
        mStateMgr.changeState(StateID_Wait);
    } else {
        mModel.setScale(mScale, mScale, mScale);
    }
}
void dAcOmoleSoil_c::finalizeState_DisAppear() {
    mDesiredNextState = 0;
    mScale = sHalfScale;
    mModel.setScale(sHalfScale, sHalfScale, sHalfScale);
}
