#include "d/flag/dungeonflag_manager.h"
#include "d/flag/flag_index.h"

// TODO move
extern "C" FlagDefinition DungeonflagManager__FLAG_DEFINITIONS[];

DungeonflagManager *DungeonflagManager::sInstance = nullptr;

u16 DungeonflagManager::sDungeonFlags[8] = {};

void DungeonflagManager::copyFromSave_Internal(u16 flagIndex) {
    mFlagSpace.copyFromSaveFile(saveFilePtr(flagIndex), 0, 0x8);
}

void DungeonflagManager::setCommitFlag(u16 flag) {
    mShouldCommit = true;
}

DungeonflagManager::DungeonflagManager()
    : mShouldCommit(false),
      mStageIndex(-1),
      mpFlagIndex(nullptr),
      mFlagSpace(sDungeonFlags, ARRAY_LENGTH(sDungeonFlags)) {}

void DungeonflagManager::init() {
    mpFlagIndex = new FlagIndex(0x11, DungeonflagManager__FLAG_DEFINITIONS);
    mShouldCommit = false;
}

void DungeonflagManager::copyFromSave(u32 flag) {
    mStageIndex = flag;
    copyFromSave_Internal(flag);
}

void DungeonflagManager::setToValue(u16 flag, u32 value) {
    u16 *data = mFlagSpace.getFlagPtrChecked();
    mpFlagIndex->setCounterOrFlag(flag, data, 8, value);
    setCommitFlag(flag);
}

void DungeonflagManager::setFlag(u16 flag) {
    u16 *data = mFlagSpace.getFlagPtrChecked();
    mpFlagIndex->setCounterOrFlag(flag, data, 8, 1);
    setCommitFlag(flag);
}

u16 DungeonflagManager::getDungeonFlag(u16 flag) {
    u16 *data = mFlagSpace.getFlagPtrUnchecked();
    return mpFlagIndex->getCounterOrFlag(flag, data, 8);
}

bool DungeonflagManager::commit() {
    FileManager *instance;
    u16 idx = mStageIndex;
    if (idx == 0xFFFF) {
        return false;
    } else if (mShouldCommit) {
        instance = FileManager::GetInstance();
        u16 *ptr = mFlagSpace.getFlagPtrUnchecked();
        instance->setDungeonFlags(ptr, (idx & 0x1fff) * 8, 0x08);
        mShouldCommit = false;
        return true;
    }
    return false;
}
