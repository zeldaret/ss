#include "d/col/c/c_bg_s_lin_chk.h"

cBgS_LinChk::cBgS_LinChk() {
    Ct();
}

cBgS_LinChk::~cBgS_LinChk() {}

void cBgS_LinChk::Ct() {
    mVec3_c set = mVec3_c::Zero;
    mLin.Set(set, set);
    mField_0x6C = set;
    mField_0x7B = false;
    mField_0x7C = false;
    mField_0x7D = false;
    mField_0x78 = true;
    mField_0x79 = false;
    mField_0x7A = false;
}

void cBgS_LinChk::Set2(const mVec3_c &start, const mVec3_c &end, u32 id) {
    mLin.Set(start, end);
    mField_0x6C = end;
    mActorId = id;
    mField_0x7A = false;
    ClearPi();
}

void cBgS_LinChk::vt_0xC() {}
