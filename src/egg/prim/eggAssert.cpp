#include <Runtime.PPCEABI.H/__va_arg.h>
#include <egg/core/eggSystem.h>
#include <egg/core/eggXfbManager.h>
#include <egg/prim/eggAssert.h>
#include <nw4r/db/db_directPrint.h>
#include <rvl/OS.h>
#include <rvl/VI.h>
#include <MSL_C/string.h>


namespace nw4r {
namespace db {
// TODO
extern s32 MapFile_QuerySymbol(void *arg, char *buf, u32 buf_size);
} // namespace db
} // namespace nw4r

namespace EGG {

namespace Assert {

AssertCallback sAssertCallback;
AssertCallback sAssertCallback2;
u32 sPtrOverride;
bool sInstantHalt;
bool sAssertOccurred;

/* 8049bf90 */ void wait(u32 time) {
    u32 tick = OSGetTick();
    u32 tick2;
    do {
        tick2 = OSGetTick();
    } while ((tick2 - tick) / (OS_BUS_CLOCK_SPEED / 4 / 1000) < time);
}

// extern "C" void OSVReport(const char *str, va_list list);

/* 8049c010 */ void system_vreport(const char *str, va_list list) {
    OSVReport(str, list);
}
/* 8049c010 */ void system_report(const char *str, ...) {
    va_list l;
    va_start(l, str);
    system_vreport(str, l);
    va_end(l);
}
/* 8049c0a0 */ s32 getPeriodPos(const char *str) {
    char *b = strchr(str, '.');
    s32 len;
    if (b == nullptr) {
        len = strlen(str);
    } else {
        len = b - str;
    }

    if (len > 0xff) {
        return 0xff;
    }
    return len;
}

/* 80674c60 */ char buf[260];

/* 8049c100 */ const char *getMapSymbol(void *arg) {
    u32 success = nw4r::db::MapFile_QuerySymbol(arg, buf, sizeof(buf));
    return success ? buf : nullptr;
}

/* 8049c150 */ bool isOutsideMEM1(u32 addr) {
    // TODO
    if (0x80000000 <= addr) {
        if (addr <= 0x83000000 - 1) {
            return false;
        }
    }

    return true;
}

/* 8049c180 */ AssertCallback setAssertCallback(AssertCallback cb) {
    AssertCallback old = sAssertCallback;
    sAssertCallback = cb;
    return old;
}

/* 8049c190 */ void system_halt(const char *file, u32 line, const char *msg, va_list list) {
    if (sAssertOccurred) {
#line 152
        OSError("Recursive assertioned");
    }

    sAssertOccurred = true;
    if (sAssertCallback != nullptr) {
        sAssertCallback();
    }
    VISetPreRetraceCallback(nullptr);
    VISetPostRetraceCallback(nullptr);
    system_report("\n---------- HALT -------------\n");
    s32 pos = getPeriodPos(file);
    system_report("%.*s(%d):", pos, file, line);
    va_list args;
    va_copy(args, list);
    system_vreport(msg, args);
    system_report("\n");
    u32 *stackp = (u32 *)OSGetStackPointer();
    if (sPtrOverride != 0) {
        stackp = (u32*)((u32*)sPtrOverride)[1];
    }
    u32 *stack = stackp;

    s32 num = 0;
    for (s32 num = 0; num < 0x1e; num++) {
        if (isOutsideMEM1((u32)stack)) {
            break;
        }
        const char *sym = getMapSymbol((void *)stack[1]);
        if (sym != nullptr) {
            system_report("%d: %s", num, sym);
        } else {
            system_report("%d: %p", num, stack[1]);
        }
        stack = (u32*)stack[0];
        if (isOutsideMEM1((u32)stack)) {
            break;
        }
    }

    if (sInstantHalt == 0) {
        VISetBlack(false);
        VIFlush();
        nw4r::db::DirectPrint_Init();
        nw4r::db::detail::DirectPrint_SetupFB(0);
        Xfb *fb = BaseSystem::mConfigData->getXfbMgr()->mToShowXfb;
        if (fb == nullptr) {
            fb = BaseSystem::mConfigData->getXfbMgr()->mNextXfb;
        }

        if (fb == nullptr) {
#line 239
            OSError("Program Halt");
        }

        // TODO this isn't optimized out?
        void *buf = (void *)0x43300000;
        nw4r::db::DirectPrint_EraseXfb(0, 0, fb->mWidth, fb->mHeight);
        nw4r::db::DirectPrint_StoreCache();
        OSDisableInterrupts();
        OSDisableScheduler();
        OSEnableInterrupts();
        while (true) {
            nw4r::db::DirectPrint_EraseXfb(0, 0, fb->mWidth, fb->mHeight);
            nw4r::db::DirectPrint_StoreCache();
            nw4r::db::DirectPrint_Printf(2, 2, "%.*s:%d", pos, file, line);
            va_list args;
            va_copy(args, list);
            nw4r::db::DirectPrint_printfsub(2, 0xe, msg, args);
            nw4r::db::DirectPrint_StoreCache();

            // TODO instshuffle
            s32 line = 0x1a;
            u32 *stack = stackp;

            for (s32 counter = 0; counter < 0x10; counter++) {
                if (isOutsideMEM1((u32)stack)) {
                    break;
                }
                const char *sym = getMapSymbol((void*)stack[1]);
                if (sym != nullptr) {
                    nw4r::db::DirectPrint_Printf(2, line, "%d:%s\n", counter, sym);
                } else {
                    nw4r::db::DirectPrint_Printf(2, line, "LR Save[%d]:%p\n", counter, (void*)stack[1]);
                }
                nw4r::db::DirectPrint_StoreCache();
                stack = (u32*)*stack;
                if (isOutsideMEM1((u32)stack)) {
                    break;
                }
                line += 0xc;
            }
            nw4r::db::DirectPrint_StoreCache();
            wait(4000);
            if (sAssertCallback2 == nullptr) {
                goto end;
            }
            nw4r::db::DirectPrint_EraseXfb(0, 0, fb->mWidth, fb->mHeight);
            (sAssertCallback2)();
            nw4r::db::DirectPrint_StoreCache();
            wait(4000);
        }
    }
end:
#line 315
    OSError("Program Halt");
    return;
}
/* 8049c530 */ void assert(const char *file, u32 line, const char *msg, ...) {
    va_list l;
    va_start(l, msg);
    system_halt(file, line, msg, l);
    va_end(l);
}

} // namespace Assert

} // namespace EGG
