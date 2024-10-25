
#ifndef D_FLAG_ITEMFLAG_MANAGER_H
#define D_FLAG_ITEMFLAG_MANAGER_H

#include "d/flag/baseflag_manager.h"
#include "toBeSorted/file_manager.h"


class ItemflagManager : public ItemStoryManagerBase {
public:
    FlagSpace itemFlags;

    ItemflagManager();
    virtual ~ItemflagManager() {}

    bool commit();

    /** 0x0C */ virtual void setFlagszptr() override {
        storyFlagsPtr = &itemFlags;
    }
    /** 0x10 */ virtual void onDirty() override;
    /** 0x14 */ virtual void copyFlagsFromSave();
    /** 0x18 */ virtual void setupUnkFlagsStuff();
    /** 0x1C */ virtual void doCommit() {
        u16 sz = flagSizeMaybe;
        u16 *flags = storyFlagsPtr->getFlagPtrUnchecked();
        FileManager::sInstance->setItemFlags(flags, 0, sz);
    }
    /** 0x20 */ virtual void setFlag(u16 flag) {
        ItemStoryManagerBase::setFlag(flag & ~0x4000);
    }
    /** 0x24 */ virtual void unsetFlag(u16 flag) {
        ItemStoryManagerBase::unsetFlag(flag & ~0x4000);
    }
    /** 0x28 */ virtual void setFlagOrCounterToValue(u16 flag, u16 value) {
        ItemStoryManagerBase::setFlagOrCounterToValue(flag & ~0x4000, value);
    }
    /** 0x2C */ virtual u16 getCounterOrFlag(u16 flag) const {
        return ItemStoryManagerBase::getCounterOrFlag(flag & ~0x4000);
    }
    /** 0x30 */ virtual u16 getUncommittedValue(u16 flag) {
        return ItemStoryManagerBase::getUncommittedValue(flag & ~0x4000);
    }
    /** 0x34 */ virtual u16 unk3(u16 arg) {
        return ItemStoryManagerBase::unk3(arg & ~0x4000);
    }
    /** 0x38 */ virtual const u16 *getSaveFlagSpace() const {
        return FileManager::sInstance->getItemFlagsConst();
    };

    u16 getFlagDirect(u16 flag) {
        return ItemStoryManagerBase::getCounterOrFlag(flag);
    }

public:
    static ItemflagManager *sInstance;
    static u16 sFlags[0x40];
};

#endif
