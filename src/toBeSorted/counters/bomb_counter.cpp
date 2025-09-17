#include "toBeSorted/counters/counter.h"

class BombCounter : public Counter {
public:
    BombCounter();
    ~BombCounter() {}
    virtual u16 getMax() const override {
        return 10;
    }
};

BombCounter BOMB_COUNTER;

BombCounter::BombCounter() : Counter(0x1f3) {}
