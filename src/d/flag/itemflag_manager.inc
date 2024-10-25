#include "d/flag/itemflag_manager.h"

u16 ItemflagManager::sFlags[0x40];

ItemflagManager *ItemflagManager::sInstance = nullptr;

ItemflagManager::ItemflagManager() : mItemFlags(sFlags, 0x40) {}

void ItemflagManager::copyFlagsFromSave() {
    setFlagSizes(0x40, 0x80);
    ItemStoryManagerBase::copyFlagsFromSave_Priv();
}

extern "C" FlagDefinition ItemflagManager__ITEMFLAG_DEFINITIONS[];

void ItemflagManager::setupUnkFlagsStuff() {
    setupFlagIndex(ItemflagManager__ITEMFLAG_DEFINITIONS, 0x1FE);
}

void ItemflagManager::onDirty() {}

bool ItemflagManager::commit() {
    if (mDirty) {
        doCommit_Priv();
        postCommit();
        return true;
    }
    return false;
}
