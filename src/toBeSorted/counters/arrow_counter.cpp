#include <toBeSorted/counters/counter.h>

class ArrowCounter : public Counter {
public:
    ArrowCounter();
    /* 8016DFA0 */ ~ArrowCounter() {}
    virtual u16 getMax() override;
};

/* 80575618 */ ArrowCounter lbl_80575618;

/* 8016DFE0 */ ArrowCounter::ArrowCounter() : Counter(0x1f2) {}
/* 8016E000 */ u16 ArrowCounter::getMax() {
    return 20;
}
