#include "nw4r/db/db_console.h"

#include "nw4r/db/db_directPrint.h"
#include "nw4r/ut/ut_TextWriterBase.h"
#include "rvl/OS.h" // IWYU pragma: export

#include <common.h>

namespace nw4r {
namespace db {

static ConsoleHandle sConsoleList;
static OSMutex sMutex;
static u8 sStrBuf[0x400];
bool sInited;

static ConsoleHandle FindInsertionPosition_(u16 priority) {
    ConsoleHandle cursor = sConsoleList;

    if (cursor == nullptr || cursor->priority < priority) {
        return nullptr;
    }

    while (cursor->next != nullptr) {
        if (cursor->next->priority < priority) {
            return cursor;
        }
        cursor = cursor->next;
    }
    return cursor;
}

static void Console_Append_(ConsoleHandle console) {
    OSLockMutex(&sMutex);

    ConsoleHandle cursor = FindInsertionPosition_(console->priority);

    if (cursor == nullptr) {
        console->next = sConsoleList;
        sConsoleList = console;
    } else {
        console->next = cursor->next;
        cursor->next = console;
    }
    OSUnlockMutex(&sMutex);
}

void RemoveConsoleFromList_(ConsoleHandle console) {
    OSLockMutex(&sMutex);

    if (sConsoleList == console) {
        sConsoleList = console->next;
        console->next = nullptr;
    } else {
        ConsoleHandle cursor = sConsoleList;
        while (cursor->next != nullptr) {
            if (cursor->next == console) {
                cursor->next = console->next;
                console->next = nullptr;
                goto end;
            }
            cursor = cursor->next;
        }
#line 386
        OSError("illegal console handle");
    }

end:
    OSUnlockMutex(&sMutex);
}

static bool TryLockMutex_(OSMutex *mutex) {
    if (OSGetCurrentThread() != nullptr) {
        OSLockMutex(&sMutex);
        return true;
    }

    if (mutex->thread == nullptr) {
        return true;
    }

    return false;
}

static void UnlockMutex_(OSMutex *mutex) {
    if (OSGetCurrentThread() != nullptr) {
        OSUnlockMutex(&sMutex);
    }
}

ConsoleHandle Console_Create(void *arg, u16 width, u16 height, u16 viewLines, u16 priority, u16 attr) {
    ConsoleHandle console = (ConsoleHandle)arg;
    if (!sInited) {
        OSInitMutex(&sMutex);
        sInited = true;
    }
    console->textBuf = (u8 *)(console + 1);
    console->width = width;
    console->height = height;
    console->priority = priority;
    console->attr = attr;
    console->isVisible = false;
    console->printTop = 0;
    console->printXPos = 0;
    console->ringTop = 0;
    console->ringTopLineCnt = 0;
    console->viewTopLine = 0;
    console->viewPosX = 0x1e;
    console->viewPosY = 0x32;
    console->viewLines = viewLines;
    console->writer = nullptr;

    if (TryLockMutex_(&sMutex)) {
        BOOL intr = OSDisableInterrupts();
        console->printTop = 0;
        console->printXPos = 0;
        console->ringTop = 0;
        console->ringTopLineCnt = 0;
        console->viewTopLine = 0;
        OSRestoreInterrupts(intr);
        UnlockMutex_(&sMutex);
    }

    Console_Append_(console);
    return console;
}

ConsoleHandle Console_Destroy(ConsoleHandle console) {
    RemoveConsoleFromList_(console);
    return console;
}

static u8 *GetTextPtr_(ConsoleHandle console, u16 line, u16 xPos) {
    return &console->textBuf[(console->width + 1) * line + xPos];
}

static u16 GetRingUsedLines_(ConsoleHandle console) {
    s32 lines = console->printTop - console->ringTop;
    if (lines < 0) {
        lines += console->height;
    }
    return lines;
}

static u16 GetActiveLines_(ConsoleHandle console) {
    u16 lines = GetRingUsedLines_(console);
    if (console->printXPos != 0) {
        lines += 1;
    }

    return lines;
}

static void DoDrawString_(ConsoleHandle console, u32 printLine, const u8 *str, nw4r::ut::TextWriterBase<char> *writer) {
    if (writer != nullptr) {
        writer->Printf("%s\n", str);
    } else {
        s32 height = console->viewPosY + printLine * 10;
        db::DirectPrint_DrawString(console->viewPosX, height, false, "%s\n", str);
    }
}

static void DoDrawConsole_(ConsoleHandle console, nw4r::ut::TextWriterBase<char> *writer) {
    // I guess we don't care if locking the mutex fails??
    TryLockMutex_(&sMutex);

    s32 viewOffset = console->viewTopLine - console->ringTopLineCnt;
    u16 line;
    u16 printLines = 0;
    u16 topLine;

    if (viewOffset < 0) {
        viewOffset = 0;
    } else if (viewOffset > GetActiveLines_(console)) {
        goto err;
    }

    line = console->ringTop + viewOffset;
    if (line >= console->height) {
        line -= console->height;
    }

    topLine = console->printTop + ((console->printXPos > 0) ? 1 : 0);
    if (topLine == console->height) {
        topLine = 0;
    }

    while (line != topLine) {
        DoDrawString_(console, printLines, GetTextPtr_(console, line, 0), writer);
        printLines++;
        line++;
        if (line == console->height) {
            if ((console->attr & 2) != 0) {
                break;
            }
            line = 0;
        }
        if (printLines >= console->viewLines) {
            break;
        }
    }

err:
    UnlockMutex_(&sMutex);
}

void Console_DrawDirect(ConsoleHandle console) {
    if (DirectPrint_IsActive() && console->isVisible) {
        DirectPrint_EraseXfb(
            console->viewPosX - 6, console->viewPosY - 3, console->width * 6 + 12, console->viewLines * 10 + 4
        );
        DoDrawConsole_(console, nullptr);
        DirectPrint_StoreCache();
    }
}

static void TerminateLine_(ConsoleHandle console) {
    *GetTextPtr_(console, console->printTop, console->printXPos) = '\0';
}

static u32 GetTabSize_(ConsoleHandle console) {
    s32 tab = (console->attr & 12) >> 2;
    return 2 << tab;
}

static u32 CodeWidth_(const u8 *p) {
    return (*p >= 0x81) ? 2 : 1;
}

static u32 PutChar_(ConsoleHandle console, const u8 *str, u8 *dstPtr) {
    u32 codeWidth;
    u32 count;

    codeWidth = CodeWidth_(str);
    if (console->printXPos + codeWidth > console->width) {
        return 0;
    } else {
        console->printXPos += codeWidth;
        for (count = codeWidth; count > 0; count--) {
            *dstPtr++ = *str++;
        }
        return codeWidth;
    }
}

static u8 *PutTab_(ConsoleHandle console, u8 *dstPtr) {
    u32 tabWidth = GetTabSize_(console);
    do {
        *dstPtr++ = ' ';
        console->printXPos++;
        if (console->printXPos >= console->width) {
            break;
        }
    } while ((console->printXPos & (tabWidth - 1)) != 0);
    return dstPtr;
}

static u8 *SearchEndOfLine_(const u8 *str) {
    while (*str != '\n' && *str != '\0') {
        str++;
    }
    return (u8 *)str;
}

static u8 *NextLine_(ConsoleHandle console) {
    *GetTextPtr_(console, console->printTop, console->printXPos) = '\0';
    console->printXPos = 0;
    console->printTop++;
    if (console->printTop == console->height && (console->attr & 2) == 0) {
        console->printTop = 0;
    }

    if (console->printTop == console->ringTop) {
        console->ringTopLineCnt++;
        if (++console->ringTop == console->height) {
            console->ringTop = 0;
        }
    }

    return GetTextPtr_(console, console->printTop, 0);
}

static void PrintToBuffer_(ConsoleHandle console, const u8 *str) {
    BOOL enabled = OSDisableInterrupts();
    u8 *storePtr = GetTextPtr_(console, console->printTop, console->printXPos);

    while (*str != '\0') {
        if ((console->attr & 2) && console->printTop == console->height) {
            break;
        }

        while (*str != '\0') {
            bool newLineFlag = false;
            if (*str == '\n') {
                str++;
                storePtr = NextLine_(console);
                break;
            } else if (*str == '\t') {
                str++;
                storePtr = PutTab_(console, storePtr);
            } else {
                u32 bytes = PutChar_(console, str, storePtr);
                if (bytes > 0) {
                    str += bytes;
                    storePtr += bytes;
                } else {
                    newLineFlag = true;
                }
            }

            if (console->printXPos >= console->width) {
                newLineFlag = true;
            }

            if (newLineFlag) {
                if ((console->attr & 1) != 0) {
                    str = SearchEndOfLine_(str);
                } else {
                    if (*str == '\n') {
                        str++;
                    }
                    storePtr = NextLine_(console);
                }
                break;
            }

            if (*str == '\0') {
                TerminateLine_(console);
            }
        }
    }

    OSRestoreInterrupts(enabled);
}

static void Console_PrintString_(ConsoleOutputType type, ConsoleHandle console, const u8 *str) {
    if ((type & CONSOLE_OUTPUT_TERMINAL) != 0) {
        OSReport("%s", str);
    }
    if ((type & CONSOLE_OUTPUT_DISPLAY) != 0) {
        PrintToBuffer_(console, str);
    }
}

void Console_VFPrintf(ConsoleOutputType type, ConsoleHandle console, const char *format, va_list vlist) {
    if (TryLockMutex_(&sMutex)) {
        vsnprintf((char *)sStrBuf, sizeof(sStrBuf), format, vlist);
        Console_PrintString_(type, console, sStrBuf);

        UnlockMutex_(&sMutex);
    }
}
void Console_Printf(ConsoleHandle console, const char *format, ...) {
    va_list args;
    va_start(args, format);

    if (TryLockMutex_(&sMutex)) {
        vsnprintf((char *)sStrBuf, sizeof(sStrBuf), format, args);
        OSReport("%s", sStrBuf);
        PrintToBuffer_(console, sStrBuf);

        UnlockMutex_(&sMutex);
    }
}

s32 Console_GetTotalLines(ConsoleHandle console) {
    s32 count;
    BOOL enabled = OSDisableInterrupts();

    count = console->ringTopLineCnt + GetActiveLines_(console);

    OSRestoreInterrupts(enabled);

    return count;
}

} // namespace db
} // namespace nw4r
