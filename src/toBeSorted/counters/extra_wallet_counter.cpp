#include "toBeSorted/counters/counter.h"

class ExtraWalletCounter : public Counter {
public:
    ExtraWalletCounter();
    ~ExtraWalletCounter() {}
    virtual u16 getMax() const override {
        return 3;
    }
};

ExtraWalletCounter EXTRA_WALLET_COUNTER;

ExtraWalletCounter::ExtraWalletCounter() : Counter(0x1fc) {}
