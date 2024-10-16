#ifndef C_M3D_G_UNK_H
#define C_M3D_G_UNK_H

#include "m/m_mtx.h"
#include "m/m_vec.h"

#include <common.h>

struct cM3dGCps;
struct cM3dGCyl;
struct cM3dGSph;

class cM3dGUnk {
public:
    /* 0x00 */ mMtx_c mMtx;
    /* 0x30 */ mMtx_c mInvMtx;
    /* 0x60 */ mVec3_c mMin;
    /* 0x6C */ mVec3_c mMax;
    /* 0x78 */ mVec3_c mField_0x78;
    /* 0x84 */ mVec3_c mField_0x84;
    /* 0x90 */ mVec3_c mField_0x90;
    /* 0x9C */ mVec3_c mField_0x9C;
    /* 0xA8 */ mVec3_c mField_0xA8;
    /* 0xB4 */ u32 mField_0xB4;
    /* 0xB8 */ mVec3_c mVirtCenter;
    /* 0xC4 */ f32 mField_0xC4;

    cM3dGUnk();

    /* vt at 0xC8*/
    virtual ~cM3dGUnk() {}

    void Set(const mVec3_c &, const mVec3_c &);
    void Update();
    void GetStartEnd(mVec3_c &, mVec3_c &);

    void fn_80338c30(cM3dGCps *, mVec3_c *);
    bool fn_80338c40(cM3dGUnk *, mVec3_c *);
    bool fn_80338ca0(/* TODO */);
    bool fn_80338cb0(cM3dGCyl *, mVec3_c *);
    bool fn_80338cc0(cM3dGCyl *, f32 *);
    bool fn_80338cd0(cM3dGSph *, mVec3_c *);
    bool fn_80338ce0(cM3dGSph *, f32 *);

    void Set(const mMtx_c &);
    void Set(const mVec3_c &, const mAng &);
    bool Cross(const mVec3_c &);
    void Clamp(const mVec3_c &, mVec3_c &);
    void fn_80338f30(f32, f32);
};

#endif
