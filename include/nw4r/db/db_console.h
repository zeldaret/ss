#ifndef NW4R_DB_CONSOLE_H
#define NW4R_DB_CONSOLE_H

#include <common.h>
#include <nw4r/ut/ut_TextWriterBase.h>

namespace nw4r {
namespace db {

namespace detail {
typedef struct ConsoleHead {
    u8 *textBuf;
    u16 width;
    u16 height;
    u16 priority;
    u16 attr;
    u16 printTop;
    u16 printXPos;
    u16 ringTop;
    u32 ringTopLineCnt;
    s32 viewTopLine;
    s16 viewPosX;
    s16 viewPosY;
    u16 viewLines;
    bool isVisible;
    nw4r::ut::TextWriterBase<char> *writer;
    ConsoleHead *next;
} ConsoleHead;

} // namespace detail

typedef detail::ConsoleHead *ConsoleHandle;

enum ConsoleOutputType {
    CONSOLE_OUTPUT_NONE = 0,
    CONSOLE_OUTPUT_DISPLAY = 1,
    CONSOLE_OUTPUT_TERMINAL = 2,
    CONSOLE_OUTPUT_ALL = CONSOLE_OUTPUT_DISPLAY | CONSOLE_OUTPUT_TERMINAL,
};

ConsoleHandle Console_Create(void *arg, u16, u16, u16, u16, u16);
ConsoleHandle Console_Destroy(ConsoleHandle console);
void Console_DrawDirect(ConsoleHandle console);
void Console_VFPrintf(ConsoleOutputType type, ConsoleHandle console, const char* format, va_list vlist);
void Console_Printf(ConsoleHandle console, const char* format, ...);
s32 Console_GetTotalLines(ConsoleHandle console);

} // namespace db
} // namespace nw4r


#endif
