#include "d/a/d_a_item.h"
#include "d/flag/itemflag_manager.h"
#include "toBeSorted/counters/counter.h"

static u16 getBaseCapacity();
static u16 getExtraWalletCapacity();

class RupeeCounter : public Counter {
public:
    RupeeCounter();
    ~RupeeCounter() {}
    virtual u16 getMax() const override {
        return (getBaseCapacity() + getExtraWalletCapacity());
    }
};

struct WalletStruct {
    u32 flag;
    u16 capacity;
};

static u16 getBaseCapacity() {
    int i = 0;
    WalletStruct wallet_definitions[4] = {
        {ITEM_MEDIUM_WALLET,  500},
        {   ITEM_BIG_WALLET, 1000},
        { ITEM_GIANT_WALLET, 5000},
        {ITEM_TYCOON_WALLET, 9000},
    };
    const WalletStruct *wallet = &wallet_definitions[3];
    for (; i < 4; i++, wallet--) {
        if (ItemflagManager::sInstance->getFlagDirect(wallet->flag)) {
            return wallet->capacity;
        }
    }
    return 300;
}

static u16 getExtraWalletCapacity() {
    return 300 * getCounterByIndex(0x27); // Maybe 0x27 corresponds to item 27 -> small wallet?
}

RupeeCounter RUPEE_COUNTER;

RupeeCounter::RupeeCounter() : Counter(0x1f5) {}
