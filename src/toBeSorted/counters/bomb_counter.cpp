#include <toBeSorted/counters/counter.h>

class BombCounter : public Counter {
public:
    BombCounter();
    /* 8016E020 */ ~BombCounter() {}
    /* 8016E080 */ virtual u16 getMax() override {
        return 10;
    }
};

/* 80575620 */ BombCounter BOMB_COUNTER;

/* 8016E060 */ BombCounter::BombCounter(): Counter(0x1f3) { }
