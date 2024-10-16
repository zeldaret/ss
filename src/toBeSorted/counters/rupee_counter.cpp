#include "toBeSorted/counters/counter.h"
#include "toBeSorted/item_story_flag_manager.h"

static u16 getBaseCapacity();
static u16 getExtraWalletCapacity();

class RupeeCounter : public Counter {
public:
    RupeeCounter();
    /* 8016DEF0 */ ~RupeeCounter() {}
    /* 8016DF50 */ virtual u16 getMax() override {
        return (getBaseCapacity() + getExtraWalletCapacity());
    }
};

struct WalletStruct {
    u32 flag;
    u16 capacity;
};

/* 8016DE10 */ static u16 getBaseCapacity() {
    int i = 0;
    /* 804E91B0 */ WalletStruct wallet_definitions[4] = {
        {0x6c,  500},
        {0x6d, 1000},
        {0x6e, 5000},
        {0x6f, 9000},
    };
    const WalletStruct *wallet = &wallet_definitions[3];
    for (; i < 4; i++, wallet--) {
        if (ItemflagManager::sInstance->getFlag(wallet->flag)) {
            return wallet->capacity;
        }
    }
    return 300;
}

// TODO main counters class
extern "C" u16 getCounterByIndex(u16);

/* 8016DEC0 */ static u16 getExtraWalletCapacity() {
    return 300 * getCounterByIndex(0x27);
}

/* 80575610 */ RupeeCounter RUPEE_COUNTER;

/* 8016DF30 */ RupeeCounter::RupeeCounter() : Counter(0x1f5) {}
