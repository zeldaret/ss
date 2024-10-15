#include "d/col/bg/d_bg_w_sv.h"

#include "d/col/c/c_bg_s_chk.h"

dBgWSv::dBgWSv() {
    mpVtx = nullptr;
}

dBgWSv::~dBgWSv() {}

bool dBgWSv::Set(cBgD_t *pDzb, PLC *pPlc, u32 p3, bool p4) {
    u32 flags = 0;
    if (p4) {
        flags |= 0x10;
    }
    if (cBgW::Set(pDzb, pPlc, flags | 99, nullptr, nullptr)) {
        return true;
    }

    mField_0x214 = p3;
    if (p3 & 1) {
        return false;
    }
    mpVtx = (nw4r::math::VEC3 *)new Vec[mpBgd->mVtxNum];
    return mpVtx == nullptr;
}

void dBgWSv::CopyBackVtx() {
    if (!mpVtx || !mpVtxTbl) {
        return;
    }
    for (int i = 0; i < mpBgd->mVtxNum; i++) {
        mpVtx[i] = mpVtxTbl[i];
    }
}

bool dBgWSv::CrrPosWork(mVec3_c *pPos, int vtx0, int vtx1, int vtx2) {
    mVec3_c tmp0, tmp1;

    tmp0.x = mpVtx[vtx1].x - mpVtx[vtx0].x;
    tmp0.y = mpVtx[vtx1].y - mpVtx[vtx0].y;
    tmp0.z = mpVtx[vtx1].z - mpVtx[vtx0].z;

    tmp1.x = mpVtx[vtx2].x - mpVtx[vtx0].x;
    tmp1.y = mpVtx[vtx2].y - mpVtx[vtx0].y;
    tmp1.z = mpVtx[vtx2].z - mpVtx[vtx0].z;

    if (cM3d_IsZero(tmp0.z) || cM3d_IsZero(tmp1.z)) {
        return true;
    }

    f32 tmp2 = tmp0.x - (tmp0.z * (tmp1.x / tmp1.z));
    f32 tmp3 = tmp1.x - (tmp1.z * (tmp0.x / tmp0.z));

    if (cM3d_IsZero(tmp2) || cM3d_IsZero(tmp3)) {
        return true;
    }

    pPos->x -= mpVtx[vtx0].x;
    pPos->z -= mpVtx[vtx0].z;

    mVec3_c tmp4;
    mVec3_c tmp5;

    f32 tmp6 = (pPos->x - (pPos->z * (tmp1.x / tmp1.z))) / tmp2;
    f32 tmp7 = (pPos->x - (pPos->z * (tmp0.x / tmp0.z))) / tmp3;

    tmp4.x = mpVtxTbl[vtx1].x - mpVtxTbl[vtx0].x;
    tmp4.z = mpVtxTbl[vtx1].z - mpVtxTbl[vtx0].z;

    tmp5.x = mpVtxTbl[vtx2].x - mpVtxTbl[vtx0].x;
    tmp5.z = mpVtxTbl[vtx2].z - mpVtxTbl[vtx0].z;

    pPos->x = (tmp6 * tmp4.x) + (tmp7 * tmp5.x);
    pPos->z = (tmp6 * tmp4.z) + (tmp7 * tmp5.z);

    pPos->x += mpVtxTbl[vtx0].x;
    pPos->z += mpVtxTbl[vtx0].z;

    return false;
}

void dBgWSv::CrrPos(
    cBgS_PolyInfo const &info, void *pObj, bool b, mVec3_c *pPos, mAng3_c *pAngle, mAng3_c *pShapeAngle
) {
    if (mField_0x214 & 1) {
        return;
    }

    if (!b) {
        return;
    }

    int idx = info.GetPolyIndex();
    cBgD_Tri_t *tri = mpBgd->mTriTbl;

    u32 vtx0 = tri[idx].mVtxIdx0;
    u32 vtx1 = tri[idx].mVtxIdx1;
    u32 vtx2 = tri[idx].mVtxIdx2;

    if (CrrPosWork(pPos, vtx0, vtx1, vtx2) && CrrPosWork(pPos, vtx1, vtx2, vtx0)) {
        CrrPosWork(pPos, vtx2, vtx0, vtx1);
    }
}

