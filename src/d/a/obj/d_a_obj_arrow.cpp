#include <c/c_math.h>
#include <d/a/d_a_player.h>
#include <d/a/obj/d_a_obj_arrow.h>

SPECIAL_ACTOR_PROFILE(ARROW, dAcArrow_c, fProfile::ARROW, 0x126, 0, 0x80);

STATE_DEFINE(dAcArrow_c, Wait);
STATE_DEFINE(dAcArrow_c, Move);
STATE_DEFINE(dAcArrow_c, ActorStop);
STATE_DEFINE(dAcArrow_c, BgStop);
STATE_DEFINE(dAcArrow_c, Bound);

// clang-format off
dCcD_SrcGObjInf dAcArrow_c::sCc1 = {
    {{0x2000}, 0x8BB, 0x15000000, 0, 0, 0, 0, 0, 0}, 
    {{0xFEB75C7F}, 0x210, 0x407, 0, 0}, 
    {0},
};

dCcD_SrcSph dAcArrow_c::sCc2 = {
    dAcArrow_c::sCc1,
    {5.0f},
};

const dCcD_SrcSph dAcArrow_c::sCc3 = {
    {{{0x0}, 0x0, 0x0, 0, 0, 0, 0, 0, 0}, 
    {{0x2}, 0x211, 0x407, 0, 0}, 
    {0xC28}},
    {100.0f},
};

// clang-format on

bool hitCallback(dAcObjBase_c* i_actorA, dCcD_GObjInf* i_objInfA, dAcObjBase_c* i_actorB, dCcD_GObjInf* i_objInfB) {
    static_cast<dAcArrow_c*>(i_actorA)->hitCallback(i_objInfA, i_actorB, i_objInfB);
    return true;
}

bool dAcArrow_c::createHeap() {
    mResFile = getOarcResFile("Alink");
    nw4r::g3d::ResMdl mdl(nullptr);
    if ((mSubType & 0x10) != 0) {
        mdl = mResFile.GetResMdl("EquipPachinkoBullet");
    } else {
        mdl = mResFile.GetResMdl("EquipArrow");
    }
    // ?
    bool ok = mModel.create(mdl, &heap_allocator, 0x120);
    if (!ok) {
        ok = false;
    }
    return ok;
}

extern "C" void fn_8025E5E0(void *);

int dAcArrow_c::create() {
    mSubType = params & 0xFF;
    CREATE_ALLOCATOR(dAcArrow_c);

    field_0x67C |= 4;
    mCCdStruct.field_0x38 = 2;
    mCollider1.init(*(dCcD_SrcUnk *)&sCc2);
    mCollider1.initUnk(mCCdStruct);
    mCollider1.setAtFlagFlag(0x2000);
    mCollider1.setAtCallback(::hitCallback);
    mCollider2.init(sCc3);
    mCollider2.initUnk(mCCdStruct);

    if (mSubType == 0x11) {
        mStateMgr.changeState(StateID_Move);
    } else {
        mStateMgr.changeState(StateID_Wait);
    }

    setBoundingBox(mVec3_c(-6.0f, -6.0f, 0.0f), mVec3_c(6.0f, 6.0f, 110.0f));
    field_0x684 = -1;
    field_0x68A = 300;
    f32 rnd = cM::rndF(20.0f);
    field_0x688 = 50.0f - rnd;
    field_0x6A8 = 80.0f;
    mDespawnTimer = 10;
    fn_8025E5E0(this);
    return SUCCEEDED;
}

dAcArrow_c::~dAcArrow_c() {}

void dAcArrow_c::hitCallback(dCcD_GObjInf* i_objInfA, dAcObjBase_c* i_actorB, dCcD_GObjInf* i_objInfB) {
    
}

int dAcArrow_c::doDelete() {
    // TODO

    return SUCCEEDED;
}

int dAcArrow_c::actorExecute() {
    mStateMgr.executeState();

    // TODO

    return SUCCEEDED;
}

int dAcArrow_c::draw() {
    return SUCCEEDED;
}

void dAcArrow_c::initializeState_Wait() {}
void dAcArrow_c::executeState_Wait() {}
void dAcArrow_c::finalizeState_Wait() {}

void dAcArrow_c::initializeState_Move() {}
void dAcArrow_c::executeState_Move() {}
void dAcArrow_c::finalizeState_Move() {}

void dAcArrow_c::initializeState_ActorStop() {}
void dAcArrow_c::executeState_ActorStop() {}
void dAcArrow_c::finalizeState_ActorStop() {}

void dAcArrow_c::initializeState_BgStop() {}
void dAcArrow_c::executeState_BgStop() {}
void dAcArrow_c::finalizeState_BgStop() {}

void dAcArrow_c::initializeState_Bound() {}
void dAcArrow_c::executeState_Bound() {}
void dAcArrow_c::finalizeState_Bound() {}
