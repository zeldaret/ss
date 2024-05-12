#ifndef EGG_ASSERT_H
#define EGG_ASSERT_H

#include <common.h>

namespace EGG {
// All this is guess
// TODO: Fixup funtions
namespace Assert {

typedef void (*AssertCallback)();

/* 8049bf90 */ void wait(u32 time);
/* 8049c010 */ void system_vreport(const char *str, va_list list);
/* 8049c010 */ void system_report(const char *str, ...);
/* 8049c0a0 */ s32 getPeriodPos(const char *);
/* 8049c100 */ char *getMapSymbol();
/* 8049c150 */ bool isOutsideMEM1(u32 addr);
/* 8049c180 */ AssertCallback setAssertCallback(AssertCallback cb);
/* 8049c190 */ void system_halt(const char *file, u32 line, const char *msg, va_list list);
/* 8049c530 */ void assert(const char *file, u32 line, const char *msg, ...);

} // namespace Assert

} // namespace EGG

#endif
