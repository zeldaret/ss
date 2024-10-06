#include "d/col/c/c_bg_s_time_area.h"

cBgS_TimeArea::cBgS_TimeArea() {
    mpPla = nullptr;
}

cBgS_TimeArea::~cBgS_TimeArea() {}

void cBgS_TimeArea::Set(const mVec3_c &start, const mVec3_c &end) {
    mLin.SetStartEnd(start, end);
}

void cBgS_TimeArea::SetCallback(TimeDrawCallback pCallback) {
    mpCallback = pCallback;
}

void cBgS_TimeArea::SetP(cM3dGPla *pPla) {
    mpPla = pPla;
}
void cBgS_TimeArea::Calc(cM3dGTri *, mVec3_c *, mVec3_c *, mVec3_c *) {
    // TODO
}
