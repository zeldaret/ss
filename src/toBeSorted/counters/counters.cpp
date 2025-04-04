#include "d/flag/dungeonflag_manager.h"
#include "toBeSorted/counters/counter.h"

class SmallKeyCounter : public Counter {
public:
    SmallKeyCounter() : Counter(0xF) {}
    virtual u16 getMax() const override {
        return 15;
    }
    virtual u16 getCommittedValue() const override;
    virtual u16 getUncommittedValue() const override;
    virtual void setValue(u16 num) override;
};

u16 SmallKeyCounter::getCommittedValue() const {
    u16 counter = counterId;
    return DungeonflagManager::sInstance->getCounterOrFlag(counter, 8);
}

u16 SmallKeyCounter::getUncommittedValue() const {
    return DungeonflagManager::sInstance->getDungeonFlag(counterId);
}

void SmallKeyCounter::setValue(u16 num) {
    return DungeonflagManager::sInstance->setToValue(counterId, num);
}

static SmallKeyCounter sSmallKeyCounter;

#define MAKE_COMMON_COUNTER(name, id, max)                                                                             \
    class name##Counter : public Counter {                                                                             \
    public:                                                                                                            \
        name##Counter() : Counter(id) {}                                                                               \
        virtual u16 getMax() const override {                                                                          \
            return max;                                                                                                \
        }                                                                                                              \
    };                                                                                                                 \
    static name##Counter s##name##Counter;

MAKE_COMMON_COUNTER(PouchExpansion, 0x1EA, 7);

class HeartPieceCounter : public Counter {
public:
    HeartPieceCounter() : Counter(0x1E9) {}
    virtual s32 checkedAdd(s32 num) override;
    virtual u16 getMax() const override {
        return 3;
    }
};

static HeartPieceCounter sHeartPieceCounter;

s32 HeartPieceCounter::checkedAdd(s32 num) {
    if (num > 0) {
        s32 uncommitted = getUncommittedValue();
        s32 newCount = uncommitted + num;
        setValue(newCount % 4);
    }

    return 0;
}

MAKE_COMMON_COUNTER(HeartContainer, 0x1Fd, 24);

MAKE_COMMON_COUNTER(FaronGrasshopper, 0x1B9, 99);
MAKE_COMMON_COUNTER(WoodlandRhinoBeetle, 0x1BA, 99);
MAKE_COMMON_COUNTER(DekuHornet, 0x1BB, 99);
MAKE_COMMON_COUNTER(SkyloftMantis, 0x1BC, 99);
MAKE_COMMON_COUNTER(VolcanicLadybug, 0x1BD, 99);
MAKE_COMMON_COUNTER(BlessedButterfly, 0x1BE, 99);
MAKE_COMMON_COUNTER(LanayruAnt, 0x1BF, 99);
MAKE_COMMON_COUNTER(SandCicada, 0x1C0, 99);
MAKE_COMMON_COUNTER(GerudoDragonfly, 0x1C1, 99);
MAKE_COMMON_COUNTER(EldinRoller, 0x1C2, 99);
MAKE_COMMON_COUNTER(SkyStagBeetle, 0x1C3, 99);
MAKE_COMMON_COUNTER(StarryFirefly, 0x1C4, 99);

MAKE_COMMON_COUNTER(HornetLarvae, 0x1A5, 99);
MAKE_COMMON_COUNTER(BirdFeather, 0x1A6, 99);
MAKE_COMMON_COUNTER(Tumbleweed, 0x1A7, 99); // TIM!!
MAKE_COMMON_COUNTER(LizardTail, 0x1A8, 99);
MAKE_COMMON_COUNTER(EldinOre, 0x1A9, 99);
MAKE_COMMON_COUNTER(AncientFlower, 0x1AA, 99);
MAKE_COMMON_COUNTER(AmberRelic, 0x1AB, 99);
MAKE_COMMON_COUNTER(DuskRelic, 0x1AC, 99);
MAKE_COMMON_COUNTER(JellyBlob, 0x1AD, 99);
MAKE_COMMON_COUNTER(MonsterClaw, 0x1AE, 99);
MAKE_COMMON_COUNTER(MonsterHorn, 0x1AF, 99);
MAKE_COMMON_COUNTER(OrnamentalSkull, 0x1B0, 99);
MAKE_COMMON_COUNTER(EvilCrystal, 0x1B1, 99);
MAKE_COMMON_COUNTER(BlueBirdFeather, 0x1B2, 99);
MAKE_COMMON_COUNTER(GoldenSkull, 0x1B3, 99);
MAKE_COMMON_COUNTER(GoddessPlume, 0x1B4, 99);
MAKE_COMMON_COUNTER(GratitudeCrystal, 0x1B5, 127);

