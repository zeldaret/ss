#include <toBeSorted/counters/counter.h>

class ExtraWalletCounter : public Counter {
public:
    ExtraWalletCounter();
    /* 8016E220 */ ~ExtraWalletCounter() {}
    virtual u16 getMax() override;
};


/* 80575640 */ ExtraWalletCounter lbl_80575640;

/* 8016E260 */ ExtraWalletCounter::ExtraWalletCounter(): Counter(0x1fc) { }
/* 8016E280 */ u16 ExtraWalletCounter::getMax() {
    return 3;
}
