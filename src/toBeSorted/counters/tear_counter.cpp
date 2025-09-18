#include "toBeSorted/counters/counter.h"

class TearCounter : public Counter {
public:
    TearCounter();
    ~TearCounter() {}
    virtual u16 getMax() const override {
        return 15;
    };
};

TearCounter TEAR_COUNTER;

TearCounter::TearCounter() : Counter(0x1f4) {}
