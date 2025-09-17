#include "d/a/obj/d_a_obj_arrow.h"

#include "c/c_math.h"
#include "d/col/c/c_cc_d.h"
#include "nw4r/types_nw4r.h"

SPECIAL_ACTOR_PROFILE(ARROW, dAcArrow_c, fProfile::ARROW, 0x126, 0, 0x80);

STATE_DEFINE(dAcArrow_c, Wait);
STATE_DEFINE(dAcArrow_c, Move);
STATE_DEFINE(dAcArrow_c, ActorStop);
STATE_DEFINE(dAcArrow_c, BgStop);
STATE_DEFINE(dAcArrow_c, Bound);

// clang-format off

cCcD_SrcGObj dAcArrow_c::sCcSrcInf = {
    {AT_TYPE_ARROW, 0x8BB, {0x15,0, 0}, 0, 0, 0, 0, 0, 0}, 
    {
        ~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000 | AT_TYPE_ARROW | AT_TYPE_0x100 | AT_TYPE_WIND | AT_TYPE_SLINGSHOT),
             0x210,  {0, 0, 0x407}, 0, 0
            }, 
    {0},
};

dCcD_SrcCps dAcArrow_c::sCc1 = {
    dAcArrow_c::sCcSrcInf,
    {5.0f},
};

const dCcD_SrcSph dAcArrow_c::sCc2 = {
    {{0x0, 0x0, 0x0, 0, 0, 0, 0, 0, 0}, 
    {AT_TYPE_SWORD, 0x211, {0, 0, 0x407}, 0, 0}, 
    {0xC28}},
    {100.0f},
};

// clang-format on

bool dAcArrow_atHitCallback(dAcObjBase_c *i_actorA, cCcD_Obj *i_objInfA, dAcObjBase_c *i_actorB, cCcD_Obj *i_objInfB) {
    static_cast<dAcArrow_c *>(i_actorA)->hitCallback(i_objInfA, i_actorB, i_objInfB);
    return true;
}

bool dAcArrow_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(getOarcResFile("Alink"));
    nw4r::g3d::ResMdl mdl(nullptr);
    if ((mSubType & 0x10) != 0) {
        mdl = mResFile.GetResMdl("EquipPachinkoBullet");
    } else {
        mdl = mResFile.GetResMdl("EquipArrow");
    }
    // ?
    bool ok = mModel.create(mdl, &mAllocator, 0x120);
    if (!ok) {
        ok = false;
    }
    return ok;
}

extern "C" void fn_8025E5E0(void *);

int dAcArrow_c::create() {
    mSubType = mParams & 0xFF;
    CREATE_ALLOCATOR(dAcArrow_c);

    field_0x67C |= 4;
    mStts.SetRank(2);
    mCcCps.Set(sCc1);
    mCcCps.SetStts(mStts);
    mCcCps.SetAtFlag(0x2000);
    mCcCps.SetAtCallback(dAcArrow_atHitCallback);
    mCcSph.Set(sCc2);
    mCcSph.SetStts(mStts);

    if (mSubType == 0x11) {
        mStateMgr.changeState(StateID_Move);
    } else {
        mStateMgr.changeState(StateID_Wait);
    }

    boundingBox.Set(mVec3_c(-6.0f, -6.0f, 0.0f), mVec3_c(6.0f, 6.0f, 110.0f));
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

void dAcArrow_c::hitCallback(cCcD_Obj *i_objInfA, dAcObjBase_c *i_actorB, cCcD_Obj *i_objInfB) {}

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
