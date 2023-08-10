#pragma once

#include "types.h"

struct UnkFlagDefinition {
    u8 mIndex;
    u8 mShiftMask;
};

class UnkFlagStuff {
    u16 mDefinitionsCount;
    UnkFlagDefinition* mpDefinitions;
public:
    u16 calculateMask(s32 shift);
    void doNothing(u32 unused);
    UnkFlagStuff(u16 count, UnkFlagDefinition* definitions);
    void prepareIndexShiftMask(u16 counterIdx, u32 flagCount, u16* pIndex, u8* pShift, u16* pMask);
    u16 maskForIdx(u16 index);
    u32 getCounterOrFlag(u16 counterIdx, u16* pData, u32 flagCount);
    void setCounterOrFlag(u16 counterIdx, u16* pData, u32 flagCount, u32 value);
    u32 checkFlagValid(u16 counterIdx, u32 flagCount);
};