// I don't want to move all of these to headers yet
extern TumbleweedCounter ARROW_COUNTER;
extern TumbleweedCounter BOMB_COUNTER;
extern TumbleweedCounter EXTRA_WALLET_COUNTER;
extern TumbleweedCounter KEY_PIECE_COUNTER;
extern TumbleweedCounter RUPEE_COUNTER;
extern TumbleweedCounter SLINGSHOT_SEED_COUNTER;
extern TumbleweedCounter TEAR_COUNTER;

class Counters {
public:
    Counters() {
        // mCounters\[([0-9]+)\] = &([A-Za-z_]+);
        mCounters[0] = &RUPEE_COUNTER;
        mCounters[1] = &ARROW_COUNTER;
        mCounters[2] = &BOMB_COUNTER;
        mCounters[3] = &TEAR_COUNTER;
        mCounters[4] = &SLINGSHOT_SEED_COUNTER;
        mCounters[5] = &KEY_PIECE_COUNTER;

        mCounters[6] = &sSmallKeyCounter;
        mCounters[7] = &sPouchExpansionCounter;
        mCounters[8] = &sHeartPieceCounter;
        mCounters[9] = &sHeartContainerCounter;

        mCounters[10] = &sFaronGrasshopperCounter;
        mCounters[11] = &sWoodlandRhinoBeetleCounter;
        mCounters[12] = &sDekuHornetCounter;
        mCounters[13] = &sSkyloftMantisCounter;
        mCounters[14] = &sVolcanicLadybugCounter;
        mCounters[15] = &sBlessedButterflyCounter;
        mCounters[16] = &sLanayruAntCounter;
        mCounters[17] = &sSandCicadaCounter;
        mCounters[18] = &sGerudoDragonflyCounter;
        mCounters[19] = &sEldinRollerCounter;
        mCounters[20] = &sSkyStagBeetleCounter;
        mCounters[21] = &sStarryFireflyCounter;

        mCounters[22] = &sHornetLarvaeCounter;
        mCounters[23] = &sBirdFeatherCounter;
        mCounters[24] = &sTumbleweedCounter;
        mCounters[25] = &sLizardTailCounter;
        mCounters[26] = &sEldinOreCounter;
        mCounters[27] = &sAncientFlowerCounter;
        mCounters[28] = &sAmberRelicCounter;
        mCounters[29] = &sDuskRelicCounter;
        mCounters[30] = &sJellyBlobCounter;
        mCounters[31] = &sMonsterClawCounter;
        mCounters[32] = &sMonsterHornCounter;
        mCounters[33] = &sOrnamentalSkullCounter;
        mCounters[34] = &sEvilCrystalCounter;
        mCounters[35] = &sBlueBirdFeatherCounter;
        mCounters[36] = &sGoldenSkullCounter;
        mCounters[37] = &sGoddessPlumeCounter;
        mCounters[38] = &sGratitudeCrystalCounter;
        mCounters[39] = &EXTRA_WALLET_COUNTER;
    }

    Counter *mCounters[40];
};

Counters sCounters;

s32 increaseCounter(u16 counterIdx, s32 value) {
    return sCounters.mCounters[counterIdx]->checkedAdd(value);
}

s32 getCounterByIndex(u16 counterIdx) {
    return sCounters.mCounters[counterIdx]->getCommittedValue();
}

s32 getMaxItemCount(u16 counterIdx) {
    return sCounters.mCounters[counterIdx]->getMax();
}
