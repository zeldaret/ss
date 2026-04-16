#ifndef EGG_UNK_H
#define EGG_UNK_H

#include "common.h"

#include "rvl/OS.h" // IWYU pragma: export

// This is seen in Animal Crossing: City Folk also. Idk where it belongs

// Seen between EGG::Thread and EGG::ConfigurationData (eggThread and eggSystem)
// dSysNandThread (or NandRequestThread ) inherits from eggThread,
//          with part of that thread referencing tsome of these funcs

namespace EGG {

// Color? Tag?
struct UnknownStruct {
    u8 _00;
    u8 _01;
    u8 _02;
    u8 _03;
};

struct ThreadInfo {
    OSThread *mThread;
    UnknownStruct _04;
    bool _08;
    char _09[32]; // Buffer for name?
    u32 _2C;
};

// This is a complete guess
class ThreadMgr {
public:
    void registerThread(OSThread *thread, UnknownStruct);

    static ThreadMgr *getInstance() {
        return sInstance;
    }

private:
    s32 getThreadIndex(OSThread *thread);
    s32 doRegisterThread(OSThread *thread);
    void sortByPriority();

private:
    u8 _placeholder_00[0x8];
    /* 0x08 */ s32 mMaxThreads;
    u8 _placeholder_0C[0x3C - 0xC];
    /* 0x3C */ ThreadInfo *mThreadList;
    u8 _placeholder_40[0x44 - 0x40];
    /* 0x44 */ s32 mThreadCount;
    u8 _placeholder_48[0xEC - 0x48];
    /* 0xEC */ UnknownStruct _EC; // Default color/tag?
    u8 _placeholder_F0[0xF4 - 0xF0];
    /* 0xF4 */ u32 _F4;
    /* 0xF8 */ bool mSortEnabled;
    // size unsure, but this is the limit seen

    static ThreadMgr *sInstance;
};

} // namespace EGG

#endif
