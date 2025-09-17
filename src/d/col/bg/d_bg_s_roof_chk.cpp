#include "d/col/bg/d_bg_s_roof_chk.h"

#include "d/col/bg/d_bg_s.h"

dBgS_ObjRoofChk dBgS_ObjRoofChk::sInstance;
f32 dBgS_ObjRoofChk::sRoofHeight;

dBgS_RoofChk::dBgS_RoofChk() {
    mPos.set(0.0f, 0.0f, 0.0f);
    mNowY = 0.0f;
    field_0x7C = 1;
    mPosCopy.set(0.0f, 0.0f, 0.0f);

    SetPolyPassChk(GetPolyPassChkInfo());
    SetGrpPassChk(GetGrpPassChkInfo());
}

dBgS_RoofChk::~dBgS_RoofChk() {}

void dBgS_RoofChk::SetPos(mVec3_c const *pPos) {
    mPos = *pPos;
}

dBgS_ObjRoofChk::dBgS_ObjRoofChk() {
    SetObj();
}

dBgS_ObjRoofChk::~dBgS_ObjRoofChk() {}

void dBgS_ObjRoofChk::ClearInstance() {
    sInstance.ClearPi();
}

bool dBgS_ObjRoofChk::CheckPos(const mVec3_c *pPos) {
    sInstance.SetPos(pPos);
    sRoofHeight = dBgS::GetInstance()->RoofChk(&sInstance);
    return (sRoofHeight != 1e9f);
}

dBgS_LinkRoofChk::dBgS_LinkRoofChk() {
    SetLink();
}

dBgS_LinkRoofChk::~dBgS_LinkRoofChk() {}
