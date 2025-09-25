#ifndef D_FLAG_STORYFLAG_MANAGER_H
#define D_FLAG_STORYFLAG_MANAGER_H

#include "d/flag/baseflag_manager.h"
#include "d/flag/storyflag_map.h" // IWYU pragma: export
#include "toBeSorted/file_manager.h"

class StoryflagManager : public ItemStoryManagerBase {
public:
    StoryflagManager();

    virtual ~StoryflagManager() {}

    FlagSpace mStoryFlags;

    bool commit();

    /** 0x0C */ virtual void initFlagSpace() override {
        mpFlagSpace = &mStoryFlags;
    }
    /** 0x10 */ virtual void onDirty() override;
    /** 0x14 */ virtual void copyFlagsFromSaveFirstTime() override;
    /** 0x18 */ virtual void setupFlagIndex() override;
    /** 0x1C */ virtual void doCommit() override {
        u16 sz = mFlagCount;
        u16 *flags = mpFlagSpace->getFlagPtrUnchecked();
        FileManager::GetInstance()->setStoryFlags(flags, 0, sz);
    }
    /** 0x24 */ virtual void unsetFlag(u16 flag) override;
    /** 0x38 */ virtual const u16 *getSaveFlagSpace() const override {
        return FileManager::GetInstance()->getStoryFlagsConst();
    };

    u16 getFlag(u16 flag) const {
        return getCounterOrFlag(flag);
    }

    static StoryflagManager *sInstance;

private:
    static u16 sFlags[0x80];
};

#endif
