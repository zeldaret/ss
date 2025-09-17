#include "common.h"
#include "d/flag/bitwise_flag_helper.h"
#include "d/flag/flag_managers.h"
#include "d/flag/flag_space.h"
#include "d/flag/skipflag_manager.h"
#include "toBeSorted/file_manager.h"

SkipflagManager *SkipflagManager::sInstance = nullptr;

u16 SkipflagManager::sSkipFlags[16] = {};

void SkipflagManager::doCopyFromSave() {
    u16 *savedSkipflags = FileManager::GetInstance()->getSkipFlags();
    mFlagSpace.copyFromSaveFile(savedSkipflags, 0, 0x10);
}

void SkipflagManager::setCommitFlag(u16 flag) {
    mShouldCommit = true;
}

SkipflagManager::SkipflagManager() : mFlagSpace(sSkipFlags, ARRAY_LENGTH(sSkipFlags)), mShouldCommit(false) {}

void SkipflagManager::init() {
    mShouldCommit = false;
}

void SkipflagManager::copyFromSave() {
    doCopyFromSave();
}

void SkipflagManager::setFlag(u16 flag) {
    mFlagHelper.setFlag(flag / 16, flag % 16, mFlagSpace.getFlagPtrChecked(), mFlagSpace.mCount);

    u16 savedSkipflags[0x10];
    checkedMemcpy(savedSkipflags, 0x20, FileManager::GetInstance()->getSkipFlags(), 0x20);

    mFlagHelper.setFlag(flag / 16, flag % 16, savedSkipflags, 0x10);
    FileManager::GetInstance()->setSkipFlagsChecked(savedSkipflags, 0, 0x10);
    setCommitFlag(flag);
}

bool SkipflagManager::checkFlag(u16 flag) {
    u16 *savedSkipflags = FileManager::GetInstance()->getSkipFlags();
    return mFlagHelper.checkFlag(flag / 16, flag % 16, savedSkipflags, 0x10);
}

bool SkipflagManager::commit() {
    if (mShouldCommit) {
        FileManager *instance = FileManager::GetInstance();
        instance->setSkipFlagsChecked(mFlagSpace.getFlagPtrUnchecked(), 0, 0x10);
        mShouldCommit = false;
        return true;
    }
    return false;
}
