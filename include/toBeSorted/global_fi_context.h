#ifndef GLOBAL_FI_CONTEXT_H
#define GLOBAL_FI_CONTEXT_H

#include "common.h"

struct FiContext {
    /* 0x00 */ u8 _0x00[0x4B - 0x00];
    /* 0x4B */ bool mDoSpecialFiMenuHandling;

    static s32 getGlobalFiInfo0(s32);
};

extern "C" FiContext *GLOBAL_FI_CONTEXT;
extern "C" void fn_8016C9F0(FiContext *);
extern "C" void createGlobalFiContext();

#endif
