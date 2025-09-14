#include "d/a/obj/d_a_obj_ruined_save.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

SPECIAL_ACTOR_PROFILE(OBJ_RUINED_SAVE, dAcOruinedSave_c, fProfile::OBJ_RUINED_SAVE, 0x15C, 0, 0);

STATE_DEFINE(dAcOruinedSave_c, Wait);
STATE_DEFINE(dAcOruinedSave_c, Vanish);

bool dAcOruinedSave_c::createHeap() {
    void *data = getOarcResFile("SaveObjectKoke");
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("SaveObjectKoke");
    TRY_CREATE(mMdl.create(mdl, &heap_allocator, 0x128));
    nw4r::g3d::ResFile resFile(data);
    mdl = mMdl.getResMdl();
    nw4r::g3d::ResAnmClr resAnmClr = resFile.GetResAnmClr("SaveObjectKoke_First");
    TRY_CREATE(mAnmMatClr.create(mdl, resAnmClr, &heap_allocator, nullptr, 1));
    return true;
}

int dAcOruinedSave_c::create() {
    if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_STATUES_ACTIVATED)) {
        return FAILED;
    }
    CREATE_ALLOCATOR(dAcOruinedSave_c);
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    poscopy2 = position;
    poscopy2.y += 300.0f;
    poscopy3 = poscopy2;
    field_0x40C = 0;
    mStateMgr.changeState(StateID_Wait);
    return SUCCEEDED;
}

int dAcOruinedSave_c::doDelete() {
    return SUCCEEDED;
}

int dAcOruinedSave_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dAcOruinedSave_c::actorExecuteInEvent() {
    actorExecute();
    return SUCCEEDED;
}

int dAcOruinedSave_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOruinedSave_c::initializeState_Wait() {}

void dAcOruinedSave_c::executeState_Wait() {
    if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_STATUES_ACTIVATED)) {
        mStateMgr.changeState(StateID_Vanish);
    }
}

void dAcOruinedSave_c::finalizeState_Wait() {}

void dAcOruinedSave_c::initializeState_Vanish() {
    mMdl.setAnm(mAnmMatClr);
    field_0x408 = mAnmMatClr.getFrameMax(0) - 1.0f;
}

void dAcOruinedSave_c::executeState_Vanish() {
    field_0x40C++;
    if (((u16)field_0x40C & 0xF) == 3) {
        poscopy3.z = poscopy2.z + cM::rndFX(50.0f);
        poscopy3.y = poscopy2.y - cM::rndFX(20.0f);
    }
    mAnmMatClr.play();
    if (field_0x408 == mAnmMatClr.getFrame(0)) {
        deleteRequest();
    }
}

void dAcOruinedSave_c::finalizeState_Vanish() {}
