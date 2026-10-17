#include "d/a/obj/d_a_obj_teni_rail_post.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/time_area_mgr.h"

SPECIAL_ACTOR_PROFILE(OBJ_TENI_RAIL_POST, dAcOTeniRailPost, fProfile::OBJ_TENI_RAIL_POST, 0x283, 0, 3);

dAcOTeniRailPost::dAcOTeniRailPost() : mDist(0.0f), mIsInPast(false) {}

bool dAcOTeniRailPost::createHeap() {
    void *resFilePtr = getOarcResFile("TeniRailPost");
    if (resFilePtr == nullptr) {
        return false;
    }

    nw4r::g3d::ResFile res(resFilePtr);
    if (!res.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl mdlT = res.GetResMdl("TeniRailPost_T");
    if (!mdlT.IsValid()) {
        return false;
    }

    if (!mMdlT.create(mdlT, &mAllocator, 0x20, 1, nullptr)) {
        return false;
    }
    mMdlT.setLocalMtx(mWorldMtx);

    nw4r::g3d::ResMdl mdlN = res.GetResMdl("TeniRailPost_N");
    if (!mdlN.IsValid()) {
        return false;
    }

    if (!mMdlN.create(mdlN, &mAllocator, 0x20, 1, nullptr)) {
        return false;
    }
    mMdlN.setLocalMtx(mWorldMtx);

    void *dzb = getOarcDZB("TeniRailPost", "TeniRailPost");
    void *plc = getOarcPLC("TeniRailPost", "TeniRailPost");
    TRY_CREATE(!mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::LOCK_e | cBgW::MOVE_BG_e, &mWorldMtx, &mScale));

    dBgS::GetInstance()->Regist(&mBgW, this);
    return true;
}

int dAcOTeniRailPost::create() {
    mVec3_c boundsMin;
    mVec3_c boundsMax;
    updateMatrix();
    CREATE_ALLOCATOR(dAcOTeniRailPost);

    mMdlT.getBounds(&boundsMin, &boundsMax);
    mBoundingBox.Set(boundsMin, boundsMax);
    mCullingDistance = 18000.0f;

    mDist = boundsMin.mag();
    f32 maxDist = boundsMax.mag();
    if (mDist < maxDist) {
        mDist = maxDist;
    }

    s32 room = (s8)mRoomID;
    if (addActorToRoom(-1)) {
        mRoomID = room;
        changeLoadedEntitiesWithSet();
    }

    return SUCCEEDED;
}

int dAcOTeniRailPost::doDelete() {
    dBgS::GetInstance()->Release(&mBgW);
    changeLoadedEntitiesNoSet();
    return SUCCEEDED;
}

int dAcOTeniRailPost::actorExecute() {
    f32 inPast = dTimeAreaMgr_c::GetInstance()->checkPositionIsInPastState(-1, mPosition, nullptr, mDist);
    mIsInPast = inPast > 0.0f;
    if (!mIsInPast) {
        mMdlN.setPriorityDraw(0x1C, 0);
    } else {
        mMdlT.setPriorityDraw(0x0F, 0);
        mMdlN.setPriorityDraw(0x0B, 0);
    }
    return SUCCEEDED;
}

int dAcOTeniRailPost::draw() {
    if (mIsInPast) {
        mMdlT.entry();
    }
    mMdlN.entry();
    return SUCCEEDED;
}

dAcOTeniRailPost::~dAcOTeniRailPost() {}

