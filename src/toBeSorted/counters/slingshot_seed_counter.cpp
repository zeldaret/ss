#include "toBeSorted/counters/counter.h"

class SlingshotSeedCounter : public Counter {
public:
    SlingshotSeedCounter();
    /* 8016E120 */ ~SlingshotSeedCounter() {}
    /* 8016E180 */ virtual u16 getMax() override {
        return 20;
    };
};

/* 80575630 */ SlingshotSeedCounter SLINGSHOT_SEED_COUNTER;

/* 8016E160 */ SlingshotSeedCounter::SlingshotSeedCounter() : Counter(0x1ed) {}
