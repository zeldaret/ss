#ifndef D_HBM_H
#define D_HBM_H

#include "common.h"
class dHbm_c {
public:
    static dHbm_c *GetInstance();

    s32 fn_801967D0();
    s32 fn_80197560(s32);

    void offFlags(u32 flags) {
        mFlags &= ~flags;
    }

private:
    u8 _0x00[0x218 - 0x000];
    u32 mFlags;
};

#endif
