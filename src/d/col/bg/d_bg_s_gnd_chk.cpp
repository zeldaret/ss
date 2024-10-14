#include "d/col/bg/d_bg_s_gnd_chk.h"

#include "d/col/bg/d_bg_s.h"

dBgS_ObjGndChk dBgS_ObjGndChk::sInstance;
f32 dBgS_ObjGndChk::sGroundHeight;

dBgS_GndChk::dBgS_GndChk() {
    SetPolyPassChk(GetPolyPassChkInfo());
    SetGrpPassChk(GetGrpPassChkInfo());
}

dBgS_GndChk::~dBgS_GndChk() {}

void dBgS_GndChk::Set(const mVec3_c *pos, u32 *id) {
    SetPos(pos);
    SetActorID(id);
}

dBgS_LinkGndChk::dBgS_LinkGndChk() {
    SetLink();
}

dBgS_LinkGndChk::~dBgS_LinkGndChk() {}

dBgS_ObjGndChk::dBgS_ObjGndChk() {
    SetObj();
}

dBgS_ObjGndChk::~dBgS_ObjGndChk() {}

dBgS_ObjGndChk_Wtr::dBgS_ObjGndChk_Wtr() {
    OffNormalGrp();
    OnWaterGrp();
}

dBgS_ObjGndChk_Wtr::~dBgS_ObjGndChk_Wtr() {}

dBgS_ObjGndChk_All::~dBgS_ObjGndChk_All() {}

dBgS_CamGndChk::dBgS_CamGndChk() {
    SetCam();
}

dBgS_CamGndChk::~dBgS_CamGndChk() {}

dBgS_ObjGndChk_All::dBgS_ObjGndChk_All() {
    OnAll();
}

dBgS_CamGndChk_Wtr::dBgS_CamGndChk_Wtr() {
    OnAll();
}

dBgS_CamGndChk_Wtr::~dBgS_CamGndChk_Wtr() {}

void dBgS_ObjGndChk::ClearInstance() {
    sInstance.ClearPi();
}

bool dBgS_ObjGndChk::CheckPos(const mVec3_c *pPos) {
    sInstance.SetPos(pPos);
    sGroundHeight = dBgS::GetInstance()->GroundCross(&sInstance);
    return sGroundHeight != -1e9f;
}

int dBgS_ObjGndChk::GetMaterial() {
    return dBgS::GetInstance()->GetPolyAtt0(sInstance);
}

int dBgS_ObjGndChk::GetPolyAtt1() {
    return dBgS::GetInstance()->GetPolyAtt1(sInstance);
}

bool dBgS_ObjGndChk::GetTriPlane(cM3dGPla *pPlane) {
    return dBgS::GetInstance()->GetTriPla(sInstance, pPlane);
}

s32 dBgS_ObjGndChk::GetRoomID() {
    return dBgS::GetInstance()->GetRoomId(sInstance);
}

bool dBgS_ObjGndChk::ChkMoveBG() {
    return dBgS::GetInstance()->ChkMoveBG(sInstance, true);
}

int dBgS_ObjGndChk::GetLightingCode() {
    return dBgS::GetInstance()->GetLightingCode(sInstance);
}

int dBgS_ObjGndChk::GetSpecialCode() {
    return dBgS::GetInstance()->GetSpecialCode(sInstance);
}
