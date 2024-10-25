#ifndef ITEM_STORY_FLAG_MANAGER_H
#define ITEM_STORY_FLAG_MANAGER_H

#include "toBeSorted/flag_space.h"
#include "toBeSorted/unk_flag_stuff.h"
#include "toBeSorted/file_manager.h"

// TODO These classes have an interesting relation and there are like 5 vtables, so
// the stuff in this header should not be relied upon for actually implementing these,
// but we need the interface

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

class StoryflagManager : public ItemStoryManagerBase {
public:
    StoryflagManager();

    virtual ~StoryflagManager() {}

    FlagSpace storyFlags;

    bool commit();

    /** 0x0C */ virtual void setFlagszptr() override {
        storyFlagsPtr = &storyFlags;
    }
    /** 0x10 */ virtual void onDirty() override;
    /** 0x14 */ virtual void copyFlagsFromSave() override;
    /** 0x18 */ virtual void setupUnkFlagsStuff() override;
    /** 0x1C */ virtual void doCommit() override {
        u16 sz = flagSizeMaybe;
        u16 *flags = storyFlagsPtr->getFlagPtrUnchecked();
        FileManager::sInstance->setStoryFlags(flags, 0, sz);
    }
    /** 0x24 */ virtual void unsetFlag(u16 flag) override;
    /** 0x38 */ virtual const u16 *getSaveFlagSpace() const override {
        return FileManager::sInstance->getStoryFlagsConst();
    };

public:
    static StoryflagManager *sInstance;
    static u16 sFlags[0x80];
};


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
