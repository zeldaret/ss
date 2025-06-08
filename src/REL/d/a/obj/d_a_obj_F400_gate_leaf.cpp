#include "d/a/obj/d_a_obj_F400_gate_leaf.h"
#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "nw4r/g3d/res/g3d_resfile.h"

SPECIAL_ACTOR_PROFILE(OBJ_F400_GATE_LEAF, dAcOF400GateLeaf_c, fProfile::OBJ_F400_GATE_LEAF, 0x286, 0, 3);

bool dAcOF400GateLeaf_c::createHeap() {
    void *data = dAcObjBase_c::getOarcResFile("F400GateLeaf");
    if (data == nullptr) {
        return false;
    }

    nw4r::g3d::ResFile resFile(data);
    if (!resFile.IsValid()){
        return false;
    }

    nw4r::g3d::ResMdl mdl = resFile.GetResMdl("F400GateLeaf");
    if (!mdl.IsValid()){
        return false;
    }

    return mMdl.create(mdl, &heap_allocator, 0, 1, nullptr);
}

int dAcOF400GateLeaf_c::create() {
    u32 counterIdx = getStoryflag();
    u32 flagValid = StoryflagManager::sInstance->checkFlagValid(counterIdx);
    if (flagValid == 0) {
        return FAILED;
    }
    CREATE_ALLOCATOR(dAcOF400GateLeaf_c);
    mMdl.setPriorityDraw(0x1C, 9);
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mVec3_c min, max;
    mMdl.getBounds(&min,&max);
    boundingBox.Set(min, max);
    return SUCCEEDED;
}

int dAcOF400GateLeaf_c::draw() {
    StoryflagManager *storyFlagManager = StoryflagManager::sInstance;
    u32 counterIdx = getStoryflag();
    if (!storyFlagManager->getCounterOrFlag(counterIdx)) {
        dAcObjBase_c::drawModelType1(&mMdl);
    }
    return SUCCEEDED;
}

u16 dAcOF400GateLeaf_c::getStoryflag() {
    return params & 0xffff;
}
