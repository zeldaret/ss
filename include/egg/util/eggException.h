#ifndef EGG_EXCEPTION_H
#define EGG_EXCEPTION_H

#include "common.h"
#include "nw4r/db/db_console.h"

namespace EGG {

class Heap;
class CoreStatus;

class Exception {
public:
    Exception(u16, u16, u16, EGG::Heap *, int);
    virtual ~Exception();

    static void ExceptionWaitTime(u32 time);
    static bool ExceptionCallback(nw4r::db::ConsoleHandle handle, void *);
    static bool ExceptionCallback_(nw4r::db::ConsoleHandle handle, void *);
    static Exception *create(u16, u16, u16, Heap *, int);

    static Exception *sException;

    static nw4r::db::ConsoleHandle sConsoleHandle;
    static void *sMapFileWorks;
    static u32 sMapFileNumMax;
    static u32 sCurrentMapFileNum;
    static volatile bool sUserCallbackMode;

    // inofficial

    static void (*sPreException)();
    static void (*sUserCallbackFunc)(CoreStatus *status);
    static s16 sExceptionDisplayInfo;

    static void SetUserCallback(void (*cb)(CoreStatus *status));
    static void SetPreExceptionCallback(void (*cb)());
    static void SetCallbackMode(bool);
    static void SetCallbackArgs(void *);
    nw4r::db::ConsoleHandle GetConsoleHandle();
};

} // namespace EGG

#endif
