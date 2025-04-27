#include "toBeSorted/counters/counter.h"

class KeyPieceCounter : public Counter {
public:
    KeyPieceCounter();
    /* 8016E1A0 */ ~KeyPieceCounter() {}
    /* 8016E200 */ virtual u16 getMax() const override {
        return 5;
    }
};

/* 80575638 */ KeyPieceCounter KEY_PIECE_COUNTER;

/* 8016E1E0 */ KeyPieceCounter::KeyPieceCounter() : Counter(0x1f9) {}
