#include <toBeSorted/counters/counter.h>

class ArrowCounter : public Counter {
public:
    ArrowCounter();
    /* 8016DFA0 */ ~ArrowCounter() {}
    /* 8016E000 */ virtual u16 getMax() override {
        return 20;
    };
};

/* 80575618 */ ArrowCounter ARROW_COUNTER;

/* 8016DFE0 */ ArrowCounter::ArrowCounter() : Counter(0x1f2) {}
