#include "d/a/obj/d_a_obj_bird_sp.h"

#include "d/a/d_a_bird.h"
#include "d/col/cc/d_cc_s.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m_mtx.h"

// clang-format off
dCcD_SrcSph dAcObjBirdSp_c::sCcSrc = {
    /* mObjInf */ {
        /* mObjAt */ {0, 2, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
        /* mObjTg */ {0xfeb77fff, 0x00000105, {0, 00, 0x407}, 0, 0},
        /* mObjCo */ {0x00004029}},
     /* mSphInf */ {1000.0f}
};
// clang-format on

SPECIAL_ACTOR_PROFILE(OBJ_BIRD_SP_UP, dAcObjBirdSp_c, fProfile::OBJ_BIRD_SP_UP, 0x260, 0, 1);

bool dAcObjBirdSp_c::createHeap() {
    return SUCCEEDED;
}

int dAcObjBirdSp_c::create() {
    CREATE_ALLOCATOR(dAcObjBirdSp_c);

    boundingBox.Set(mVec3_c(-2000, -2000, -2000), mVec3_c(2000, 2000, 2000));
    mCollider.Set(sCcSrc);
    mCollider.SetStts(mStts);
    mCollider.SetR(mScale.x * 1000.0f);

    return SUCCEEDED;
}
int dAcObjBirdSp_c::actorExecute() {
    /* if its colliding with Bird or Player */
    if (mCollider.ChkCoHit() && mCollider.GetCoActor() != nullptr &&
        (mCollider.GetCoActor()->profile_name == fProfile::BIRD ||
         mCollider.GetCoActor()->profile_name == fProfile::PLAYER)) {
        mVec3_c posChange(0, -0.2, 1.0);
        mMtx_c matrix;
        matrix.ZXYrotS(rotation);
        PSMTXMultVec(matrix.m, posChange, posChange);

        // Current most accurate solution
        if ((position.z - dBird_c::getInstance()->position.z) * posChange.z +
                (position.y - dBird_c::getInstance()->position.y) * posChange.y +
                (position.x - dBird_c::getInstance()->position.x) * posChange.x <
            0) {
            posChange = -posChange;
        }

        // !! I think the error occurs down here, and the if statement i think matches as good as i can get it to
        // First attempt
        /* posChange = (posChange * 1000 + position) - dBird_c::getInstance()->position; */
        // Current most accurate solution
        posChange.x = (posChange.x * 1000.0f + position.x) - dBird_c::getInstance()->position.x;
        posChange.y = (posChange.y * 1000.0f + position.y) - dBird_c::getInstance()->position.y;
        posChange.z = (posChange.z * 1000.0f + position.z) - dBird_c::getInstance()->position.z;
        dBird_c::getInstance()->speedUpAccel(posChange);
    }

    mCollider.SetC(position);
    dCcS::GetInstance()->Set(&mCollider);
    PSMTXTrans(mWorldMtx.m, position.x, position.y, position.z);
    mWorldMtx.ZXYrotM((mAng3_c &)position);

    return SUCCEEDED;
}

int dAcObjBirdSp_c::draw() {
    return SUCCEEDED;
}
