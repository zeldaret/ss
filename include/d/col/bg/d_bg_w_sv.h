#ifndef D_BG_W_SV_H
#define D_BG_W_SV_H

#include "d/col/bg/d_bg_w.h"

class dBgWSv : public dBgW {
public:
    dBgWSv();

    bool Set(cBgD_t *pDzb, PLC *pPlc, u32 p3, bool p4);
    void CopyBackVtx();
    bool CrrPosWork(mVec3_c *, int, int, int);
    bool TransPosWork(mVec3_c *, int, int, int);
    void ApplyMtx(mMtx_c *);

    void SetTriHeight(int, f32);
    nw4r::math::VEC3 *GetTri(int);

    virtual ~dBgWSv();
    /* 0x0E4 */ virtual void CrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) override;
    /* 0x0E8 */ virtual void TransPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) override;
    /* 0x0EC */ virtual void
    MatrixCrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) override;

private:
    /* 0x210 */ nw4r::math::VEC3 *mpVtx;
    /* 0x214 */ u32 mField_0x214;
};

#endif
