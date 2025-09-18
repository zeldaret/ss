#include "d/col/c/c_bg_s_lin_chk.h"

cBgS_LinChk::cBgS_LinChk() {
    Ct();
}

cBgS_LinChk::~cBgS_LinChk() {}

void cBgS_LinChk::Ct() {
    mVec3_c set = mVec3_c::Zero;
    mLin.Set(set, set);
    field_0x6C = set;
    mPreRoofChk = false;
    mPreWallChk = false;
    mPreGroundChk = false;
    mFrontFlag = true;
    mBackFlag = false;
    mHit = false;
}

void cBgS_LinChk::Set2(const mVec3_c &start, const mVec3_c &end, u32 id) {
    mLin.Set(start, end);
    field_0x6C = end;
    mActorId = id;
    mHit = false;
    ClearPi();
}

void cBgS_LinChk::SetEnd(const mVec3_c *) {}
