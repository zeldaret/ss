#include <d/a/obj/d_a_obj_junk_repairing.h>
#include <toBeSorted/item_story_flag_manager.h>

SPECIAL_ACTOR_PROFILE(OBJ_JUNK_REPAIR, dAcOJunkRep_c, fProfile::OBJ_JUNK_REPAIR, 0x027B, 0, 3);

STATE_DEFINE(dAcOJunkRep_c, Wait);

const f32 dAcOJunkRep_c::sSomeFloat = 0.0f;

bool dAcOJunkRep_c::loadMdl(m3d::smdl_c &mdl, const char *name) {
    nw4r::g3d::ResMdl resMdl = mResFile.GetResMdl(name);
    return mdl.create(resMdl, &heap_allocator, 0x120);
}

bool dAcOJunkRep_c::getState() {
    return StoryFlagManager::sInstance->getCounterOrFlag(0x143) == 1;
}

bool dAcOJunkRep_c::createHeap() {
    mResFile = getOarcResFile("Junk");
    if (!loadMdl(mModel1, sMdlName1)) {
        return false;
    }
    if (!loadMdl(mModel2, sMdlName2)) {
        return false;
    }

    realizeState();
    return static_cast<bool>(mpModelToUse);
}

int dAcOJunkRep_c::create() {
    CREATE_ALLOCATOR(dAcOJunkRep_c);

    forwardAccel = -1.0f;
    forwardMaxSpeed = -40.0f;

    mStateMgr.changeState(StateID_Wait);

    setBoundingBox(mVec3_c(-30.0f, -0.0f, -20.0f), mVec3_c(30.0f, 120.0f, 20.0f));

    return SUCCEEDED;
}

int dAcOJunkRep_c::doDelete() {
    return SUCCEEDED;
}

int dAcOJunkRep_c::actorExecute() {
    mStateMgr.executeState();
    updateMatrix();
    mpModelToUse->setLocalMtx(mWorldMtx);
    return SUCCEEDED;
}

int dAcOJunkRep_c::draw() {
    drawModelType1(mpModelToUse);
    static mQuat_c rot(0.0f, 37.5f, 0.0f, 75.0f);
    fn_8002edb0(mShadow, *mpModelToUse, &rot, -1, -1, sSomeFloat);
    return SUCCEEDED;
}

void dAcOJunkRep_c::realizeState() {
    if (getState()) {
        mpModelToUse = &mModel2;
    } else {
        mpModelToUse = &mModel1;
    }
}

void dAcOJunkRep_c::initializeState_Wait() {}
void dAcOJunkRep_c::executeState_Wait() {}
void dAcOJunkRep_c::finalizeState_Wait() {}
