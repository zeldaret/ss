#include "toBeSorted/d_exception.h"

#include "DynamicLink.h"
#include "common.h"
#include "egg/core/eggController.h"
#include "egg/core/eggHeap.h"
#include "egg/prim/eggAssert.h"
#include "egg/util/eggException.h"
#include "m/m_pad.h"
#include "nw4r/db/db_assert.h"
#include "nw4r/db/db_console.h"
#include "nw4r/db/db_exception.h"
#include "rvl/AI/AI.h"
#include "rvl/KPAD/KPAD.h"
#include "rvl/OS/OSError.h"
#include "rvl/OS/OSInterrupt.h"
#include "rvl/WPAD/WPAD.h"
#include "s/s_Assert.h"
#include "toBeSorted/revision_info.h"

#include <cstdarg>

#define D_INFO_BUILD_TYPE "PRD"

#define D_INFO_COUNTRY "US"
#define D_INFO_GAME_CODE "SOUE"

#define D_ASSERT_NUM_ERROR_HANDLERS 5

static const u8 sErrorCodes[D_ASSERT_NUM_ERROR_HANDLERS] = {
    OS_ERR_DSI, OS_ERR_ISI, OS_ERR_ALIGNMENT, OS_ERR_PROTECTION, OS_ERR_FP_EXCEPTION
};

static OSErrorHandler sSavedErrorHandlers[D_ASSERT_NUM_ERROR_HANDLERS] = {};

static void printGameInfo() {
    nw4r::db::ConsoleHandle c = nw4r::db::Exception_GetConsole();
    nw4r::db::Console_Printf(c, "   COUNTRY: %s\n", D_INFO_COUNTRY);
    nw4r::db::Console_Printf(c, "BUILD_TYPE: %s\n", D_INFO_BUILD_TYPE);
    nw4r::db::Console_Printf(c, " GAME_CODE: %s\n", D_INFO_GAME_CODE);

    nw4r::db::Console_Printf(c, "URL_INFO:%s\n", getUrlInfo());
    nw4r::db::Console_Printf(c, "REV_INFO:%s\n", getRevisionInfo());
    nw4r::db::Console_Printf(c, "COPYDATE:%s\n", getCopydate());
}

// Copied from EGG::Exception::ExceptionCallback_
static void waitForButtonSequence(void *data) {
    KPADStatus status;
    OSEnableInterrupts();
    if (data != nullptr) {
        // Require a specific button sequence to show the exception handler
        int u4 = 0;
        while (((u16 *)data)[u4] != 0) {
            KPADRead(0, &status, 1);
            EGG::Exception::ExceptionWaitTime(50);
            if (status.trig) {
                // Reset the sequence on wrong button press, advance on right button press
                u4 = status.trig & (((u16 *)data)[u4]) ? u4 + 1 : 0;
            }
        }
    }
}

static void preExceptionCallback() {
    WPADControlMotor(0, WPAD_MOTOR_STOP);
    AIStopDMA();
    WPADSetAcceptConnection(true);
    printGameInfo();
}

static void exceptionCallback(EGG::CoreStatus *status) {
    if ((status->hold & (mPad::BUTTON_MINUS | mPad::BUTTON_PLUS)) == (mPad::BUTTON_MINUS | mPad::BUTTON_PLUS)) {
        // When holding Minus and Plus at the same time, go back to the EGG exception handler
        EGG::Exception::SetCallbackMode(true);
        EGG::Exception::SetUserCallback(nullptr);
    }
}

static void *sButtonSequencePtr = nullptr;

static void assertCallback() {
    WPADControlMotor(0, WPAD_MOTOR_STOP);
    AIStopDMA();
    WPADSetAcceptConnection(true);
    waitForButtonSequence(sButtonSequencePtr);
    printGameInfo();
}

static bool sAsserted = false;
static void sAssertCallback(void *) {
    OSDisableInterrupts();
    if (sAsserted) {
        OSReport("Recursive assertioned.\n");
        while (true) {}
    }
    sAsserted = true;
    EGG::Assert::assert("PANIC", 0, "PANIC");
}

static void mainErrorHandler(u16 error, OSContext *ctx, u32 dsisr, u32 dar) {
    for (int i = 0; i < D_ASSERT_NUM_ERROR_HANDLERS; i++) {
        if (error == sErrorCodes[i] && sSavedErrorHandlers[i] != nullptr) {
            (sSavedErrorHandlers[i])(error, ctx, dsisr, dar);
        }
    }
}

static void initErrorHandlers() {
    for (int i = 0; i < D_ASSERT_NUM_ERROR_HANDLERS; i++) {
        // UB: casting non-variadic function to variadic (and letting OS invoke it)
        OSErrorHandler oldHandler = OSSetErrorHandler(sErrorCodes[i], (OSErrorHandler)mainErrorHandler);
        if (oldHandler != nullptr) {
            sSavedErrorHandlers[i] = oldHandler;
        } else {
            OSSetErrorHandler(sErrorCodes[i], nullptr);
        }
    }
}

// HOME, -, +, -, +, 1, 2, 1, 2, A, same as NSMBW
static const u16 sButtonSequence[] = {
    mPad::BUTTON_HOME,
    mPad::BUTTON_MINUS,
    mPad::BUTTON_PLUS,
    mPad::BUTTON_MINUS,
    mPad::BUTTON_PLUS,
    mPad::BUTTON_1,
    mPad::BUTTON_2,
    mPad::BUTTON_1,
    mPad::BUTTON_2,
    mPad::BUTTON_A,
    0,
};

void exceptionCreate(EGG::Heap *heap) {
    EGG::Exception::create(64, 100, 1, heap, 0);
    initErrorHandlers();

    static DbMapFile mapFile;
    mapFile.RegisterOnDvd("maps/RVZELDANP.map", nullptr);
    nw4r::db::Exception_SetDisplayInfo(0x20 | 0x10 | 0x8 | 0x4 | 0x2 | 0x1);
    nw4r::db::Assertion_SetConsole(EGG::Exception::GetConsoleHandle());
    EGG::Exception::SetCallbackArgs((void *)sButtonSequence);
    sButtonSequencePtr = (void *)sButtonSequence;
    EGG::Exception::SetPreExceptionCallback(preExceptionCallback);
    EGG::Exception::SetUserCallback(exceptionCallback);
    EGG::Assert::setAssertCallback(assertCallback);
    sAssert::setAssertCallback(sAssertCallback);
}

// Overriding weakly linked OS function
static bool sPanicked = false;
void OSPanic(const char *file, int line, const char *msg, ...) {
    OSDisableInterrupts();
    if (sPanicked) {
        OSReport("Recursive assertioned.\n");
        while (true) {}
    }
    sPanicked = true;

    va_list l;
    va_start(l, msg);
    EGG::Assert::system_halt(file, line, msg, l);
    va_end(l);
}

// Pooled data
void d_exception_unused() {
    OSReport("d_Exception.cpp");
    OSReport("Recursive assertioned.");
}
