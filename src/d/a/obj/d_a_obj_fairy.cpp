#include "d/a/obj/d_a_obj_fairy.h"

#include "d/a/d_a_player.h"

SPECIAL_ACTOR_PROFILE(OBJ_FAIRY, dAcObjFairy_c, fProfile::OBJ_FAIRY, 0x166, 0, 2);

STATE_DEFINE(dAcObjFairy_c, Wait);
STATE_DEFINE(dAcObjFairy_c, Avoid);
STATE_DEFINE(dAcObjFairy_c, PlayerAvoid);
STATE_DEFINE(dAcObjFairy_c, CureStart);
STATE_DEFINE(dAcObjFairy_c, Cure);
STATE_DEFINE(dAcObjFairy_c, CureEnd);
STATE_DEFINE(dAcObjFairy_c, CatchDemo);

bool dAcObjFairy_c::createHeap() {
    void *file = getOarcResFile("PutFairy");
    TRY_CREATE(mModel.create(file, "PutFairy", "PutFairy_fly", &mAllocator, 0x120));

    return true;
}

int dAcObjFairy_c::create() {
    CREATE_ALLOCATOR(dAcObjFairy_c);

    // TODO

    mStateMgr.changeState(StateID_CureStart);
    mStateMgr.changeState(StateID_Wait);
    mModel.getModel().setPriorityDraw(0x7F, 0x7F);
    mBoundingBox.Set(mVec3_c(-20.0f, -200.0f, -20.0f), mVec3_c(20.0f, 20.0f, 20.0f));

    return SUCCEEDED;
}

int dAcObjFairy_c::doDelete() {
    // TODO

    return SUCCEEDED;
}

int dAcObjFairy_c::actorExecute() {
    mStateMgr.executeState();

    // TODO

    return SUCCEEDED;
}

int dAcObjFairy_c::draw() {
    if (!mStateMgr.isState(StateID_CatchDemo)) {
        drawModelType1(&mModel.getModel());

        if (!isCuring()) {
            static mQuat_c rot(0.0f, 0.0f, 0.0f, 10.0f);
            f32 f = field_0x4B0;
            drawShadow(mShadow, nullptr, mWorldMtx, &rot, -1, -1, -1, -1, -1, mPosition.y - f);
        }
    }

    return SUCCEEDED;
}

void dAcObjFairy_c::initializeState_Wait() {}
void dAcObjFairy_c::executeState_Wait() {}
void dAcObjFairy_c::finalizeState_Wait() {}

void dAcObjFairy_c::initializeState_Avoid() {}
void dAcObjFairy_c::executeState_Avoid() {}
void dAcObjFairy_c::finalizeState_Avoid() {
    setActorProperty(AC_PROP_0x1);
}

void dAcObjFairy_c::initializeState_PlayerAvoid() {}
void dAcObjFairy_c::executeState_PlayerAvoid() {}
void dAcObjFairy_c::finalizeState_PlayerAvoid() {
    setActorProperty(AC_PROP_0x1);
}

void dAcObjFairy_c::initializeState_CureStart() {}
void dAcObjFairy_c::executeState_CureStart() {}
void dAcObjFairy_c::finalizeState_CureStart() {}

void dAcObjFairy_c::initializeState_Cure() {}
void dAcObjFairy_c::executeState_Cure() {}
void dAcObjFairy_c::finalizeState_Cure() {}

void dAcObjFairy_c::initializeState_CureEnd() {}
void dAcObjFairy_c::executeState_CureEnd() {}
void dAcObjFairy_c::finalizeState_CureEnd() {}

void dAcObjFairy_c::initializeState_CatchDemo() {}
void dAcObjFairy_c::executeState_CatchDemo() {
    if (!field_0xB89) {
        mStateMgr.changeState(StateID_Wait);
    }
}
void dAcObjFairy_c::finalizeState_CatchDemo() {
    field_0xB89 = 0;
    unsetObjectProperty(OBJ_PROP_0x200);
}

bool dAcObjFairy_c::shouldAvoidLink() const {
    // TODO shuffles
    if (dAcPy_c::LINK->isUsingBugnet()) {
        mVec3_c dist = dAcPy_c::LINK->getBugNetPos() - mPosition;
        bool isClose = false;
        if (dist.mag() < 100.0f && mVelocity.dot(dist) > 0.0f) {
            isClose = true;
        }

        return isClose;
    }

    return false;
}

bool dAcObjFairy_c::isCuring() const {
    return mStateMgr.isState(StateID_CureStart) || mStateMgr.isState(StateID_Cure) ||
           mStateMgr.isState(StateID_CureEnd);
}

bool dAcObjFairy_c::canTargetWithBugNet() const {
    bool canTarget = false;
    if (dAcPy_c::LINK != nullptr && dAcPy_c::LINK->isUsingBugnet() && !isCuring()) {
        canTarget = true;
    }
    return canTarget;
}
