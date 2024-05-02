#ifndef EGG_ASYNC_DISPLAY_H
#define EGG_ASYNC_DISPLAY_H

#include "egg/core/eggDisplay.h"
#include <common.h>

namespace EGG {

class AsyncDisplay : public Display {
public:
    // vtable at 0x04 | 8056eb60
    /* vt 0x08 | 80497f20 */ virtual void beginFrame();
    /* vt 0x10 | 804980e0 */ virtual void endRender();
    /* vt 0x14 | 804980c0 */ virtual void endFrame();
    /* vt 0x18 | 804982d0 */ virtual u32 getTickPerFrame();
    /* vt 0x1C | 80498100 */ virtual void preVRetrace();

private:
    inline void waitForMsg(bool);

public:
    /* 0x2C */ char field_0x2C[0x60 - 0x2C];
    /* 0x60 */ OSThreadQueue mThreadQueue;
    /* 0x68 */ OSMessageQueue mMesgQueue;
    /* 0x88 */ OSMessage mMesgBuf[4];
    /* 0x98 */ s32 field_0x98;
    /* 0x9C */ f32 field_0x9C;
    /* 0xA0 */ f32 field_0xA0;
    /* 0xA4 */ s32 field_0xA4;
    /* 0xA8 */ s32 field_0xA8;
    /* 0xAC */ s32 field_0xAC;
    /* 0xB0 */ u32 field_0xB0;
    /* 0xB4 */ s32 field_0xB4;
    /* 0xB8 */ u32 field_0xB8;
    /* 0xBC */ u32 field_0xBC;

public:
    /* 80497e40 */ AsyncDisplay(u8 maxRetrace);
    /* 80497ec0 */ f32 calcS();
    /* 80498260 */ void clearEFB();
    /* 804982f0 */ void clearEFB(u16, u16, u16, u16, u16, u16, nw4r::ut::Color);
};

} // namespace EGG

#endif
