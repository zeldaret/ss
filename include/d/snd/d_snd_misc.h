#ifndef D_SND_MISC_H
#define D_SND_MISC_H

#include "common.h"

// This is at least accessed by dSnd3DEngine_c's functions that
// are called as part of UpdateAmbientParam. Appears to be
// related to the biquadFilterValue.
struct dSndSourceParam {
    dSndSourceParam() {
        reset();
    }

    void reset(f32 value_0x10 = 5000.0f) {
        field_0x00 = 300.0;
        field_0x04 = 1000.0f;
        field_0x08 = 100.0f;
        field_0x0C = 400.0f;
        field_0x10 = value_0x10;
    }

    /* 0x00 */ f32 field_0x00;
    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0C;
    /* 0x10 */ f32 field_0x10;
};

#endif