bool dBgWSv::TransPosWork(mVec3_c *pPos, int vtx0, int vtx1, int vtx2) {
    mVec3_c tmp0;
    mVec3_c tmp1;

    tmp0.x = mpVtx[vtx1].x - mpVtx[vtx0].x;
    tmp0.y = mpVtx[vtx1].y - mpVtx[vtx0].y;
    tmp0.z = mpVtx[vtx1].z - mpVtx[vtx0].z;

    tmp1.x = mpVtx[vtx2].x - mpVtx[vtx0].x;
    tmp1.y = mpVtx[vtx2].y - mpVtx[vtx0].y;
    tmp1.z = mpVtx[vtx2].z - mpVtx[vtx0].z;

    if (cM3d_IsZero(tmp0.z) || cM3d_IsZero(tmp1.z)) {
        return true;
    }

    f32 tmp2 = tmp0.x - (tmp0.z * (tmp1.x / tmp1.z));
    f32 tmp3 = tmp1.x - (tmp1.z * (tmp0.x / tmp0.z));

    if (cM3d_IsZero(tmp2) || cM3d_IsZero(tmp3)) {
        return true;
    }

    f32 tmp4 = pPos->x - mpVtx[vtx0].x;
    f32 tmp5 = pPos->z - mpVtx[vtx0].z;

    f32 tmp6 = (tmp4 - (tmp5 * (tmp1.x / tmp1.z))) / tmp2;
    f32 tmp7 = (tmp4 - (tmp5 * (tmp0.x / tmp0.z))) / tmp3;

    if (tmp6 < 0.0f || tmp6 > 1.0f || tmp7 < 0.0f || tmp7 > 1.0f) {
        return true;
    }

    mVec3_c tmp8;
    mVec3_c tmp9;

    tmp8.x = mpVtxTbl[vtx1].x - mpVtxTbl[vtx0].x;
    tmp8.y = mpVtxTbl[vtx1].y - mpVtxTbl[vtx0].y;
    tmp8.z = mpVtxTbl[vtx1].z - mpVtxTbl[vtx0].z;

    tmp9.x = mpVtxTbl[vtx2].x - mpVtxTbl[vtx0].x;
    tmp9.y = mpVtxTbl[vtx2].y - mpVtxTbl[vtx0].y;
    tmp9.z = mpVtxTbl[vtx2].z - mpVtxTbl[vtx0].z;

    pPos->x = (tmp6 * tmp8.x) + (tmp7 * tmp9.x);
    pPos->y = (tmp6 * tmp8.y) + (tmp7 * tmp9.y);
    pPos->z = (tmp6 * tmp8.z) + (tmp7 * tmp9.z);

    pPos->x += mpVtxTbl[vtx0].x;
    pPos->y += mpVtxTbl[vtx0].y;
    pPos->z += mpVtxTbl[vtx0].z;

    return false;
}

void dBgWSv::TransPos(
    cBgS_PolyInfo const &info, void *pObj, bool b, mVec3_c *pPos, mAng3_c *pAngle, mAng3_c *pShapeAngle
) {
    if (mField_0x214 & 1) {
        return;
    }

    if (!b) {
        return;
    }

    int idx = info.GetPolyIndex();
    cBgD_Tri_t *tri = mpBgd->mTriTbl;

    u32 vtx0 = tri[idx].mVtxIdx0;
    u32 vtx1 = tri[idx].mVtxIdx1;
    u32 vtx2 = tri[idx].mVtxIdx2;

    if (TransPosWork(pPos, vtx0, vtx1, vtx2) && TransPosWork(pPos, vtx1, vtx2, vtx0)) {
        TransPosWork(pPos, vtx2, vtx0, vtx1);
    }
}

void dBgWSv::MatrixCrrPos(
    cBgS_PolyInfo const &info, void *pObj, bool b, mVec3_c *pPos, mAng3_c *pAngle, mAng3_c *pShapeAngle
) {}

void dBgWSv::ApplyMtx(mMtx_c *pMtx) {
    mpMdlMtx = pMtx;
    ClrNoCalcVtx();
    UpdateMtx();
    GlobalVtx();
    mpMdlMtx = nullptr;
    UpdateMtx();
    SetNoCalcVtx();
}

void dBgWSv::SetTriHeight(int idx, f32 y) {
    mpVtxTbl[idx].y = y;
}

nw4r::math::VEC3 *dBgWSv::GetTri(int idx) {
    return &mpVtxTbl[idx];
}
