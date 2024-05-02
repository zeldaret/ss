#include <toBeSorted/counters/counter.h>

class BombCounter : public Counter {
public:
    BombCounter();
    /* 8016E020 */ ~BombCounter() {}
    virtual u16 getMax() override;
};

/* 80575620 */ BombCounter lbl_80575620;

/* 8016E060 */ BombCounter::BombCounter(): Counter(0x1f3) { }
/* 8016E080 */ u16 BombCounter::getMax() {
    return 10;
}
