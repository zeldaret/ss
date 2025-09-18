#include "toBeSorted/counters/counter.h"

class ArrowCounter : public Counter {
public:
    ArrowCounter();
    ~ArrowCounter() {}
    virtual u16 getMax() const override {
        return 20;
    };
};

ArrowCounter ARROW_COUNTER;

ArrowCounter::ArrowCounter() : Counter(0x1f2) {}
