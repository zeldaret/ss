#include "d/col/c/c_bg_s_gnd_chk.h"

cBgS_GndChk::cBgS_GndChk() {
    mPos = mVec3_c::Zero;
    mField_0x74 = 1;
}

cBgS_GndChk::~cBgS_GndChk() {}

void cBgS_GndChk::SetPos(const mVec3_c &pPos) {
    mPos.x = pPos.x;
    mPos.y = pPos.y;
    mPos.z = pPos.z;
}
void cBgS_GndChk::SetPos(const mVec3_c *pPos) {
    mPos = *pPos;
}
