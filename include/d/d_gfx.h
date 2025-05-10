#ifndef D_GFX_H
#define D_GFX_H

#include "common.h"

// Made up name
class dGfx_c {
public:
    typedef void (*PreDrawCallback)(void);

    void setDrawCallback(PreDrawCallback cb) {
        mPreDrawCb = cb;
    }

    void setField0x09(u8 val) {
        field_0x09 = val;
    }

    static dGfx_c *GetInstance() {
        return sInstance;
    }

private:
    static dGfx_c *sInstance;

    /* 0x00 */ u8 _0x00[0x04 - 0x00];
    /* 0x04 */ PreDrawCallback mPreDrawCb;
    /* 0x08 */ u8 field_0x08;
    /* 0x09 */ u8 field_0x09;
};

#endif
