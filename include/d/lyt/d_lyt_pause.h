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

    bool getField_0x0841() const {
        return field_0x0841;
    }

    enum SelectionType_e {
        SELECT_DOWSING = 2,
        SELECT_ITEM = 6,
        SELECT_CATEGORY = 7,
        SELECT_FIRE = 8,
        SELECT_RING = 9,
    };

    SelectionType_e getCurrentSelectionType() const {
        return mCurrentSelectionType;
    }

    s32 getCurrentSelectionId() const {
        return mCurrentSelectionId;
    }

private:
    static dLytPauseMgr_c *sInstance;

    /* 0x0000 */ u8 _0x0000[0x00C8 - 0x0000];
    
    /* 0x00C8 */ d2d::ResAccIf_c mResAcc1;

    /* 0x0438 */ u8 _0x0438[0x0814 - 0x0438];

    /* 0x0814 */ EGG::CpuTexture *mpBgTexture;
    /* 0x0818 */ SelectionType_e mCurrentSelectionType;

    /* 0x081C */ u8 _0x081C[0x082C - 0x081C];
    
    /* 0x082C */ u16 mCurrentSelectionId;
    
    /* 0x0830 */ u8 _0x0830[0x083B - 0x082E];

    /* 0x083B */ bool field_0x083B;

    /* 0x083D */ u8 _0x083D[0x0841 - 0x083C];

    /* 0x0841 */ bool field_0x0841;
};

#endif
