#include <toBeSorted/counters/counter.h>

class TearCounter : public Counter {
public:
    TearCounter();
    /* 8016E0A0 */ ~TearCounter() {}
    virtual u16 getMax() override;
};

/* 80575628 */ TearCounter lbl_80575628;

/* 8016E0E0 */ TearCounter::TearCounter(): Counter(0x1f4) { }
/* 8016E100 */ u16 TearCounter::getMax() {
    return 15;
}
