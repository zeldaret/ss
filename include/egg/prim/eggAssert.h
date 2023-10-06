#pragma once

#include "types.h"

namespace EGG
{
// All this is guess
// TODO: Fixup funtions
namespace Assert
{

/* 8049bf90 */ void wait(u32 time);
/* 8049c010 */ void system_print();
/* 8049c010 */ void system_report();
/* 8049c0a0 */ s32 getPeriodPos(char*);
/* 8049c100 */ char* getMapSymbol();
/* 8049c150 */ bool isOutsideMEM1(u32 addr);
/* 8049c190 */ void system_halt();
/* 8049c530 */ void assert();

} // namespace Assert

} // namespace EGG
