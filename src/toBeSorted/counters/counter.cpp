#include <toBeSorted/counters/counter.h>

// TODO use the item flag manager once it exists
class ItemFlagManager {
public:
    ItemFlagManager() {}
    /** 0x08 */ virtual ~ItemFlagManager();
    /** 0x0C */ virtual void setFlagszptr();
    /** 0x10 */ virtual void onDirty();
    /** 0x14 */ virtual void copyFlagsFromSave() = 0;
    /** 0x18 */ virtual void setupUnkFlagsStuff() = 0;
    /** 0x1C */ virtual bool doCommit() = 0;
    /** 0x20 */ virtual void setFlag(u16 flag);
    /** 0x24 */ virtual void unsetFlag(u16 flag);
    /** 0x28 */ virtual void setFlagOrCounterToValue(u16 flag, u16 value);
    /** 0x2C */ virtual  u16 getCounterOrFlag(u16 flag);
    /** 0x30 */ virtual  u16 getUncommittedValue(u16 flag);
    /** 0x34 */ virtual void unk3();
    /** 0x38 */ virtual  u16 *getSaveFlagSpace() = 0;
};

// TODO set up item flag manager
extern "C" ItemFlagManager *lbl_80575400;


/* 8016cc40 */ s32 Counter::checkedAdd(s32 num) {
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

/* 8016cd30 */ u16 Counter::getCommittedValue() {
    return lbl_80575400->getCounterOrFlag(counterId | 0x4000);
}

/* 8016cd50 */ u16 Counter::getUncommittedValue() {
    return lbl_80575400->getUncommittedValue(counterId | 0x4000);
}

/* 8016cd70 */ void Counter::setValue(u16 num) {
    lbl_80575400->setFlagOrCounterToValue(counterId | 0x4000, num);
}
