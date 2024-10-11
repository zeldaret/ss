#ifndef C_PARTITION_H
#define C_PARTITION_H

/**
 * File Made Up.
 * Some Mask/Quick index into the collision octree
 */
#include "common.h"
#include "d/col/c/c_m3d_g_aab.h"

class cPartition {
public:
    // static u32 sShift;
    // static u32 sMask;

    /* 0x0 */ u32 mX;
    /* 0x4 */ u32 mY;
    /* 0x8 */ u32 mZ;

    cPartition();

    // vt at 0xC
    virtual ~cPartition();

    void Calc(mVec3_c *pPos);

    void fn_803391f0(const cM3dGAab *);

    static void fn_80338fb0();

    static bool fn_80339070(const cM3dGAab *);
};

#endif
