#pragma once

#include <types.h>

struct UnkFlagDefinition {
    u8 index;
    u8 shiftMask;
};

class UnkFlagStuff {
u16 definitionsCount;
UnkFlagDefinition* definitions;
public:
    u16 calculateMask(s32 shift);
    void doNothing(u32 unused);
    UnkFlagStuff(u16 count, UnkFlagDefinition* definitions);
    void prepareIndexShiftMask(u16 counterIdx, u32 unused, u16* indexPtr, u8* shiftPtr, u16* maskPtr);
    u16 maskForIdx(u16 index);
    u32 getCounterOrFlag(u16 counterIdx, u16* dataPtr, u32 param4);
    void setCounterOrFlag(u16 counterIdx, u16* dataPtr, u32 flagCount, u32 value);
    u32 unk(u16 param1, u32 param2);
};

u16 UnkFlagStuff::calculateMask(s32 shift) {
    return (2 << (shift & 0xF)) - 1;
}

void UnkFlagStuff::doNothing(u32 unused) {}

UnkFlagStuff::UnkFlagStuff(u16 count, UnkFlagDefinition* definitions) {
    this->definitionsCount = count;
    this->definitions = definitions;
}

void UnkFlagStuff::prepareIndexShiftMask(u16 counterIdx, u32 unused, u16* indexPtr, u8* shiftPtr, u16* maskPtr) {
    doNothing(counterIdx);
    *indexPtr = definitions[counterIdx].index;
    *shiftPtr = definitions[counterIdx].shiftMask >> 4;
    *maskPtr = calculateMask(definitions[counterIdx].shiftMask);
}

u16 UnkFlagStuff::maskForIdx(u16 index) {
    doNothing(index);

    return calculateMask(definitions[index].shiftMask);
}

u32 UnkFlagStuff::getCounterOrFlag(u16 counterIdx, u16* dataPtr, u32 param4) {
    u16 index = 0;
    u8 shift = 0;
    u16 mask = 0;
    prepareIndexShiftMask(counterIdx, param4, &index, &shift, &mask);
    return mask & (dataPtr[index] >> shift);
}

void UnkFlagStuff::setCounterOrFlag(u16 counterIdx, u16* dataPtr, u32 flagCount, u32 value) {
    u16 index = 0;
    u8 shift = 0;
    u16 mask = 0;
    prepareIndexShiftMask(counterIdx, flagCount, &index, &shift, &mask);
    dataPtr[index] &= ~(mask << shift);
    dataPtr[index] = (u16)(value << shift) | dataPtr[index];
}

inline bool check(int a, int b) {
    return a <= b;
}

u32 UnkFlagStuff::unk(u16 param1, u32 param2) {
    if (definitions == nullptr) {
        return 0;
    }
    if (definitionsCount <= param1 || definitions[param1].index == 0xFF) {
        return 0;
    } else {
        if (!check(param2, definitions[param1].index)) {
            return 1;
        } else {
            return 0;
        }
    }
}
