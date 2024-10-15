#include "d/col/bg/d_bg_w_sv.h"

dBgWSv::dBgWSv() {
    mpVtx = nullptr;
}

dBgWSv::~dBgWSv() {}

bool dBgWSv::Set(cBgD_t *pDzb, PLC *pPlc, u32 p3, bool p4) {}

void dBgWSv::CopyBackVtx() {}

bool dBgWSv::CrrPosWork(mVec3_c *, int, int, int) {}

void dBgWSv::CrrPos(
    cBgS_PolyInfo const &info, void *pObj, bool b, mVec3_c *pPos, mAng3_c *pAngle, mAng3_c *pShapeAngle
) {}

bool dBgWSv::TransPosWork(mVec3_c *, int, int, int) {}

void dBgWSv::TransPos(
    cBgS_PolyInfo const &info, void *pObj, bool b, mVec3_c *pPos, mAng3_c *pAngle, mAng3_c *pShapeAngle
) {}

void dBgWSv::MatrixCrrPos(
    cBgS_PolyInfo const &info, void *pObj, bool b, mVec3_c *pPos, mAng3_c *pAngle, mAng3_c *pShapeAngle
) {}

void dBgWSv::ApplyMtx(mMtx_c *) {}

void dBgWSv::SetTriHeight(int, f32) {}

nw4r::math::VEC3 *dBgWSv::GetTri(int) {}
