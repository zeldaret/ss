#ifndef GODDESS_CHEST_COUNTER_H
#define GODDESS_CHEST_COUNTER_H

#include "toBeSorted/counters/counter.h"

class GoddessChestCounter : public Counter {
public:
    GoddessChestCounter() : Counter(0x43F) {}
    virtual ~GoddessChestCounter() {};

    virtual u16 getCommittedValue() const override;
    virtual u16 getMax() const override;
    virtual u16 getUncommittedValue() const override;
    virtual void setValue(u16 num) override;

    static GoddessChestCounter sInstance;
};

#endif
