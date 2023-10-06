#pragma once

// Pulled from ogws

#include "types.h"

namespace EGG
{
    
template<typename T> class TBitFlag {
public:
    T value;
    inline void makeAllZero() {
        value = T();
    }
    inline TBitFlag() {
        makeAllZero();
    }
    inline T makeMask(u8 bit) const {
        return 1 << bit;
    }
    inline bool on(T mask) const {
        return value & mask;
    }
    inline bool off(T mask) const {
        return !(value & mask);
    }
    inline void set(T mask) {
        value |= mask;
    }
    inline void reset(T mask) {
        value &= ~mask;
    }
    inline bool onBit(u8 bit) const {
        return on(makeMask(bit));
    }
    inline bool offBit(u8 bit) const {
        return off(makeMask(bit));
    }
    inline void setBit(u8 bit) {
        set(makeMask(bit));
    }
    inline void resetBit(u8 bit) {
        reset(makeMask(bit));
    }
    inline void toggleBit(u8 bit) {
        if (!onBit(bit))
            setBit(bit);
        else
            resetBit(bit);
    }
    inline void changeBit(u8 bit, bool b) {
        if (b)
            setBit(bit);
        else
            resetBit(bit);
    }
    inline T getDirect() {
        return value;
    }
};

} // namespace EGG
