#ifndef EGG_ASSERT_H
#define EGG_ASSERT_H

#include <common.h>
#include <Runtime.PPCEABI.H/__va_arg.h>

namespace EGG {
// All this is guess
// TODO: Fixup funtions
namespace Assert {

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
