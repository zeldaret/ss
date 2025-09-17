#include "d/a/obj/d_a_obj_bstone.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/math/math_arithmetic.h"
#include "toBeSorted/time_area_mgr.h"

#include "rvl/GX.h" // IWYU pragma: export


SPECIAL_ACTOR_PROFILE(OBJ_BSTONE, dAcObstonec, fProfile::OBJ_BSTONE, 0x130, 0, 2);

bool dAcObstonec::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("Bstone"));
    nw4r::g3d::ResMdl mdl = mRes.GetResMdl("model0");
    TRY_CREATE(mMdl.create(mdl, &heap_allocator, 0xA0));
    if (mVariant == 1) {
        mMdl.setPriorityDraw(0xB, 0);
        mMdl.setBlendModeAll(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR, false);
    }
    void *dzb = getOarcFile("Bstone", "dzb/model0.dzb");
    void *plc = getOarcFile("Bstone", "dat/model0.plc");

    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.setScale(mScale);
    mScaleMag = nw4r::math::FSqrt(mScale.squaredLength()) * 350.0f;
    mMdl.calc(false);
    return !mBg.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale);
}

int dAcObstonec::create() {
    mVariant = mParams & 0xF;
    CREATE_ALLOCATOR(dAcObstonec);

    if (mVariant == 1) {
        mBg.SetUnkBase(2);
    } else {
        mBg.SetUnkBase(1);
    }

    forwardAccel = -1.0f;
    forwardMaxSpeed = -40.0f;
    boundingBox.Set(mVec3_c(-350.0f, -60.0f, -240.0f), mVec3_c(350.0f, 600.0f, 240.0f));
    return SUCCEEDED;
}

int dAcObstonec::doDelete() {
    return SUCCEEDED;
}

int dAcObstonec::actorExecute() {
    f32 tmp = 1.0f;
    if (mVariant != 0) {
        tmp = dTimeAreaMgr_c::GetInstance()->checkPositionIsInPastState(roomid, position, nullptr, mScaleMag);
        if (mVariant != 1) {
            if (tmp > 0.0f) {
                mMdl.setPriorityDraw(0xB, 0);
            } else {
                mMdl.setPriorityDraw(0x7F, 0);
            }
        }
    }
    if (tmp > 0.0f) {
        dBgS::GetInstance()->Regist(&mBg, this);
    } else {
        dBgS::GetInstance()->Release(&mBg);
    }

    return SUCCEEDED;
}

int dAcObstonec::draw() {
    if (mVariant != 1 || mBg.ChkUsed()) {
        drawModelType1(&mMdl);
    }
    return SUCCEEDED;
}
