#include "toBeSorted/counters/counter.h"

class SlingshotSeedCounter : public Counter {
public:
    SlingshotSeedCounter();
    ~SlingshotSeedCounter() {}
    virtual u16 getMax() const override {
        return 20;
    };
};

SlingshotSeedCounter SLINGSHOT_SEED_COUNTER;

SlingshotSeedCounter::SlingshotSeedCounter() : Counter(0x1ed) {}
