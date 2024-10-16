#include "d/col/bg/d_bg_s_chk.h"

dBgS_Chk::dBgS_Chk() {}

dBgS_Chk::~dBgS_Chk() {}

cBgS_PolyPassChk *dBgS_Chk::GetPolyPassChkInfo() {
    return this;
}
cBgS_GrpPassChk *dBgS_Chk::GetGrpPassChkInfo() {
    return this;
}

cBgS_GrpPassChk::cBgS_GrpPassChk() {}

cBgS_GrpPassChk::~cBgS_GrpPassChk() {}

cBgS_PolyPassChk::cBgS_PolyPassChk() {}

cBgS_PolyPassChk::~cBgS_PolyPassChk() {}
