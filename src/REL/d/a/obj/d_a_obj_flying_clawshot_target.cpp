#include "d/a/obj/d_a_obj_flying_clawshot_target.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "f/f_base.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "rvl/MTX/mtxvec.h"

SPECIAL_ACTOR_PROFILE(
    OBJ_FLYING_CLAWSHOT_TARGET, dAcOFlyingClawshotTarget_c, fProfile::OBJ_FLYING_CLAWSHOT_TARGET, 0x142, 0, 2
);

bool dAcOFlyingClawshotTarget_c::createHeap() {
    void *data = getOarcResFile("BirdObjD3_S");

    if (!data) {
        return false;
    }

    nw4r::g3d::ResFile file(data);
    if (!file.IsValid()) {
        return false;
    }
    nw4r::g3d::ResMdl mdl = file.GetResMdl("BirdObjD3_S");
    if (!mdl.IsValid()) {
        return false;
    }

    return mMdl.create(mdl, &mAllocator, 0x120, 1, nullptr);
}

// clang-format off
const cCcD_SrcGObj dAcOFlyingClawshotTarget_c::sColSrc = {
  /* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
  /* mObjTg */ {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000 | AT_TYPE_WIND), 0x1080111, {0, 6, 0x407}, 0, 0},
  /* mObjCo */ {0x0}
};
// clang-format on

int dAcOFlyingClawshotTarget_c::actorCreate() {
    initAllocatorWork1Heap(-1, "dAcOFlyingClawshotTarget_c::m_allocator", 0x20);

    mMdl.setPriorityDraw(0x7F, 0x7F);
    mWorldMtx.transS(mVec3_c::Zero);
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.calc(true);
    mMdl.getNodeWorldMtxMultVecZero(mMdl.getNodeID("Mark_point"), mMarkPoint);

    mStts.SetRank(0xC);
    dCcD_SrcSph src = {sColSrc, 80.f};

    mSph.Set(src);
    mSph.SetStts(mStts);
    mSph.SetC(mPosition);
    mState = 0;

    mVec3_c min, max;
    mMdl.getBounds(&min, &max);
    boundingBox.Set(min, max);

    return SUCCEEDED;
}

int dAcOFlyingClawshotTarget_c::actorPostCreate() {
    mVec3_c v = mVec3_c::Ez;
    v.rotX(mRotation.x);
    v.rotY(mRotation.y);
    mVec3_c diff = dAcPy_c::LINK->mPosition - mPosition;
    diff.normalize();
    if (!mToLink.Set(v, diff)) {
        mToLink.set(1.f, 0.f, 0.f, 0.f);
    }
    return SUCCEEDED;
}

int dAcOFlyingClawshotTarget_c::actorExecute() {
    dAcPy_c *player = dAcPy_c::LINK;

    mVec3_c markPoint;
    PSMTXMultVec(mWorldMtx, mMarkPoint, markPoint);

    f32 dist_to = markPoint.squareDistance(player->mPosition);

    if (checkObjectProperty(0x4)) {
        player->vt_0x0DC(this, mMarkPoint);
    }
    if (checkObjectProperty(0x4) && dist_to < 2500.f &&
        (player->checkActionFlagsCont(0x10000000) || player->checkActionFlagsCont(0x20000000))) {
        static mVec3_c vec = -mVec3_c::Ey;
        mVec3_c v2;

        v2.copyFrom(mToLink.rotateVector(mVec3_c::Ez));
        f32 f = 0.16f;

        mQuat_c q;
        if (!q.fn_802F2450(v2, vec, f)) {
            q.set(1.f, 0.f, 0.f, 0.f);
        }

        mToLink = q * mToLink;

    } else {
        mVec3_c vec = player->mPosition - mPosition;
        vec.normalize();

        mVec3_c v2;
        v2.copyFrom(mToLink.rotateVector(mVec3_c::Ez));
        f32 f = 0.1f;

        mQuat_c q;
        if (!q.fn_802F2450(v2, vec, f)) {
            q.set(1.f, 0.f, 0.f, 0.f);
        }

        mToLink = q * mToLink;
    }

    switch (mState) {
        case 0: {
            if (checkObjectProperty(0x4)) {
                if (player->checkActionFlagsCont(0x10000000) || player->checkActionFlagsCont(0x20000000)) {
                    mState = 2;
                } else {
                    mState = 1;
                }
            }
        } break;
        case 1: {
            if (checkObjectProperty(0x4)) {
                if (player->checkActionFlagsCont(0x10000000) || player->checkActionFlagsCont(0x20000000)) {
                    mState = 2;
                }
            } else {
                mState = 0;
            }
        } break;
        case 2: {
            if (!checkObjectProperty(0x4)) {
                mState = 0;
            }
        } break;
    }

    mMtx_c mtx;
    updateMatrix();
    mtx.fromQuat(mToLink);
    mWorldMtx += mtx;
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.calc(false);
    mSph.SetC(mPosition);
    dCcS::GetInstance()->Set(&mSph);

    return SUCCEEDED;
}

int dAcOFlyingClawshotTarget_c::draw() {
    drawModelType1(&mMdl);

    return SUCCEEDED;
}
