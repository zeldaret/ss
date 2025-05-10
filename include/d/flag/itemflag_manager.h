
#ifndef D_FLAG_ITEMFLAG_MANAGER_H
#define D_FLAG_ITEMFLAG_MANAGER_H

#include "d/flag/baseflag_manager.h"
#include "toBeSorted/file_manager.h"

class ItemflagManager : public ItemStoryManagerBase {
private:
    FlagSpace mItemFlags;

    static const u32 MASK = 0x4000;

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
        ItemStoryManagerBase::setFlag(flag & ~MASK);
    }
    /** 0x24 */ virtual void unsetFlag(u16 flag) override {
        ItemStoryManagerBase::unsetFlag(flag & ~MASK);
    }
    /** 0x28 */ virtual void setFlagOrCounterToValue(u16 flag, u16 value) override {
        ItemStoryManagerBase::setFlagOrCounterToValue(flag & ~MASK, value);
    }
    /** 0x2C */ virtual u16 getCounterOrFlag(u16 flag) const override {
        return ItemStoryManagerBase::getCounterOrFlag(flag & ~MASK);
    }
    /** 0x30 */ virtual u16 getUncommittedValue(u16 flag) const override {
        return ItemStoryManagerBase::getUncommittedValue(flag & ~MASK);
    }
    /** 0x34 */ virtual u16 unk3(u16 arg) override {
        return ItemStoryManagerBase::unk3(arg & ~MASK);
    }
    /** 0x38 */ virtual const u16 *getSaveFlagSpace() const override {
        return FileManager::GetInstance()->getItemFlagsConst();
    };

    u16 getFlagDirect(u16 flag) {
        return ItemStoryManagerBase::getCounterOrFlag(flag);
    }

    static ItemflagManager *sInstance;

    void setItemflag(u16 flag) {
        setFlag(flag | MASK);
    }

    u16 getItemCounterOrFlag(u16 flag) const {
        return getCounterOrFlag(flag | MASK);
    }

    void setItemFlag(u16 flag) {
        setFlag(flag | MASK);
    }

    void setItemFlagOrCounterToValue(u16 flag, u16 value) {
        setFlagOrCounterToValue(flag | MASK, value);
    }

    u16 getUncommittedItemValue(u16 flag) const {
        return getUncommittedValue(flag | MASK);
    }

private:
    static u16 sFlags[0x40];
};

#endif
