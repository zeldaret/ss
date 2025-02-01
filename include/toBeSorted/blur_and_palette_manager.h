#ifndef BLUR_AND_PALETTE_MANAGER_H
#define BLUR_AND_PALETTE_MANAGER_H

#include "common.h"
#include "m/m_vec.h"

class BlurAndPaletteManager {
public:
    static BlurAndPaletteManager &GetInstance() {
        return sInstance;
    }

    void fn_800247D0(mVec3_c, f32);
    void fn_80022AF0(f32);
    void setField_0x2F20(f32 arg) {
        field_0x2F20 = arg;
    }
    void fn_800223A0(void *);
    void fn_80022440(void *);

private:
    u8 field_0x0000[0x2F20 - 0x0000];
    f32 field_0x2F20;
    static BlurAndPaletteManager sInstance;
};

#endif
