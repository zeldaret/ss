#include "d/flag/storyflag_manager.h"

u16 StoryflagManager::sFlags[0x80];
StoryflagManager *StoryflagManager::sInstance = nullptr;

StoryflagManager::StoryflagManager() : mStoryFlags(sFlags, 0x80) {}

void StoryflagManager::copyFlagsFromSaveFirstTime() {
    setFlagSizes(0x80, 0x100);
    ItemStoryManagerBase::copyFromSave();
}

extern "C" FlagDefinition StoryflagManager__STORYFLAG_DEFINITIONS[];

void StoryflagManager::setupFlagIndex() {
    createFlagIndex(StoryflagManager__STORYFLAG_DEFINITIONS, STORYFLAG_MAX);
}

void StoryflagManager::onDirty() {}

bool StoryflagManager::commit() {
    if (mDirty) {
        doCommit_Priv();
        postCommit();
        return true;
    }
    return false;
}

void StoryflagManager::unsetFlag(u16 flag) {
    ItemStoryManagerBase::unsetFlag(flag);
}
