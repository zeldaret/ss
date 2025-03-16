#include "egg/util/eggException.h"

#include "egg/core/eggController.h"
#include "egg/core/eggHeap.h"
#include "egg/core/eggSystem.h"
#include "egg/core/eggVideo.h"
#include "nw4r/db/db_directPrint.h"
#include "nw4r/db/db_exception.h"
#include "nw4r/db/db_mapFile.h"
#include "rvl/KPAD.h" // IWYU pragma: export
#include "rvl/OS.h"   // IWYU pragma: export

namespace EGG {

Exception *Exception::sException;
nw4r::db::ConsoleHandle Exception::sConsoleHandle;
void *Exception::sMapFileWorks;
u32 Exception::sMapFileNumMax;
u32 Exception::sCurrentMapFileNum;
void (*Exception::sPreException)();
void (*Exception::sUserCallbackFunc)(CoreStatus *status);
volatile bool Exception::sUserCallbackMode;
s16 Exception::sExceptionDisplayInfo;

void Exception::ExceptionWaitTime(u32 time) {
    u32 tick = OSGetTick();
    u32 tick2;
    do {
        tick2 = OSGetTick();
    } while ((tick2 - tick) / (OS_BUS_CLOCK_SPEED / 4 / 1000) < time);
}

bool Exception::ExceptionCallback(nw4r::db::ConsoleHandle handle, void *data) {
    if (sPreException != nullptr) {
        (*sPreException)();
    }
    nw4r::db::Exception_SetDisplayInfo(sExceptionDisplayInfo);
    nw4r::db::Exception_SetUserCallback(ExceptionCallback_, data);
    return true;
}

bool Exception::ExceptionCallback_(nw4r::db::ConsoleHandle console, void *data) {
    EGG::CoreStatus status;
    if (console == nullptr) {
        OSReport("No Console\n");
        return 0;
    }
    OSEnableInterrupts();
    if (data != nullptr) {
        // Require a specific button sequence to show the exception handler
        int u4 = 0;
        while (((u16 *)data)[u4] != 0) {
            KPADRead(0, &status, 1);
            ExceptionWaitTime(50);
            if (status.mTrig) {
                // Reset the sequence on wrong button press, advance on right button press
                u4 = status.mTrig & (((u16 *)data)[u4]) ? u4 + 1 : 0;
            }
        }
    }
    int line, start, end;
    start = nw4r::db::Console_GetBufferHeadLine(console);
    line = start;
    nw4r::db::Console_SetVisible(console, true);
    end = nw4r::db::Console_GetTotalLines(console);
    for (; line < end; line++) {
        nw4r::db::Console_SetViewBaseLine(console, line);
        nw4r::db::Console_DrawDirect(console);
        ExceptionWaitTime(250);
    }

    int prevY, newY;
    int newX, prevX;

    newY = start;

    nw4r::db::Console_SetViewBaseLine(console, start);
    nw4r::db::Console_DrawDirect(console);
    while (true) {
        KPADRead(0, &status, 1);
        ExceptionWaitTime(100);
        if (status.mTrig & (/* HOME */ 0x8000)) {
            sUserCallbackMode = !sUserCallbackMode;
        }
        if (sUserCallbackMode && sUserCallbackFunc != nullptr) {
            (*sUserCallbackFunc)(&status);
            continue;
        }

        prevX = newX = nw4r::db::Console_GetPositionX(console);
        ;
        prevY = newY;

        if (status.mHold & (/* DPAD_DOWN */ 0x4)) {
            newY += 1;
            if (newY > end) {
                newY = end;
            }
        } else if (status.mHold & (/* DPAD_UP */ 0x8)) {
            newY -= 1;
            if (newY < start) {
                newY = start;
            }
        } else if (status.mHold & (/* DPAD_RIGHT */ 0x2)) {
            newX -= 5;
            if (newX < -150) {
                newX = -150;
            }
        } else if (status.mHold & (/* DPAD_LEFT */ 0x1)) {
            newX += 5;
            if (newX > 10) {
                newX = 10;
            }
        }

        if (newY != prevY || newX != prevX) {
            nw4r::db::Console_SetPosition(console, newX, nw4r::db::Console_GetPositionY(console));
            nw4r::db::Console_SetViewBaseLine(console, newY);
            nw4r::db::Console_DrawDirect(console);
        }
    }
}

Exception *Exception::create(u16 width, u16 height, u16 attr, EGG::Heap *heap, int mapFileNum) {
    if (sException == nullptr) {
        nw4r::db::Exception_Init();
        nw4r::db::DirectPrint_Init();
        if (heap == nullptr) {
            heap = Heap::sCurrentHeap;
        }
        sException = new (heap) Exception(width, height, attr, heap, mapFileNum);
    }

    return sException;
}
void Exception::SetCallbackArgs(void *data) {
    nw4r::db::Exception_SetUserCallback(ExceptionCallback_, data);
    nw4r::db::Exception_SetUserCallback(ExceptionCallback, data);
    sExceptionDisplayInfo = nw4r::db::Exception_SetDisplayInfo(0);
}

void Exception::SetPreExceptionCallback(void (*cb)()) {
    sPreException = cb;
}

void Exception::SetUserCallback(void (*cb)(CoreStatus *status)) {
    sUserCallbackFunc = cb;
}

void Exception::SetCallbackMode(bool arg) {
    sUserCallbackMode = arg;
}

nw4r::db::ConsoleHandle Exception::GetConsoleHandle() {
    return sConsoleHandle;
}

Exception::~Exception() {
    nw4r::db::ConsoleHandle h = sConsoleHandle;
    if (h != nullptr) {
        nw4r::db::ConsoleHandle handle = nw4r::db::Console_Destroy(h);
        Heap::free(handle, nullptr);
        sConsoleHandle = nullptr;
    }
    nw4r::db::MapFile_UnregistAll();
    if (sMapFileWorks != nullptr) {
        Heap::free(sMapFileWorks, nullptr);
        sMapFileWorks = nullptr;
        sMapFileNumMax = 0;
        sCurrentMapFileNum = 0;
    }
}

Exception::Exception(u16 width, u16 height, u16 attr, EGG::Heap *heap, int mapFileNum) {
    void *data = Heap::alloc((width + 1) * height + 0x2C, 4, heap);
    nw4r::db::ConsoleHandle handle = nw4r::db::Console_Create(data, width, height, 18, 0, attr);
    sConsoleHandle = handle;
    if (handle == nullptr) {
        return;
    }

    const GXRenderModeObj *o = EGG::BaseSystem::getVideo()->getCurrentOrFallbackRenderModeObj();
    // Fakematch? Everything before Exception_SetConsole matches if it's volatile, but things after
    // it break
    nw4r::db::Exception_SetConsole(*(nw4r::db::ConsoleHandle *volatile)&sConsoleHandle, o);

    nw4r::db::Console_SetPosition(sConsoleHandle, 0, 30);
    nw4r::db::Console_SetVisible(sConsoleHandle, false);

    sMapFileNumMax = mapFileNum;
    sCurrentMapFileNum = 0;
    sMapFileWorks = Heap::alloc(mapFileNum * 0x14, 4, heap);
    SetCallbackArgs(nullptr);
}

} // namespace EGG
