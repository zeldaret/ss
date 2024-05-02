#include <toBeSorted/counters/counter.h>

class KeyPieceCounter : public Counter {
public:
    KeyPieceCounter();
    /* 8016E1A0 */ ~KeyPieceCounter() {}
    virtual u16 getMax() override;
};

/* 80575638 */ KeyPieceCounter lbl_80575638;

/* 8016E1E0 */ KeyPieceCounter::KeyPieceCounter() : Counter(0x1f9) {}
/* 8016E200 */ u16 KeyPieceCounter::getMax() {
    return 5;
}
