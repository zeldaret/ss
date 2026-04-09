#ifndef D_T_CLEF_GAME_H
#define D_T_CLEF_GAME_H

#include "d/t/d_tg.h"

class dTgClefGame_c : public dTg_c {
public:
    dTgClefGame_c() {}
    virtual ~dTgClefGame_c() {}

    static dTgClefGame_c *GetInstance() {
        return sInstance;
    }

    u8 getField_0x14C(s32 idx) const {
        return field_0x14C[idx];
    }

    u8 getField_0x15D(s32 idx) const {
        return field_0x15D[idx];
    }

private:
    static dTgClefGame_c *sInstance;

    static const s32 NUM_TADTONE_GROUPS = 17;

    /* 0x0FC */ u8 _0x0FC[0x14C - 0x0FC];
    /* 0x14C */ u8 field_0x14C[NUM_TADTONE_GROUPS];
    /* 0x15D */ u8 field_0x15D[NUM_TADTONE_GROUPS];
};

#endif
