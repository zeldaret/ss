#ifndef D_LYT_PAUSE_H
#define D_LYT_PAUSE_H

#include "d/lyt/d2d.h"
#include "egg/gfx/eggCpuTexture.h"

class dLytPauseMgr_c {
public:
    static dLytPauseMgr_c *GetInstance() {
        return sInstance;
    }

    d2d::ResAccIf_c *getResAcc1() {
        return &mResAcc1;
    }

    EGG::CpuTexture *getBgTexture() {
        return mpBgTexture;
    }

    bool getField_0x083B() const {
        return field_0x083B;
    }

private:
    static dLytPauseMgr_c *sInstance;

    /* 0x0000 */ u8 _0x0000[0x00C8 - 0x0000];
    /* 0x00C8 */ d2d::ResAccIf_c mResAcc1;
    /* 0x0438 */ u8 _0x0438[0x0814 - 0x0438];

    /* 0x0814 */ EGG::CpuTexture *mpBgTexture;

    /* 0x0818 */ u8 _0x0818[0x083B - 0x0818];

    /* 0x083B */ bool field_0x083B;
};

#endif
