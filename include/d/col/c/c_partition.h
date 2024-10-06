#ifndef C_PARTITION_H
#define C_PARTITION_H

/**
 * File Made Up.
 * Some Mask/Quick index into the collision octree
 */
#include "common.h"

class cPartition {
public:
    /* 0x0 */ u32 mX;
    /* 0x4 */ u32 mY;
    /* 0x8 */ u32 mZ;

    cPartition();
    // vt at 0xC
    virtual ~cPartition();
};

#endif
