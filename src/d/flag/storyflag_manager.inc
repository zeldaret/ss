#include "d/flag/storyflag_manager.h"

u16 StoryflagManager::sFlags[0x80];
StoryflagManager *StoryflagManager::sInstance = nullptr;

StoryflagManager::StoryflagManager() : storyFlags(sFlags, 0x80) {

}

void StoryflagManager::copyFlagsFromSave() {
    setFlagSizes(0x80, 0x100);
    copyFlagsFromSave_Priv();
}

extern "C" UnkFlagDefinition StoryflagManager__STORYFLAG_DEFINITIONS[];

void StoryflagManager::setupUnkFlagsStuff() {
    setupUnkFlagStuff(StoryflagManager__STORYFLAG_DEFINITIONS, 0x4B1);
}

void StoryflagManager::onDirty() {

}

bool StoryflagManager::commit() {
    if (dirty) {
        doCommit_Priv();
        FUN_800bf690();
        return true;
    }
    return false;
}

void StoryflagManager::unsetFlag(u16 flag) {
    ItemStoryManagerBase::unsetFlag(flag);
}
