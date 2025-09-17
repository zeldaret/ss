#include "toBeSorted/counters/counter.h"

#include "d/flag/itemflag_manager.h"

s32 Counter::checkedAdd(s32 num) {
    s32 uncommitted = getUncommittedValue();
    s32 max = getMax();
    s32 result = uncommitted + num;
    if (result < 0) {
        setValue(0);
    } else if (result < max) {
        setValue(result);
    } else {
        setValue(max);
    }

    if (result < 0) {
        return result;
    }

    return result <= max ? 0 : (result - max);
}

u16 Counter::getCommittedValue() const {
    return ItemflagManager::sInstance->getItemCounterOrFlag(counterId);
}

u16 Counter::getUncommittedValue() const {
    return ItemflagManager::sInstance->getUncommittedItemValue(counterId);
}

void Counter::setValue(u16 num) {
    ItemflagManager::sInstance->setItemFlagOrCounterToValue(counterId, num);
}
