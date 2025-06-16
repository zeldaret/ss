#ifndef M_PAD_H
#define M_PAD_H

#include "egg/core/eggController.h"

namespace mPad {

extern EGG::CoreControllerMgr *g_padMg;
extern int g_currentCoreId;
extern EGG::CoreController *g_currentCore;
extern EGG::CoreController *g_core[4];

enum Button {
    BUTTON_LEFT = 0x1,
    BUTTON_RIGHT = 0x2,
    BUTTON_DOWN = 0x4,
    BUTTON_UP = 0x8,
    BUTTON_PLUS = 0x10,
    BUTTON_2 = 0x100,
    BUTTON_1 = 0x200,
    BUTTON_B = 0x400,
    BUTTON_A = 0x800,
    BUTTON_MINUS = 0x1000,
    BUTTON_Z = 0x2000,
    BUTTON_C = 0x4000,
};

inline EGG::CoreController *getCore(const int i) {
    return g_core[i];
}
inline void stopRumbleMgr(int i) {
    g_core[i]->stopRumbleMgr();
}
inline EGG::CoreController *getCore() {
    return g_currentCore;
}
inline EGG::CoreControllerMgr *getMgr() {
    return g_padMg;
}
inline int getCurrentCoreID() {
    return g_currentCoreId;
}

// Defined in dPad, referenced in both mPad and dPad
extern "C" void async_info_callback(s32 chan, s32 result);

void create();
void beginPad();
void endPad();
}; // namespace mPad

#endif
