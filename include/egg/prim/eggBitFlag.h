#ifndef EGG_BIT_FLAG_H
#define EGG_BIT_FLAG_H

// Pulled from ogws

#include <common.h>

namespace EGG {

template <typename T>
class TBitFlag {
public:
    T value;
    void makeAllZero() {
        value = T();
    }
    TBitFlag() {}
    TBitFlag(T value) : value(value) {}
    T makeMask(u8 bit) const {
        return 1 << bit;
    }
    bool on(T mask) const {
        return (value & mask);
    }
    bool off(T mask) const {
        return !(value & mask);
    }
    void set(T mask) {
        value |= mask;
    }
    void reset(T mask) {
        value &= ~mask;
    }
    bool onBit(u8 bit) const {
        return on(makeMask(bit));
    }
    bool offBit(u8 bit) const {
        return off(makeMask(bit));
    }
    void setBit(u8 bit) {
        set(makeMask(bit));
    }
    void resetBit(u8 bit) {
        reset(makeMask(bit));
    }
    void toggleBit(u8 bit) {
        if (!onBit(bit)) {
            setBit(bit);
        } else {
            resetBit(bit);
        }
    }
    void changeBit(u8 bit, bool b) {
        if (b) {
            setBit(bit);
        } else {
            resetBit(bit);
        }
    }
    T getDirect() const {
        return value;
    }
};

} // namespace EGG

#endif
