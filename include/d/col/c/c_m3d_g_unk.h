#ifndef C_M3D_G_UNK_H
#define C_M3D_G_UNK_H

#include "m/m_mtx.h"
#include "m/m_vec.h"

#include <common.h>

class cM3dGUnk {
public:
    /* 0x00 */ mMtx_c mMtx;
    /* 0x30 */ mMtx_c mInvMtx;
    /* 0x60 */ mVec3_c mStart;
    /* 0x78 */ mVec3_c mEnd;
    /* 0x84 */ mVec3_c mMid;
    /* 0x90 */ mVec3_c mField_0x90;
    /* 0x9C */ mVec3_c mField_0x9C;
    /* 0xA8 */ mVec3_c mField_0xA8;
    /* 0xB4 */ u32 mField_0xB4;
    /* 0xB8 */ f32 mField_0xB8;
    /* 0xBC */ f32 mField_0xBC;
    /* 0xC0 */ f32 mField_0xC0;
    /* 0xC4 */ f32 mField_0xC4;

    cM3dGUnk();

    /* vt at 0xC8*/
    virtual ~cM3dGUnk();

    void Set(const mVec3_c &, const mVec3_c &);
    void Update();
    void GetStartEnd(mVec3_c &, mVec3_c &);

    void fn_80338c30(/* TODO */);
    bool fn_80338c40(/* TODO */);
    bool fn_80338ca0(/* TODO */);
    bool fn_80338cb0(/* TODO */);
    bool fn_80338cc0(/* TODO */);
    bool fn_80338cd0(/* TODO */);
    bool fn_80338ce0(/* TODO */);

    void Set(const mMtx_c &);
    void Set(const mVec3_c &, const mAng &);
    bool Cross(const mVec3_c &);
    void Clamp(const mVec3_c &, mVec3_c &);
    void fn_80338f30(f32, f32);
};

#endif
