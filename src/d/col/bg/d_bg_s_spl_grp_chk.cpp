#include "d\col\bg\d_bg_s_spl_grp_chk.h"

dBgS_SplGrpChk::dBgS_SplGrpChk() {
    mGnd.set(0.0f, 0.0f, 0.0f);
    mRoof = 1e9f;
    mHeight = -1e9f;
    mGroundH = -1e9f;

    mFlags = 0;

    SetPolyPassChk(GetPolyPassChkInfo());
    SetGrpPassChk(GetGrpPassChkInfo());

    OffNormalGrp();
}

dBgS_SplGrpChk::~dBgS_SplGrpChk() {}

void dBgS_SplGrpChk::Set(mVec3_c &gnd, f32 roof) {
    mGnd = gnd;
    mRoof = roof;
}
