#include <toBeSorted/counters/counter.h>

class ExtraWalletCounter : public Counter {
public:
    ExtraWalletCounter();
    /* 8016E220 */ ~ExtraWalletCounter() {}
    /* 8016E280 */ virtual u16 getMax() override {
        return 3;
    }
};


/* 80575640 */ ExtraWalletCounter lbl_80575640;

/* 8016E260 */ ExtraWalletCounter::ExtraWalletCounter(): Counter(0x1fc) { }
