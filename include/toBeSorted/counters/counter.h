#ifndef COUNTER_H
#define COUNTER_H

#include "common.h"

class Counter {
public:
    Counter(u16 id) : counterId(id) {}
    virtual ~Counter() {}
    virtual s32 checkedAdd(s32 num);
    virtual u16 getCommittedValue() const;
    virtual u16 getMax() const = 0;
    virtual u16 getUncommittedValue() const;
    virtual void setValue(u16 num);

    u16 counterId;
};

s32 increaseCounter(u16 counterIdx, s32 value);
s32 getCounterByIndex(u16 counterIdx);
s32 getMaxItemCount(u16 counterIdx);

#endif
