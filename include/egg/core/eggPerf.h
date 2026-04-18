#ifndef EGG_PERF_H
#define EGG_PERF_H

#include "egg/core/eggThread.h"

namespace EGG {

class PerformanceView {
public:
    // vtable at 0x0
    /* 0x08 */ virtual void measureBeginFrame() = 0;
    /* 0x0C */ virtual void measureEndFrame() = 0;
    /* 0x10 */ virtual void measureBeginRender() = 0;
    /* 0x14 */ virtual void measureEndRender() = 0;
    /* 0x18 */ virtual void callbackDrawSync(u16 token) = 0;
    /* 0x1C */ virtual void unused() = 0;
    /* 0x20 */ virtual void draw() = 0;
    /* 0x24 */ virtual void setVisible(bool visible) = 0;
    /* 0x28 */ virtual bool isVisible() = 0;
};

}; // namespace EGG

#endif // EGG_PERF_H
