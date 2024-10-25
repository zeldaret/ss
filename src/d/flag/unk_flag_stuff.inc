#include "d/flag/unk_flag_stuff.h"

u16 UnkFlagStuff::calculateMask(s32 shift) {
    return (2 << (shift & 0xF)) - 1;
}

void UnkFlagStuff::doNothing(u32 unused) {}

UnkFlagStuff::UnkFlagStuff(u16 count, UnkFlagDefinition *definitions) {
    this->mDefinitionsCount = count;
    this->mpDefinitions = definitions;
}

void UnkFlagStuff::prepareIndexShiftMask(u16 counterIdx, u32 unused, u16 *indexPtr, u8 *shiftPtr, u16 *maskPtr) {
    doNothing(counterIdx);
    *indexPtr = mpDefinitions[counterIdx].mIndex;
    *shiftPtr = mpDefinitions[counterIdx].mShiftMask >> 4;
    *maskPtr = calculateMask(mpDefinitions[counterIdx].mShiftMask);
}

u16 UnkFlagStuff::maskForIdx(u16 index, u16 flagSizeMaybe) {
    doNothing(index);

    return calculateMask(mpDefinitions[index].mShiftMask);
}

u16 UnkFlagStuff::getCounterOrFlag(u16 counterIdx, const u16 *dataPtr, u32 param4) {
    u16 index = 0;
    u8 shift = 0;
    u16 mask = 0;
    prepareIndexShiftMask(counterIdx, param4, &index, &shift, &mask);
    return mask & (dataPtr[index] >> shift);
}

void UnkFlagStuff::setCounterOrFlag(u16 counterIdx, u16 *dataPtr, u32 flagCount, u32 value) {
    u16 index = 0;
    u8 shift = 0;
    u16 mask = 0;
    prepareIndexShiftMask(counterIdx, flagCount, &index, &shift, &mask);
    dataPtr[index] &= ~(mask << shift);
    dataPtr[index] = (u16)(value << shift) | dataPtr[index];
}

inline bool isLessThan(int a, int b) {
    return a <= b;
}

u32 UnkFlagStuff::checkFlagValid(u16 counterIdx, u32 flagCount) {
    if (mpDefinitions == nullptr) {
        return 0;
    }
    if (mDefinitionsCount <= counterIdx || mpDefinitions[counterIdx].mIndex == 0xFF) {
        return 0;
    } else {
        if (isLessThan(flagCount, mpDefinitions[counterIdx].mIndex)) {
            return 0;
        } else {
            return 1;
        }
    }
}
