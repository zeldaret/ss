#include "d/col/cc/d_cc_s.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/c/c_m3d_g_aab.h"
#include "d/d_stage_mgr.h"
#include "f/f_base.h"
#include "rvl/MTX/mtx.h"

dCcS *dCcS::sInstance;

dCcS *dCcS::GetInstance() {
    return sInstance;
}

void dCcS::Ct_cCcS() {
    for (cCcD_Obj **end = mpObjAt + ARRAY_LENGTH(mpObjAt), **obj = mpObjAt; obj < end; ++obj) {
        *obj = nullptr;
    }
    mObjAtCount = 0;

    for (cCcD_Obj **end = mpObjTg + ARRAY_LENGTH(mpObjTg), **obj = mpObjTg; obj < end; ++obj) {
        *obj = nullptr;
    }
    mObjTgCount = 0;

    for (cCcD_Obj **end = mpObjCo + ARRAY_LENGTH(mpObjCo), **obj = mpObjCo; obj < end; ++obj) {
        *obj = nullptr;
    }
    mObjCoCount = 0;

    for (cCcD_Obj **end = mpObj + ARRAY_LENGTH(mpObj), **obj = mpObj; obj < end; ++obj) {
        *obj = nullptr;
    }
    mObjCount = 0;

    for (UNKTYPE **end = mpUnk + ARRAY_LENGTH(mpUnk), **obj = mpUnk; obj < end; ++obj) {
        *obj = nullptr;
    }
    mUnkCount = 0;
}

dCcS::dCcS() {
    sInstance = this;
}

dCcS::~dCcS() {
    sInstance = nullptr;
}

void dCcS::Ct() {
    Ct_cCcS();
    mMassMng.Ct();
    ClearArea();
}

void dCcS::Dt() {
    Ct_cCcS();
    ClearArea();
}

bool dCcS::ChkRemoveActor(dAcObjBase_c *pActor) {
    bool ret = false;
    if (pActor && (pActor->delete_request || pActor->lifecycle_state == fBase_c::TO_BE_DELETED)) {
        ret = true;
    }
    return ret;
}

bool dCcS::ChkNoHitAtTg(cCcD_Obj *, cCcD_Obj *) {}

bool dCcS::ChkAtTgCommonHitInf(cCcD_Obj *, cCcD_Obj *) {}

void dCcS::ChkAtTg() {}

bool dCcS::ChkNoHitCo(cCcD_Obj *, cCcD_Obj *) {}

void dCcS::SetCoCommonHitInf(cCcD_Obj *, mVec3_c *, cCcD_Obj *, mVec3_c *, f32) {}

void dCcS::ChkCo() {}

bool dCcS::ChkSheildFrontRange(cCcD_Obj *, cCcD_Obj *) {}

bool dCcS::ChkShield(cCcD_Obj *, cCcD_Obj *, cCcD_ShieldChkCallback *) {}

bool dCcS::ChkAtTgHitAfterCross(bool, bool, cCcD_Obj *, cCcD_Obj *) {}

void dCcS::SetCoObjInf(bool, bool, cCcD_Obj *, cCcD_Obj *) {}

void dCcS::SetPosCorrect(cCcD_Obj *, mVec3_c *, cCcD_Obj *, mVec3_c *, f32) {}

void dCcS::CalcParticleAngle(cCcD_Obj *, cCcD_Obj *, mAng3_c *, bool) {}

void dCcS::fn_80356fd0(mVec3_c *, cCcD_Obj *, cCcD_Obj *) {}

u32 dCcS::ProcAtTgHitmark(cCcD_Obj *, cCcD_Obj *, mVec3_c *, u32, mVec3_c *) {}

void dCcS::SetAtTgObjInf(bool, bool, cCcD_Obj *, cCcD_Obj *, mVec3_c *, bool) {}

bool dCcS::ChkCamera(mVec3_c &, mVec3_c &, dAcObjBase_c *, dAcObjBase_c *, dAcObjBase_c *, f32) {}

bool dCcS::ChkCameraPoint(
    const mVec3_c &, const mVec3_c &, mVec3_c &, dAcObjBase_c *, dAcObjBase_c *, dAcObjBase_c *, f32
) {}

bool dCcS::ChkCo(const mVec3_c &, dAcObjBase_c *) {}

bool dCcS::fn_80357c90(const mVec3_c &, const mVec3_c &, u32, cCcD_ShapeAttr **, f32) {}

void dCcS::Set(cCcD_Obj *) {}

void dCcS::MoveAfterCheck() {}

void dCcS::RemoveDeadObj() {}

void dCcS::CalcArea() {
    if (mbAreaSet) {
        return;
    }

    cM3dGAab aab;
    aab.ClearForMinMax();
    for (cCcD_Obj **obj = mpObj; obj < mpObj + mObjCount; ++obj) {
        cCcD_ShapeAttr *attr = (*obj)->GetShapeAttr();
        attr->CalcAabBox();
        aab.SetMinMax(attr->GetWorkAab());
    }
    // TODO
}

void dCcS::Move() {
    RemoveDeadObj();
    CalcArea();
    ChkAtTg();
    ChkCo();
    for (int i = 0; i < mObjTgCount; ++i) {
        mpObjTg[i]->SetTg_0x4B(0);
    }
    MoveAfterCheck();
    AdjustHitPos();
    mObjAtCount = 0;
    mObjTgCount = 0;
    mObjCoCount = 0;
    mObjCount = 0;
    mUnkCount = 0;
    mbAreaSet = false;
    ClearArea();
}

void dCcS::MassClear() {
    mMassMng.Clear();
}

void dCcS::SetArea(mMtx_c *pMtx) {
    if (mAreaCount >= 16) {
        return;
    }

    // non-match: Huh
    mAreas[mAreaCount].mInv = mAreas[mAreaCount].mMtx = *pMtx;

    PSMTXInverse(mAreas[mAreaCount].mInv, mAreas[mAreaCount].mInv);
    mAreaCount++;
}

void dCcS::ClearArea() {
    mAreaCount = 0;
}

extern "C" bool fn_8019A7A0(void *stg, f32 *, f32 *);
void dCcS::AdjustHitPos() {
    f32 x, z;
    if (dStageMgr_c::GetInstance() != nullptr && fn_8019A7A0(dStageMgr_c::GetInstance(), &x, &z)) {
        for (cCcD_Obj **obj = mpObj; obj < mpObj + mObjCount; ++obj) {
            (*obj)->AdjustHitPos(x, z);
            (*obj)->GetShapeAttr()->TranslateXZ(x, z);
        }
    }
}
