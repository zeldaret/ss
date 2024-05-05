#ifndef COUNTER_H
#define COUNTER_H

#include <common.h>

class Counter {
public:
    Counter(u16 id): counterId(id) {}
    virtual ~Counter() {};
    virtual s32 checkedAdd(s32 num);
    virtual u16 getCommittedValue();
    virtual u16 getMax() = 0;
    virtual u16 getUncommittedValue();
    virtual void setValue(u16 num);

    u16 counterId;
};

#endif COUNTER_H
