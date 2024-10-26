#include "d/flag/flag_index.h"

u16 FlagIndex::calculateMask(s32 shift) {
    return (2 << (shift & 0xF)) - 1;
}

void FlagIndex::doNothing(u32 unused) {}

FlagIndex::FlagIndex(u16 count, FlagDefinition *definitions) {
    this->mDefinitionsCount = count;
    this->mpDefinitions = definitions;
}

void FlagIndex::prepareIndexShiftMask(u16 counterIdx, u32 unused, u16 *indexPtr, u8 *shiftPtr, u16 *maskPtr) {
    doNothing(counterIdx);
    *indexPtr = mpDefinitions[counterIdx].mIndex;
    *shiftPtr = mpDefinitions[counterIdx].mShiftMask >> 4;
    *maskPtr = calculateMask(mpDefinitions[counterIdx].mShiftMask);
}

u16 FlagIndex::maskForIdx(u16 index, u16 flagCount) {
    doNothing(index);

    return calculateMask(mpDefinitions[index].mShiftMask);
}

u16 FlagIndex::getCounterOrFlag(u16 counterIdx, const u16 *dataPtr, u32 param4) {
    u16 index = 0;
    u8 shift = 0;
    u16 mask = 0;
    prepareIndexShiftMask(counterIdx, param4, &index, &shift, &mask);
    return mask & (dataPtr[index] >> shift);
}

void FlagIndex::setCounterOrFlag(u16 counterIdx, u16 *dataPtr, u32 flagCount, u32 value) {
    u16 index = 0;
    u8 shift = 0;
    u16 mask = 0;
    prepareIndexShiftMask(counterIdx, flagCount, &index, &shift, &mask);
    dataPtr[index] &= ~(mask << shift);
    dataPtr[index] = (u16)(value << shift) | dataPtr[index];
}

u32 FlagIndex::checkFlagValid(u16 counterIdx, u32 flagCount) {
    if (mpDefinitions == nullptr) {
        return 0;
    }
    if (mDefinitionsCount <= counterIdx || mpDefinitions[counterIdx].mIndex == 0xFF) {
        return 0;
    } else {
        return (int)mpDefinitions[counterIdx].mIndex < (int)flagCount;
    }
}
