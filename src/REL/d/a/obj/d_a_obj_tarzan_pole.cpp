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
dCcD_SrcSph dAcOTarzanPole_c::sCcSrc = {
    /* mObjInf */ {
        /* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
        /* mObjTg */ {(AT_TYPE_WHIP | AT_TYPE_WIND), 0x800311, {0, 0, 0x407}, 0x0000, 0x0000},
        /* mObjCo */ {0x00000000}},
     /* mSphInf */ {10.0f}
};
// clang-format on

const f32 dAcOTarzanPole_c::sXOffset = 320.0f;
const f32 dAcOTarzanPole_c::sYOffset = 0.0f;

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
    CREATE_ALLOCATOR(dAcOTarzanPole_c);

    dBgS::GetInstance()->Regist(&mBgCollider, this);

    mStts.SetRank(1);
    mCollider.Set(sCcSrc);
    mCollider.SetStts(mStts);

    forwardAccel = 0.0f;
    forwardMaxSpeed = -40.0f;

    mFloat = (s32)(params & 0xFF) * 10.0f;
    if ((s32)(params & 0xFF) == 0xFF) {
        mFloat = 0.0f;
    }

    mVec = mVec3_c::Ex * 400.0f;
    mVec.rotY(rotation.y);
    boundingBox.Set(mVec3_c(-0.0f, -120.0f, -110.0f), mVec3_c(500.0f, 140.0f, 110.0f));

    return SUCCEEDED;
}

int dAcOTarzanPole_c::doDelete() {
    return SUCCEEDED;
}

int dAcOTarzanPole_c::actorExecute() {
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.calc(false);

    const dAcPy_c *player = dAcPy_c::GetLink();
    bool bVar = false;
    if (mCollider.ChkTgHit() && mCollider.ChkTgAtHitType(AT_TYPE_WHIP)) {
        setObjectProperty(0x1000);
    }

    mVec = mVec3_c::Ex * sXOffset + mVec3_c::Ey * sYOffset;
    mVec.rotY(rotation.y);
    poscopy2 = position + mVec;
    poscopy3 = poscopy2 + mVec3_c::Ey * 20.0f;

    // 0x400000 corresponds to dAcPy_FLG0::FLG0_SWING_ROPE
    if (player != nullptr && !player->checkActionFlags(0x400000) && dAcItem_c::checkFlag(ITEM_WHIP)) {
        AttentionManager::GetInstance()->addUnk7Target(*this, 1, 1000.0f, 10.0f, -600.0, 200);
    }

    mCollider.SetC(poscopy2);

    dCcS::GetInstance()->Set(&mCollider);
    return SUCCEEDED;
}

int dAcOTarzanPole_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}
