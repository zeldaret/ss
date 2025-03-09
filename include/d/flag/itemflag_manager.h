
#ifndef D_FLAG_ITEMFLAG_MANAGER_H
#define D_FLAG_ITEMFLAG_MANAGER_H

#include "d/flag/baseflag_manager.h"
#include "toBeSorted/file_manager.h"

class ItemflagManager : public ItemStoryManagerBase {
private:
    FlagSpace mItemFlags;

public:
    ItemflagManager();
    virtual ~ItemflagManager() {}

    bool commit();

    /** 0x0C */ virtual void initFlagSpace() override {
        mpFlagSpace = &mItemFlags;
    }
    /** 0x10 */ virtual void onDirty() override;
    /** 0x14 */ virtual void copyFlagsFromSaveFirstTime() override;
    /** 0x18 */ virtual void setupFlagIndex() override;
    /** 0x1C */ virtual void doCommit() override {
        u16 sz = mFlagCount;
        u16 *flags = mpFlagSpace->getFlagPtrUnchecked();
        FileManager::GetInstance()->setItemFlags(flags, 0, sz);
    }
    /** 0x20 */ virtual void setFlag(u16 flag) override {
        ItemStoryManagerBase::setFlag(flag & ~0x4000);
    }
    /** 0x24 */ virtual void unsetFlag(u16 flag) override {
        ItemStoryManagerBase::unsetFlag(flag & ~0x4000);
    }
    /** 0x28 */ virtual void setFlagOrCounterToValue(u16 flag, u16 value) override {
        ItemStoryManagerBase::setFlagOrCounterToValue(flag & ~0x4000, value);
    }
    /** 0x2C */ virtual u16 getCounterOrFlag(u16 flag) const override {
        return ItemStoryManagerBase::getCounterOrFlag(flag & ~0x4000);
    }
    /** 0x30 */ virtual u16 getUncommittedValue(u16 flag) override {
        return ItemStoryManagerBase::getUncommittedValue(flag & ~0x4000);
    }
    /** 0x34 */ virtual u16 unk3(u16 arg) override {
        return ItemStoryManagerBase::unk3(arg & ~0x4000);
    }
    /** 0x38 */ virtual const u16 *getSaveFlagSpace() const override {
        return FileManager::GetInstance()->getItemFlagsConst();
    };

    u16 getFlagDirect(u16 flag) {
        return ItemStoryManagerBase::getCounterOrFlag(flag);
    }

    static ItemflagManager *sInstance;

private:
    static u16 sFlags[0x40];
};

#endif
