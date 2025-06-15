#ifndef EGG_ASSERT_H
#define EGG_ASSERT_H

#include "__va_arg.h"
#include "common.h"

namespace EGG {
// All this is guess
// TODO: Fixup funtions
namespace Assert {

#if 1
#define EGG_ASSERT(EXP)
#define EGG_ASSERT_MSG(EXP, ...)
#define EGG_PRINT(...)
#else
#define EGG_ASSERT(EXP)                                                                                                \
    if (!(EXP))                                                                                                        \
    system_halt(__FILE__, __LINE__, #EXP)

#define EGG_ASSERT_MSG(EXP, ...)                                                                                       \
    if (!(EXP))                                                                                                        \
    system_halt(__FILE__, __LINE__, __VA_ARGS__)

#define EGG_PRINT(...) system_print(true, __FILE__, __VA_ARGS__)
#endif

typedef void (*AssertCallback)();
void wait(u32 time);
void system_vreport(const char *str, va_list list);
void system_report(const char *str, ...);
s32 getPeriodPos(const char *);
char *getMapSymbol();
bool isOutsideMEM1(u32 addr);
AssertCallback setAssertCallback(AssertCallback cb);
void system_halt(const char *file, u32 line, const char *msg, va_list list);
void assert(const char *file, u32 line, const char *msg, ...);

} // namespace Assert

} // namespace EGG

#endif
