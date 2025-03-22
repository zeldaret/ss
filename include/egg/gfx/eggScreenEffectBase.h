#ifndef EGG_SCREEN_EFFECT_BASE_H
#define EGG_SCREEN_EFFECT_BASE_H

#include "common.h"
#include "egg/gfx/eggScreen.h"

namespace EGG {

class ScreenEffectBase {
protected:
    u8 mFlag;

public:
    ScreenEffectBase();
    virtual ~ScreenEffectBase() {} // at 0x8

protected:
    static Screen sScreen;
};

} // namespace EGG

#endif
