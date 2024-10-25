#include "d/flag/storyflag_manager.h"

u16 StoryflagManager::sFlags[0x80];
StoryflagManager *StoryflagManager::sInstance = nullptr;

StoryflagManager::StoryflagManager() : mStoryFlags(sFlags, 0x80) {}

void StoryflagManager::copyFlagsFromSave() {
    setFlagSizes(0x80, 0x100);
    ItemStoryManagerBase::copyFlagsFromSave_Priv();
}

extern "C" FlagDefinition StoryflagManager__STORYFLAG_DEFINITIONS[];

void StoryflagManager::setupUnkFlagsStuff() {
    setupFlagIndex(StoryflagManager__STORYFLAG_DEFINITIONS, 0x4B1);
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
