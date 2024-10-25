#ifndef D_FLAG_BASEFLAG_MANAGER_H
#define D_FLAG_BASEFLAG_MANAGER_H

#include "d/flag/unk_flag_stuff.h"
#include "d/flag/flag_space.h"

class ItemStoryManagerBase {
public:
    ItemStoryManagerBase();

    /** 0x08 */ virtual ~ItemStoryManagerBase();
    /** 0x0C */ virtual void setFlagszptr();
    /** 0x10 */ virtual void onDirty();
    /** 0x14 */ virtual void copyFlagsFromSave() = 0;
    /** 0x18 */ virtual void setupUnkFlagsStuff() = 0;
    /** 0x1C */ virtual void doCommit() = 0;
    /** 0x20 */ virtual void setFlag(u16 flag);
    /** 0x24 */ virtual void unsetFlag(u16 flag);
    /** 0x28 */ virtual void setFlagOrCounterToValue(u16 flag, u16 value);
    /** 0x2C */ virtual u16 getCounterOrFlag(u16 flag) const;
    /** 0x30 */ virtual u16 getUncommittedValue(u16 flag);
    /** 0x34 */ virtual u16 unk3(u16 arg);
    /** 0x38 */ virtual const u16 *getSaveFlagSpace() const = 0;

    
    void init();
    void setFlagSizes(u16 flagSizeMaybe, u16 anotherFlagSizeMaybe);
    void copyFlagsFromSave_Priv();
    void setupUnkFlagStuff(UnkFlagDefinition *def, u16 count);
    void doCommit_Priv();
    void setOrClearFlag(u16 flag, u16 value);
    u16 getFlag(u16 flag) const;
    void FUN_800bf610(u16 flag);
    u16 FUN_800bf640(u16 flag);
    void FUN_800bf690();

    u16 getUncommittedValue_Priv(u16 flag);

protected:
    /** 0x04 */ u16 flagSizeMaybe;
    /** 0x06 */ u16 anotherSizeMaybe;
    /** 0x08 */ FlagSpace *storyFlagsPtr;
    /** 0x0C */ UnkFlagStuff *unkFlagsPtr;
    /** 0x10 */ bool dirty;
};

#endif
