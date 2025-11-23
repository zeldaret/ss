#ifndef D_T_BARREL_POS_H
#define D_T_BARREL_POS_H

#include "d/t/d_tg.h"

class dTgBarrelPos_c : public dTg_c {
public:
    dTgBarrelPos_c() {}
    virtual ~dTgBarrelPos_c() {}

    u8 getLinkIndex() const {
        return mLinkIndex;
    }
    u8 getLinkId() const {
        return mLinkId;
    }

private:
    /* 0x0FC */ u8 _0xFC[0x114 - 0xFC];
    /* 0x114 */ u8 mLinkIndex;
    /* 0x115 */ u8 mLinkId;
};

#endif
