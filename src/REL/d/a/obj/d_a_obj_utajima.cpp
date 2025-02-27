#include "d/a/obj/d_a_obj_utajima.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/c/c_m3d_g_aab.h"
#include "d/flag/storyflag_manager.h"
#include "toBeSorted/room_manager.h"

static const char *const mMdlNames[] = {
    "IslSon",
    "IslSon_s",
};

SPECIAL_ACTOR_PROFILE(OBJ_UTAJIMA, dAcOutajima_c, fProfile::OBJ_UTAJIMA, 0x1D4, 0, 3);

void dAcOutajima_c::rideCallback(dBgW *bg, dAcObjBase_c *o1, dAcObjBase_c *o2) {
    if (o2->isActorPlayer() && o1 != nullptr) {
        if (StoryflagManager::sInstance->getCounterOrFlag(285) == 0) {
            StoryflagManager::sInstance->setFlag(285);
        }
    }
}

bool dAcOutajima_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("IslSon"));
    RoomManager::bindStageResToFile(&mRes);
    RoomManager::bindSkyCmnToResFile(&mRes);

    nw4r::g3d::ResMdl m(nullptr);

    for (int i = 0; i < 2; i++) {
        m = mRes.GetResMdl(mMdlNames[i]);
        TRY_CREATE(mMdls[i].create(m, &heap_allocator, 0x120));
    }

    m = mRes.GetResMdl(mMdlNames[0]);
    void *dzb = getOarcFile("IslSon", "dzb/IslSon.dzb");
    void *plc = getOarcFile("IslSon", "dat/IslSon.plc");

    updateMatrix();
    for (int i = 0; i < 2; i++) {
        mMdls[i].setLocalMtx(mWorldMtx);
    }

    TRY_CREATE(!mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mBgW.Lock();

    mMdls[0].calc(true);
    nw4r::g3d::ResNode nd = m.GetResNode("SetLineGoal");
    mMdls[0].getNodeWorldMtxMultVecZero(nd.GetID(), mVec);

    return true;
}

int dAcOutajima_c::create() {
    CREATE_ALLOCATOR(dAcOutajima_c);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mMdls[0].setPriorityDraw(0x1C, 9);
    mMdls[1].setPriorityDraw(0x22, 9);
    mVec3_c min;
    mVec3_c max;
    mMdls[0].getBounds(&min, &max);
    boundingBox.Set(min, max);
    mCullingDistance = 500000.0f;
    mBgW.SetRideCallback(rideCallback);

    return SUCCEEDED;
}

int dAcOutajima_c::doDelete() {
    return SUCCEEDED;
}

int dAcOutajima_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOutajima_c::draw() {
    for (int i = 0; i < 2; i++) {
        drawModelType1(&mMdls[i]);
    }
    return SUCCEEDED;
}
