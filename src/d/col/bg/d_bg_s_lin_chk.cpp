#include "d/col/bg/d_bg_s_lin_chk.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/c/c_bg_w.h"

dBgS_ObjLinChk dBgS_ObjLinChk::sInstance;
dBgS_WtrLinChk dBgS_WtrLinChk::sInstance;

dBgS_LinChk::dBgS_LinChk() {
    SetPolyPassChk(GetPolyPassChkInfo());
    SetGrpPassChk(GetGrpPassChkInfo());
}

dBgS_LinChk::~dBgS_LinChk() {}

void dBgS_LinChk::Set(const mVec3_c *pStart, const mVec3_c *pEnd, const dAcObjBase_c *pObj) {
    Set2(*pStart, *pEnd, pObj ? pObj->getID() : 0);
}

dBgS_CamLinChk::dBgS_CamLinChk() {
    SetCam();
}

dBgS_CamLinChk::~dBgS_CamLinChk() {}

dBgS_CamLinChk_NorWtr::dBgS_CamLinChk_NorWtr() {
    OnAll();
}

dBgS_CamLinChk_NorWtr::~dBgS_CamLinChk_NorWtr() {}

dBgS_ObjLinChk::dBgS_ObjLinChk() {
    SetObj();
    mPreRoofChk = false;
}

dBgS_ObjLinChk::~dBgS_ObjLinChk() {}

void dBgS_ObjLinChk::ClearInstance() {
    sInstance.ClearPi();
}

void dBgS_ObjLinChk::SetEnd(const mVec3_c *pEnd) {
    sInstance.mLin.mEnd = *pEnd;
}

bool dBgS_ObjLinChk::LineCross(mVec3_c const *pStart, mVec3_c const *pEnd, dAcObjBase_c const *pObj) {
    sInstance.Set(pStart, pEnd, pObj);
    return dBgS::GetInstance()->LineCross(&sInstance);
}

mAng dBgS_ObjLinChk::GetAngle() {
    cM3dGPla plane;
    GetTriPla(&plane);
    return plane.GetAngleY();
}

bool dBgS_ObjLinChk::GetTriPla(cM3dGPla *pPlane) {
    return dBgS::GetInstance()->GetTriPla(sInstance, pPlane);
}

int dBgS_ObjLinChk::GetMaterial() {
    return dBgS::GetInstance()->GetPolyAtt0(sInstance);
}

bool dBgS_ObjLinChk::ChkMoveBG() {
    return dBgS::GetInstance()->ChkMoveBG(sInstance, true);
}

bool dBgS_ObjLinChk::ChkGround() {
    cM3dGPla plane;
    GetTriPla(&plane);
    return cBgW_CheckBGround(plane.mNormal.y);
}

bool dBgS_ObjLinChk::ChkWall() {
    cM3dGPla plane;
    GetTriPla(&plane);
    return cBgW_CheckBWall(plane.mNormal.y);
}

bool dBgS_ObjLinChk::ChkRoof() {
    cM3dGPla plane;
    GetTriPla(&plane);
    return cBgW_CheckBRoof(plane.mNormal.y);
}

dBgS_WtrLinChk::dBgS_WtrLinChk() {
    OffFullGrp();
    OnWaterGrp();
}

dBgS_WtrLinChk::~dBgS_WtrLinChk() {}

void dBgS_WtrLinChk::ClearInstance() {
    sInstance.ClearPi();
}

void dBgS_WtrLinChk::SetEnd(const mVec3_c *pEnd) {
    sInstance.mLin.mEnd = *pEnd;
}

bool dBgS_WtrLinChk::SetIsWater(mVec3_c const *pStart, mVec3_c const *pEnd, dAcObjBase_c const *pObj) {
    sInstance.Set(pStart, pEnd, pObj);
    bool ret = false;
    if (dBgS::GetInstance()->LineCross(&sInstance)) {
        if (dBgS::GetInstance()->GetSpecialCode(sInstance) == 0xC /* POLY_GROUND_WATER */) {
            ret = true;
        }
    }
    return ret;
}

dBgS_LinkLinChk::dBgS_LinkLinChk() {
    SetLink();
}

dBgS_LinkLinChk::~dBgS_LinkLinChk() {}

dBgS_BombLinChk::dBgS_BombLinChk() {
    SetBomb();
}

dBgS_BombLinChk::~dBgS_BombLinChk() {}

dBgS_ArrowLinChk::dBgS_ArrowLinChk() {
    SetArrow();
}

dBgS_ArrowLinChk::~dBgS_ArrowLinChk() {}

dBgS_BeetleLinChk::dBgS_BeetleLinChk() {
    SetBeetle();
}

dBgS_BeetleLinChk::~dBgS_BeetleLinChk() {}

dBgS_ClawshotLinChk::dBgS_ClawshotLinChk() {
    SetClawshot();
}

dBgS_ClawshotLinChk::~dBgS_ClawshotLinChk() {}

dBgS_GustBellowsLinChk::dBgS_GustBellowsLinChk() {
    SetBellows();
}

dBgS_GustBellowsLinChk::~dBgS_GustBellowsLinChk() {}

dBgS_WhipLinChk::dBgS_WhipLinChk() {
    SetWhip();
}

dBgS_WhipLinChk::~dBgS_WhipLinChk() {}
