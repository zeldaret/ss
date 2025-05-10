#ifndef D_FLAG_BASEFLAG_MANAGER_H
#define D_FLAG_BASEFLAG_MANAGER_H

#include "d/flag/flag_index.h"
#include "d/flag/flag_space.h"

class ItemStoryManagerBase {
public:
    ItemStoryManagerBase();

    /** 0x08 */ virtual ~ItemStoryManagerBase();
    /** 0x0C */ virtual void initFlagSpace();
    /** 0x10 */ virtual void onDirty();
    /** 0x14 */ virtual void copyFlagsFromSaveFirstTime() = 0;
    /** 0x18 */ virtual void setupFlagIndex() = 0;
    /** 0x1C */ virtual void doCommit() = 0;
    /** 0x20 */ virtual void setFlag(u16 flag);
    /** 0x24 */ virtual void unsetFlag(u16 flag);
    /** 0x28 */ virtual void setFlagOrCounterToValue(u16 flag, u16 value);
    /** 0x2C */ virtual u16 getCounterOrFlag(u16 flag) const;
    /** 0x30 */ virtual u16 getUncommittedValue(u16 flag) const;
    /** 0x34 */ virtual u16 unk3(u16 arg);
    /** 0x38 */ virtual const u16 *getSaveFlagSpace() const = 0;

    
    void init();
    void copyFromSave();
    void createFlagIndex(FlagDefinition *def, u16 count);
    void doCommit_Priv();
    void setOrClearFlag(u16 flag, u16 value);
    u16 getFlag(u16 flag) const;
    void onFlagChange(u16 flag);
    u16 getMaskForFlag(u16 flag);
    void postCommit();

    u16 getUncommittedValue_Priv(u16 flag) const;

protected:

    void setFlagSizes(u16 flagCount, u16 flagSizeBytes);

    /** 0x04 */ u16 mFlagCount;
    /** 0x06 */ u16 mFlagSizeBytes;
    /** 0x08 */ FlagSpace *mpFlagSpace;
    /** 0x0C */ FlagIndex *mpFlagIndex;
    /** 0x10 */ bool mDirty;
};

#endif
