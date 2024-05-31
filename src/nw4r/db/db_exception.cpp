#include <MSL_C/string.h>
#include <nw4r/db/db_directPrint.h>
#include <nw4r/db/db_exception.h>
#include <nw4r/db/db_mapFile.h>
#include <rvl/GX.h>
#include <rvl/VI.h>

namespace nw4r {
namespace db {

struct ExceptionStruct {
    OSThread thread;
    OSMessageQueue queue;
    void *frameMemory;
    u32 stackAddress;
    ConsoleHandle consoleHandle;
    const GXRenderModeObj *exceptionRenderObj;
    bool (*exceptionCallback)(detail::ConsoleHead *, void *);
    void *exceptionCallbackArgs;
    u32 MSR_COPY;
    u32 FPCSR_COPY;
    u16 exceptionDisplayInfo;
};

static OSMessage sMessageBuffer[1];
static struct ExceptionStruct sException;

static u8 sThreadBuffer[0x4000] ALIGN(0x20);

extern "C" u32 lbl_80574960;

const char *CPU_EXP_NAME[] = {
        "SYSTEM RESET",
        "MACHINE CHECK",
        "DSI",
        "ISI",
        "EXTERNAL INTERRUPT",
        "ALIGNMENT",
        "PROGRAM",
        "FLOATING POINT",
        "DECREMENTER",
        "SYSTEM CALL",
        "TRACE",
        "PERFORMACE MONITOR",
        "BREAK POINT",
        "SYSTEM INTERRUPT",
        "THERMAL INTERRUPT",
        "PROTECTION",
        "FLOATING POINT",
};

void *RunThread_(void *);
// TODO u8 vs u16
void ErrorHandler_(u16, OSContext *, u32, u32);

void Exception_Init() {
    memset(&sException, 0, sizeof(struct ExceptionStruct));
    sException.exceptionCallback = nullptr;
    sException.consoleHandle = nullptr;
    sException.exceptionDisplayInfo = 0x21;
    sException.frameMemory = nullptr;
    sException.exceptionRenderObj = nullptr;
    OSCreateThread(&sException.thread, RunThread_, nullptr, sThreadBuffer + 0x4000, 0x4000, 0, 1);
    OSInitMessageQueue(&sException.queue, sMessageBuffer, 1);
    OSResumeThread(&sException.thread);
    OSSetErrorHandler(2, ErrorHandler_);
    OSSetErrorHandler(3, ErrorHandler_);
    OSSetErrorHandler(5, ErrorHandler_);
    OSSetErrorHandler(15, ErrorHandler_);
    lbl_80574960 = 0;
    OSSetErrorHandler(16, nullptr);
}
extern "C" u32 PPCMfmsr();
extern "C" void PPCMtmsr(u32);
extern "C" void OSFillFPUContext(OSContext *ctx);
extern "C" void fn_803AA2E0(u32, u32, u32, u32);

static void DumpException_(const ExceptionCallbackParam *);

void ErrorHandler_(u16 error, OSContext *ctx, u32 dsisr, u32 dar) {
    sException.MSR_COPY = PPCMfmsr();
    sException.FPCSR_COPY = ctx->fpscr;
    OSFillFPUContext(ctx);
    OSSetErrorHandler(error, nullptr);
    if (error == 0xf) {
        fn_803AA2E0(0, 0, 0, 3);
        fn_803AA2E0(1, 0, 0, 3);
        fn_803AA2E0(2, 0, 0, 3);
        fn_803AA2E0(3, 0, 0, 3);
    }
    ExceptionCallbackParam param;

    param.error = error;
    param.ctx = ctx;
    param.dsisr = dsisr;
    param.dar = dar;
    OSSendMessage(&sException.queue, &param, 1);
    if (OSGetCurrentThread() == nullptr) {
        VISetPreRetraceCallback(nullptr);
        VISetPostRetraceCallback(nullptr);
        PPCMtmsr(PPCMfmsr() | 2);
        DumpException_(&param);
    }
    while (OSEnableScheduler() > 0) {}
    OSYieldThread();
    OSDisableScheduler();
}

void *RunThread_(void *) {
    OSMessage message;

    u32 msr = PPCMfmsr();
    PPCMtmsr(msr & ~(0x800 | 0x100));
    OSReceiveMessage(&sException.queue, &message, 1);
    OSDisableInterrupts();
    VISetPreRetraceCallback(nullptr);
    VISetPostRetraceCallback(nullptr);
    if (sException.consoleHandle != nullptr) {
        VISetBlack(false);
        VIFlush();
    }
    DumpException_((const ExceptionCallbackParam *)message);
    return nullptr;
}

static void PrintContext_(u16, const OSContext *, u32, u32);
static void WaitTime_(s32 time);
static void Console_SetVisible(ConsoleHandle, bool);
static void Console_SetViewBaseLine(ConsoleHandle, s32);
static u32 Console_GetBufferHeadLine(ConsoleHandle);

static void DrawConsoleEndless_(ConsoleHandle console) {
    s32 lineCnt = Console_GetBufferHeadLine(console);
    while (true) {
        if (lineCnt >= Console_GetTotalLines(console)) {
            lineCnt = console->ringTopLineCnt;
        }
        Console_SetViewBaseLine(sException.consoleHandle, lineCnt);
        Console_DrawDirect(console);

        WaitTime_(200);

        lineCnt++;
    }
}

static void DumpException_(const ExceptionCallbackParam *param) {
    u16 error = param->error;
    OSContext *ctx = param->ctx;
    u32 dsisr = param->dsisr;
    u32 dar = param->dar;

    if (error < 0x11) {
        sException.stackAddress = ctx->gprs[1];
    }
    if (sException.consoleHandle != nullptr) {
        sException.frameMemory = detail::DirectPrint_SetupFB(sException.exceptionRenderObj);
    }

    while (true) {
        PrintContext_(error, ctx, dsisr, dar);
        if (sException.consoleHandle != nullptr && DirectPrint_IsActive()) {
            Console_DrawDirect(sException.consoleHandle);
        }

        if (sException.exceptionCallback != nullptr) {
            u16 info = sException.exceptionDisplayInfo;
            do {
                if (!(sException.exceptionCallback)(sException.consoleHandle, sException.exceptionCallbackArgs)) {
                    return;
                }
            } while (info == sException.exceptionDisplayInfo);
        } else if (sException.consoleHandle != nullptr) {
            DrawConsoleEndless_(sException.consoleHandle);
        } else {
            break;
        }
    }
}

static void Exception_Printf_(const char *format, ...) {
    const GXRenderModeObj *obj = sException.exceptionRenderObj;

    va_list args;
    va_start(args, fmt);
    OSVReport(format, args);
    va_end(args);

    if (sException.consoleHandle != nullptr) {
        void *frameBuf = VIGetCurrentFrameBuffer();
        DirectPrint_ChangeXfb(frameBuf, obj->fbWidth, obj->xfbHeight);

        va_list args;
        va_start(args, fmt);
        Console_VFPrintf(CONSOLE_OUTPUT_DISPLAY, sException.consoleHandle, format, args);
        va_end(args);
    }
}

static void ShowStackTrace_(u32);
static void ShowMainInfo_(u16, const OSContext *, u32, u32);
static void ShowGPR_(const OSContext *);
static void ShowSRR0Map_(const OSContext *);
static void ShowGPRMap_(const OSContext *);
static void ShowFloat_(const OSContext *);
static bool ShowMapInfoSubroutine_(u32 address, bool arg);

static void PrintContext_(u16 error, const OSContext *context, u32 dsisr, u32 dar) {
    if (error < 0x11) {
        Exception_Printf_("******** EXCEPTION OCCURRED! ********\nFrameMemory:%XH\n", sException.frameMemory);
    } else {
        Exception_Printf_("******** USER HALT ********\nFrameMemory:%XH\n", sException.frameMemory);
    }

    if (sException.exceptionDisplayInfo & 0x1) {
        Exception_Printf_("---MainInfo---\n");
        ShowMainInfo_(error, context, dsisr, dar);
    }

    if (sException.exceptionDisplayInfo & 0x2) {
        Exception_Printf_("---EXCEPTION_INFO_GPR---\n");
        ShowGPR_(context);
    }

    if (sException.exceptionDisplayInfo & 0x8) {
        Exception_Printf_("---EXCEPTION_INFO_SRR0MAP---\n");
        ShowSRR0Map_(context);
    }

    if (sException.exceptionDisplayInfo & 0x4) {
        Exception_Printf_("---EXCEPTION_INFO_GPRMAP---\n");
        ShowGPRMap_(context);
    }

    if (sException.exceptionDisplayInfo & 0x10) {
        Exception_Printf_("---EXCEPTION_INFO_FPR---\n");
        ShowFloat_(context);
        Exception_Printf_(" MSR:%08XH     FPSCR:%08XH\n", sException.MSR_COPY, sException.FPCSR_COPY);
    }

    if (sException.exceptionDisplayInfo & 0x20) {
        Exception_Printf_("---EXCEPTION_INFO_TRACE---\n");
        ShowStackTrace_(sException.stackAddress);
    }

    Exception_Printf_("--------------------------------\n");
}

static bool IsValidStackAddr_(u32 stackAddress) {
    if (stackAddress == 0 || stackAddress == 0xFFFFFFFF) {
        return false;
    } else if (OS_MEM_IS_MEM1(stackAddress)) {
        if ((stackAddress & 0x0fffffff) >= OSGetPhysicalMem1Size()) {
            return false;
        }
    } else if (OS_MEM_IS_MEM2(stackAddress)) {
        if ((stackAddress & 0x0fffffff) >= OSGetPhysicalMem2Size()) {
            return false;
        }
    } else {
        return false;
    }

    return true;
}

static void ShowStackTrace_(u32 stackAddress) {
    Exception_Printf_("-------------------------------- TRACE\n");
    Exception_Printf_("Stack Address: %08X\n", stackAddress);
    Exception_Printf_("LR save\n");

    u32 i;
    u32 *ptr = (u32 *)stackAddress;
    for (i = 0; i < 16; i++) {
        if (!IsValidStackAddr_((u32)ptr)) {
            break;
        }
        Exception_Printf_("%08X ", ptr[1]);
        u32 fp = ptr[1];
        // TODO arg
        if (!ShowMapInfoSubroutine_(fp, true)) {
            Exception_Printf_("\n");
        }
        ptr = (u32 *)*ptr;
    }
}

void ShowMainInfo_(u16 error, const OSContext *context, u32 dsisr, u32 dar) {
    if (error < 0x11) {
        Exception_Printf_("CONTEXT:%08XH  (%s EXCEPTION)\n", context, CPU_EXP_NAME[error]);
    } else {
        Exception_Printf_("CONTEXT:%08XH\n", context);
    }
    /* floating point error */
    if (error == 0x10) {
        u32 uVar1 = sException.FPCSR_COPY;
        uVar1 &= (((uVar1 & 0xf8) << 0x16) | 0x01f80700);
        if ((uVar1 & 0x20000000) != 0) {
            Exception_Printf_(" FPE: Invalid operation\n");
            if ((sException.FPCSR_COPY & 0x1000000) != 0) {
                Exception_Printf_(" SNaN\n");
            }
            if ((sException.FPCSR_COPY & 0x800000) != 0) {
                Exception_Printf_(" Infinity - Infinity\n");
            }
            if ((sException.FPCSR_COPY & 0x400000) != 0) {
                Exception_Printf_(" Infinity / Infinity\n");
            }
            if ((sException.FPCSR_COPY & 0x200000) != 0) {
                Exception_Printf_(" 0 / 0\n");
            }
            if ((sException.FPCSR_COPY & 0x100000) != 0) {
                Exception_Printf_(" Infinity * 0\n");
            }
            if ((sException.FPCSR_COPY & 0x80000) != 0) {
                Exception_Printf_(" Invalid compare\n");
            }
            if ((sException.FPCSR_COPY & 0x400) != 0) {
                Exception_Printf_(" Software request\n");
            }
            if ((sException.FPCSR_COPY & 0x200) != 0) {
                Exception_Printf_(" Invalid square root\n");
            }
            if ((sException.FPCSR_COPY & 0x100) != 0) {
                Exception_Printf_(" Invalid integer convert\n");
            }
        }
        if ((uVar1 & 0x10000000) != 0) {
            Exception_Printf_(" FPE: Overflow\n");
        }
        if ((uVar1 & 0x8000000) != 0) {
            Exception_Printf_(" FPE: Underflow\n");
        }
        if ((uVar1 & 0x4000000) != 0) {
            Exception_Printf_(" FPE: Zero division\n");
        }
        if ((uVar1 & 0x2000000) != 0) {
            Exception_Printf_(" FPE: Inexact result\n");
        }
    }
    Exception_Printf_("SRR0:   %08XH   SRR1:%08XH\n", context->srr0, context->srr1);
    Exception_Printf_("DSISR:  %08XH   DAR: %08XH\n", dsisr, dar);
}

void ShowGPR_(const OSContext *ctx) {
    Exception_Printf_("-------------------------------- GPR\n");
    for (int i = 0; i < 10; i++) {
        Exception_Printf_("R%02d:%08XH  R%02d:%08XH  R%02d:%08XH\n", i, ctx->gprs[i], i + 11, ctx->gprs[i + 11], i + 22,
                ctx->gprs[i + 22]);
    }
    Exception_Printf_("R%02d:%08XH  R%02d:%08XH\n", 10, ctx->gprs[10], 21, ctx->gprs[21]);
}

void ShowGPRMap_(const OSContext *ctx) {
    bool shownAny = false;
    Exception_Printf_("-------------------------------- GPRMAP\n");
    for (int i = 0; i <= 31; i++) {
        u32 address = ctx->gprs[i];
        if ((address < 0x80000000) || (0x82ffffff < address)) {
            continue;
        }
        shownAny = true;
        Exception_Printf_("R%02d: %08XH ", i, address);
        // TODO arg
        if (!ShowMapInfoSubroutine_(address, true)) {
            Exception_Printf_(" no information\n");
        }
    }

    if (!shownAny) {
        Exception_Printf_("  no register which seem to address.\n");
    }
}

void ShowSRR0Map_(const OSContext *ctx) {
    Exception_Printf_("-------------------------------- SRR0MAP\n");
    u32 address = ctx->srr0;
    if (!((address < 0x80000000) || (0x82ffffff < address))) {
        Exception_Printf_("SRR0: %08XH ", address);
        // TODO arg
        if (!ShowMapInfoSubroutine_(address, true)) {
            Exception_Printf_(" no information\n");
        }
    }
}

// 804ca0e4
extern "C" int __fpclassifyf(float val);

static void ShowFloatSub_(s32 reg, f32 val);

void ShowFloat_(const OSContext *ctx) {
    Exception_Printf_("-------------------------------- FPR\n");
    for (int i = 0; i < 10; i++) {
        ShowFloatSub_(i, ctx->fprs[i]);
        Exception_Printf_(" ");
        ShowFloatSub_(i + 11, ctx->fprs[i + 11]);
        Exception_Printf_(" ");
        ShowFloatSub_(i + 22, ctx->fprs[i + 22]);
        Exception_Printf_("\n");
    }
    ShowFloatSub_(10, ctx->fprs[10]);
    Exception_Printf_(" ");
    ShowFloatSub_(21, ctx->fprs[21]);

    Exception_Printf_("\n");
}

void ShowFloatSub_(s32 reg, f32 val) {
    if (__fpclassifyf(val) == 1) {
        Exception_Printf_("F%02d: Nan      ", reg);
    } else {
        if (__fpclassifyf(val) == 2) {
            if ((reinterpret_cast<u8 &>(val) & 0x80) != 0) {
                Exception_Printf_("F%02d:+Inf      ", reg);
            } else {
                Exception_Printf_("F%02d:-Inf      ", reg);
            }
        } else if (val == 0.0f) {
            Exception_Printf_("F%02d: 0.0      ", reg);
        } else {
            Exception_Printf_("F%02d:%+.3E", reg, val);
        }
    }
}

static void SetFPException_(u32) {
    // TODO
}

bool ShowMapInfoSubroutine_(u32 address, bool arg) {
    if ((address < 0x80000000) || (0x82ffffff < address)) {
        return false;
    } else {
        u8 acStack_124[260];
        if (nw4r::db::MapFile_QuerySymbol(address, acStack_124, sizeof(acStack_124))) {
            Exception_Printf_("%s\n", acStack_124);
            return true;
        } else {
            return false;
        }
    }
}

ConsoleHandle Exception_SetConsole(ConsoleHandle console, const GXRenderModeObj *renderObj) {
    ConsoleHandle old = sException.consoleHandle;
    sException.exceptionRenderObj = renderObj;
    sException.consoleHandle = console;
    return old;
}

ConsoleHandle Exception_GetConsole() {
    return sException.consoleHandle;
}

void Exception_SetUserCallback(bool (*callback)(detail::ConsoleHead *, void *), void *args) {
    sException.exceptionCallback = callback;
    sException.exceptionCallbackArgs = args;
}

u16 Exception_SetDisplayInfo(u16 newInfo) {
    u16 old = sException.exceptionDisplayInfo;
    sException.exceptionDisplayInfo = newInfo;
    return old;
}

static void WaitTime_(s32 time) {
    s64 t1 = OSGetTime();
    s64 t2;
    do {
        t2 = OSGetTime();
    } while ((t2 - t1) / ((OS_BUS_CLOCK_SPEED / 4) / 1000) < time);
}
/*
void DrawConsoleEndless_(detail::ConsoleHead *) {
    // TODO
}
*/
static void Console_SetVisible(detail::ConsoleHead *, bool) {
    // TODO
}

static void Console_SetViewBaseLine(detail::ConsoleHead *console, s32 line) {
    console->viewTopLine = line;
}

static u32 Console_GetBufferHeadLine(detail::ConsoleHead *console) {
    return console->ringTopLineCnt;
}

} // namespace db
} // namespace nw4r
