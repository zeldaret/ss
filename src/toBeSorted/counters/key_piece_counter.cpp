#include <toBeSorted/counters/counter.h>

class KeyPieceCounter : public Counter {
public:
    KeyPieceCounter();
    /* 8016E1A0 */ ~KeyPieceCounter() {}
    /* 8016E200 */ virtual u16 getMax() override {
        return 5;
    }
};

/* 80575638 */ KeyPieceCounter lbl_80575638;

/* 8016E1E0 */ KeyPieceCounter::KeyPieceCounter() : Counter(0x1f9) {}
