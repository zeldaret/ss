#include <libc.h>
#include "toBeSorted/bitwise_flag_helper.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/flag_space.h"

class SkipflagManager {
public:
    bool mShouldCommit;
    BitwiseFlagHelper mFlagHelper;
    FlagSpace mFlagSpace;

    static u16 sSkipFlags[16];
    static SkipflagManager *sInstance;

    void copyFromSave();
    void setCommitFlag(u16 flag);
    SkipflagManager();
    void unsetCommitFlag();
    void thunk_copyFromSave();
    void setFlag(u16 flag);
    bool checkFlag(u16 flag);
    bool commitFlags();
};

// 0x80575408
SkipflagManager *SkipflagManager::sInstance = nullptr;
// 0x805A9C68
u16 SkipflagManager::sSkipFlags[16] = {};

// 800bfba0
void SkipflagManager::copyFromSave() {
    u16* savedSkipflags = FileManager::sInstance->getSkipFlags();
    mFlagSpace.copyFromSaveFile(savedSkipflags, 0, 0x10);
}

// 800bfbf0
void SkipflagManager::setCommitFlag(u16 flag) {
    mShouldCommit = true;
}

// 800bfc00
SkipflagManager::SkipflagManager() : mFlagSpace(sSkipFlags, ARRAY_LENGTH(sSkipFlags)), mShouldCommit(false) {
}

// 800bfc30
void SkipflagManager::unsetCommitFlag() {
    mShouldCommit = false;
}

// 800bfc40
void SkipflagManager::thunk_copyFromSave() {
    SkipflagManager::copyFromSave();
}

// 800bfc50
void SkipflagManager::setFlag(u16 flag) {
    mFlagHelper.setFlag(flag / 16, flag % 16, mFlagSpace.getFlagPtrChecked(), mFlagSpace.mCount);

    u16 savedSkipflags[0x10];
    checkedMemcpy(savedSkipflags, 0x20, FileManager::sInstance->getSkipFlags(), 0x20);

    mFlagHelper.setFlag(flag / 16, flag % 16, savedSkipflags, 0x10);
    FileManager::sInstance->setSkipFlagsChecked(savedSkipflags, 0, 0x10);
    setCommitFlag(flag);
}

// 800bfd20
bool SkipflagManager::checkFlag(u16 flag) {
    u16* savedSkipflags = FileManager::sInstance->getSkipFlags();
    return mFlagHelper.checkFlag(flag / 16, flag % 16, savedSkipflags, 0x10);
}

// 800bfd90
bool SkipflagManager::commitFlags() {
    if (mShouldCommit) {
        FileManager *instance = FileManager::sInstance;
        instance->setSkipFlagsChecked(mFlagSpace.getFlagPtrUnchecked(), 0, 0x10);
        mShouldCommit = false;
        return true;
    }
    return false;
}
