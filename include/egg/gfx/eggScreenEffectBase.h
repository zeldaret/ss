#ifndef EGG_SCREEN_EFFECT_BASE_H
#define EGG_SCREEN_EFFECT_BASE_H

#include "common.h"
#include "egg/gfx/eggScreen.h"

namespace EGG {

class ScreenEffectBase {
protected:
    /* 0x0 */ u8 mFlag;

public:
    ScreenEffectBase();
    virtual ~ScreenEffectBase() {} // at 0x8

    static Screen &GetScreen() {
        return sScreen;
    }

    bool isFlag0x2() const {
        return mFlag & 2;
    }

protected:
    static Screen sScreen;
};

} // namespace EGG

#endif
