#include "d/col/bg/d_bg_s_sph_chk.h"

dBgS_SphChk::dBgS_SphChk() {
    mField_0x80 = 0.0f;
    mField_0x84 = 0.0f;
    mField_0x88 = 0.0f;
    mField_0x8C = 0.0f;

    SetInfo(600, 0, 0);

    mCallback = nullptr;

    SetPolyPassChk(GetPolyPassChkInfo());
    SetGrpPassChk(GetGrpPassChkInfo());

    ClearPi();
}

dBgS_SphChk::~dBgS_SphChk() {}

dBgS_CamSphChk::dBgS_CamSphChk() {
    SetCam();
}

dBgS_CamSphChk::~dBgS_CamSphChk() {}
