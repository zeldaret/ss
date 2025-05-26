#include "d/a/obj/d_a_obj_tarzan_pole.h"

#include "d/a/d_a_base.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "f/f_base.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/attention.h"

SPECIAL_ACTOR_PROFILE(OBJ_TARZAN_POLE, dAcOTarzanPole_c, fProfile::OBJ_TARZAN_POLE, 0x235, 0, 3);

// clang-format off
// TODO: FIX: INCORRECT DATA!
dCcD_SrcSph dAcOTarzanPole_c::sCcSrc = {
    /* mObjInf */ {
        /* mObjAt */ {0,0,0,0,0,0,0,0},
        /* mObjTg */ {~(AT_TYPE_BUGNET | AT_TYPE_0x80000 | AT_TYPE_0x8000), 0x801111, {0, 0xA, 0x40F}, 8, 0},
        /* mObjCo */ {0x1E9}},
     /* mSphInf */ {10.0f}
};
// clang-format on

bool dAcOTarzanPole_c::createHeap() {
    // Get Res File
    void *resFile = getOarcResFile("WhipBar");
    mBrres = nw4r::g3d::ResFile(resFile);

    // Create model
    nw4r::g3d::ResMdl resMdl = mBrres.GetResMdl("WhipBar");
    TRY_CREATE(mMdl.create(resMdl, &heap_allocator, 0x120, 1, nullptr));

    // Load Collider
    void *dzb = getOarcDZB("WhipBar", "WhipBar");
    void *plc = getOarcPLC("WhipBar", "WhipBar");
    updateMatrix();
    if (mBgCollider.Set((cBgD_t *)dzb, (PLC *)plc, 1, &mWorldMtx, &mScale)) {
        return false;
    }

    return true;
}

int dAcOTarzanPole_c::create() {
    if (!initAllocatorWork1Heap(0xFFFFFFFF, "dAcOTarzanPole_c::m_allocator", 0x20)) {
        return FAILED;
    }

    dBgS::GetInstance()->Regist(&mBgCollider, this);

    mStts.SetRank(1);
    mCollider.Set(sCcSrc);
    mCollider.SetStts(mStts);

    forwardAccel = 0;
    forwardMaxSpeed = -40;

    mFloat = (double)params;

    mVec = mVec3_c::Ex * 400;
    mVec.rotY(rotation.y);
    boundingBox.Set(mVec3_c(-0, -120, -110), mVec3_c(500, 140, 110));

    return SUCCEEDED;
}

int dAcOTarzanPole_c::doDelete() {
    return SUCCEEDED;
}

int dAcOTarzanPole_c::actorExecute() {
    // Update matrix
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.calc(false);

    // IDK Player?
    dAcPy_c *player = dAcPy_c::LINK;
    bool bVar = false;
    if (mCollider.mTg.MskRPrm(1) && mCollider.mTg.GetActor() != nullptr) {
        bVar = true;
    }
    if (bVar && mCollider.ChkTgAtHitType(0x800)) {
        mObjectActorFlags = mObjectActorFlags | 0x1000;
    }

    mVec = mVec3_c::Ex * 320.0f + mVec3_c::Ey * 0;
    mVec.rotY(rotation.y);
    poscopy2 = position + mVec;
    poscopy3 = poscopy2 + mVec3_c::Ey * 20;

    if (player != nullptr && !player->checkActionFlags(0x400000) && dAcItem_c::checkFlag(ITEM_WHIP)) {
        AttentionManager::GetInstance()->addUnk7Target((dAcObjBase_c &)*this, 1000, 0, 10, -600.0, 200);
    }

    mCollider.SetC(poscopy2);

    dCcS::GetInstance()->Set(&mCollider);
    return SUCCEEDED;
}

int dAcOTarzanPole_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}
