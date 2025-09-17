#include "d/a/obj/d_a_obj_F400_gate_seal.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_door.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_anmmatclr.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "rvl/MTX/vec.h"

SPECIAL_ACTOR_PROFILE(OBJ_F400_GATE_SEAL, dAcOF400GateSeal_c, fProfile::OBJ_F400_GATE_SEAL, 0x287, 0, 6);

bool dAcOF400GateSeal_c::createHeap() {
    void *data = dAcObjBase_c::getOarcResFile("F400GateSeal");
    if (data == nullptr) {
        return false;
    }

    nw4r::g3d::ResFile resFile(data);
    if (!resFile.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl mdl = resFile.GetResMdl("F400GateSeal");
    if (!mdl.IsValid()) {
        return false;
    }

    if (!mMdl.create(mdl, &mAllocator, 0)) {
        return false;
    }

    nw4r::g3d::ResAnmClr anmClr = resFile.GetResAnmClr("F400GateSeal");
    if (!anmClr.IsValid()) {
        return false;
    }

    if (!mAnmClr.create(mdl, anmClr, &mAllocator, 0, 1)) {
        return false;
    }

    return mMdl.setAnm(mAnmClr);
}

int dAcOF400GateSeal_c::actorCreate() {
    return SUCCEEDED;
}

int dAcOF400GateSeal_c::actorPostCreate() {
    bool notDone = true;
    dAcOdoor_c *parent = (dAcOdoor_c *)fManager_c::searchBaseByProfName(fProfile::OBJ_DOOR, nullptr);
    dAcOdoor_c *door;

    while (notDone && parent != nullptr) {
        if (10000.0f < PSVECSquareDistance(mPosition, parent->mPosition)) {
            parent = (dAcOdoor_c *)fManager_c::searchBaseByProfName(fProfile::OBJ_DOOR, parent);
            door = parent;
        } else {
            notDone = false;
        }
    }
    if (notDone) {
        return FAILED;
    }

    mDoor.link(door);
    CREATE_ALLOCATOR(dAcOF400GateSeal_c);
    if (mDoor.get() == nullptr) {
        return FAILED;
    }
    if (mDoor.get()->isLocked()) {
        mAnmClr.setFrame(mAnmClr.getFrameStart(0), 0);
    } else {
        mAnmClr.setFrame(mAnmClr.getFrameMax(0), 0);
    }
    mMdl.setPriorityDraw(0x1C, 9);
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mVec3_c min, max;
    mMdl.getBounds(&min, &max);
    mBoundingBox.Set(min, max);
    return SUCCEEDED;
}

int dAcOF400GateSeal_c::actorExecute() {
    if (mDoor.get() == nullptr) {
        return SUCCEEDED;
    }

    if (mDoor.get()->isLocked()) {
        f32 frame = mAnmClr.getFrameStart(0);
        mAnmClr.setFrame(frame, 0);
    } else {
        if (checkObjectProperty(2)) {
            f32 frame = mAnmClr.getFrameMax(0);
            mAnmClr.setFrame(frame, 0);
        } else {
            mAnmClr.play();
        }
    }
    if (mAnmClr.isStop(0)) {
        setObjectProperty(0x200);
    } else {
        unsetObjectProperty(0x200);
    }
    return SUCCEEDED;
}

int dAcOF400GateSeal_c::draw() {
    dAcObjBase_c::drawModelType1(&mMdl);
    return SUCCEEDED;
}
