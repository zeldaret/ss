#include "d/a/obj/d_a_obj_hole.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

SPECIAL_ACTOR_PROFILE(OBJ_HOLE, dAcOhole_c, fProfile::OBJ_HOLE, 0xF7, 0, 258);

STATE_DEFINE(dAcOhole_c, Wait);

bool dAcOhole_c::createHeap() {
    void *data = getOarcResFile("Or");
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("octarock_hole");
    bool ok = mMdl.create(mdl, &mAllocator, 0x20, 1, nullptr);
    if (!ok) {
        return ok;
    }
    updateMatrix();
    mMdl.setScale(mScale);
    mMdl.setLocalMtx(mWorldMtx);
    return ok;
}

int dAcOhole_c::create() {
    CREATE_ALLOCATOR(dAcOhole_c);

    u8 params1 = getFromParams(0, 3);
    field_0x798 = params1 != 3 ? params1 : 0;

    u8 params2 = getFromParams(2, 0xF);
    field_0x799 = params2 != (s8)0xF ? params2 : 0xFF;

    boundingBox.Set(mVec3_c(-200.0f, -200.0f, -200.0f), mVec3_c(200.0f, 200.0f, 200.0f));
    mObjAcch.Set(this, 1, &mAcchCir);
    mAcchCir.SetWall(100.0f, 100.0f);
    mObjAcch.CrrPos(*dBgS::GetInstance());
    dBgS_ObjGndChk::CheckPos(mPosition);
    mStateMgr.changeState(StateID_Wait);
    return SUCCEEDED;
}

int dAcOhole_c::doDelete() {
    return SUCCEEDED;
}

int dAcOhole_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dAcOhole_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOhole_c::initializeState_Wait() {}
void dAcOhole_c::executeState_Wait() {}
void dAcOhole_c::finalizeState_Wait() {}
