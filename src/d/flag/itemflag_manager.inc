#include "d/flag/itemflag_manager.h"

u16 ItemflagManager::sFlags[0x40];

ItemflagManager *ItemflagManager::sInstance = nullptr;

ItemflagManager::ItemflagManager() : itemFlags(sFlags, 0x40) {}

void ItemflagManager::copyFlagsFromSave() {
    setFlagSizes(0x40, 0x80);
    copyFlagsFromSave_Priv();
}

extern "C" UnkFlagDefinition ItemflagManager__ITEMFLAG_DEFINITIONS[];

void ItemflagManager::setupUnkFlagsStuff() {
    setupUnkFlagStuff(ItemflagManager__ITEMFLAG_DEFINITIONS, 0x1FE);
}

void ItemflagManager::onDirty() {}

bool ItemflagManager::commit() {
    if (dirty) {
        doCommit_Priv();
        FUN_800bf690();
        return true;
    }
    return false;
}
