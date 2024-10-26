#ifndef D_FLAG_FLAG_INDEX_H
#define D_FLAG_FLAG_INDEX_H

#include "common.h"

/**
 * @brief Defines a single story or item flag.
 */
struct FlagDefinition {
    u8 mIndex;
    u8 mShiftMask;
};

/**
 * @brief Helper class for packing and unpacking flag data.
 * 
 * Some flag managers store their data in a packed format, where
 * flags have a variable number of bits and can appear in varied
 * locations within a halfword.
 */
class FlagIndex {
    u16 mDefinitionsCount;
    FlagDefinition *mpDefinitions;

public:
    u16 calculateMask(s32 shift);
    void doNothing(u32 unused);
    FlagIndex(u16 count, FlagDefinition *definitions);
    void prepareIndexShiftMask(u16 counterIdx, u32 flagCount, u16 *pIndex, u8 *pShift, u16 *pMask);
    u16 maskForIdx(u16 index, u16 flagCount);
    u16 getCounterOrFlag(u16 counterIdx, const u16 *pData, u32 flagCount);
    void setCounterOrFlag(u16 counterIdx, u16 *pData, u32 flagCount, u32 value);

    u32 checkFlagValid(u16 counterIdx, u32 flagCount);
};

#endif
