#include "d/col/c/c_bg_s_shdw_draw.h"

cBgS_ShdwDraw::cBgS_ShdwDraw() {
    mpPla = nullptr;
}

cBgS_ShdwDraw::~cBgS_ShdwDraw() {}

void cBgS_ShdwDraw::Set(const mVec3_c &start, const mVec3_c &end) {
    mBnd.Set(start, end);
}

void cBgS_ShdwDraw::SetCallback(cBgS_ShdwDraw_Callback pCallback) {
    mpCallback = pCallback;
}

void cBgS_ShdwDraw::SetP(cM3dGPla *pPla) {
    mpPla = pPla;
}
void cBgS_ShdwDraw::Calc(cM3dGTri *, mVec3_c *, mVec3_c *, mVec3_c *) {
    // TODO
}
