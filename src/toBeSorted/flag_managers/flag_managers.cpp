// clang-format off

#include <toBeSorted/flag_space.h>
#include <toBeSorted/misc_flag_managers.h>
#include <toBeSorted/item_story_flag_manager.h>

#include <toBeSorted/flag_managers/sceneflag_manager.inc>
#include <toBeSorted/item_story_flag_manager.h>
#include <toBeSorted/flag_managers/misc_flag_managers.inc>
#include <toBeSorted/flag_managers/flag_space.inc>
#include <toBeSorted/flag_managers/unk_flag_stuff.inc>
#include <toBeSorted/flag_managers/bitwise_flag_helper.inc>


ItemStoryManagerBase::ItemStoryManagerBase(): flagSizeMaybe(0), anotherSizeMaybe(0), storyFlagsPtr(nullptr), unkFlagsPtr(nullptr), dirty(false) {}
ItemStoryManagerBase::~ItemStoryManagerBase() {
    if (unkFlagsPtr != nullptr) {
        delete unkFlagsPtr;
    }
}

void ItemStoryManagerBase::setFlagSizes(u16 flagSizeMaybe, u16 anotherFlagSizeMaybe) {
    this->flagSizeMaybe = flagSizeMaybe;
    this->anotherSizeMaybe = anotherFlagSizeMaybe;
}

/** 800bf320 */
void ItemStoryManagerBase::setupUnkFlagStuff(UnkFlagDefinition *def, u16 count) {
    UnkFlagStuff *stuff = new UnkFlagStuff(count, def);
    this->unkFlagsPtr = stuff;
}

/** 800bf380 */
void ItemStoryManagerBase::copyFlagsFromSave_Priv() {
    FlagSpace *current = this->storyFlagsPtr;
    const u16 *saved = this->getSaveFlagSpace();
    current->copyFromSaveFile(saved, 0, this->flagSizeMaybe);
}

/** 800bf3e0 */
void ItemStoryManagerBase::init() {
    const u16 *space = this->getSaveFlagSpace();
    if (space == nullptr || this->unkFlagsPtr == nullptr) {
        this->setFlagszptr();
        this->copyFlagsFromSave();
        this->setupUnkFlagsStuff();
    }
}

/** 800bf470 */
void ItemStoryManagerBase::setFlagszptr() {

}

/** 800bf480 */
u16 ItemStoryManagerBase::getCounterOrFlag(u16 flag) const {
    const u16 *data = this->getSaveFlagSpace();
    return this->unkFlagsPtr->getCounterOrFlag(flag, data, this->flagSizeMaybe);
}

/** 800bf4e0 */
u16 ItemStoryManagerBase::getUncommittedValue_Priv(u16 flag) {
    u16 *data = this->storyFlagsPtr->getFlagPtrUnchecked();
    return this->unkFlagsPtr->getCounterOrFlag(flag, data, this->flagSizeMaybe);
}

/** 800bf530 */
void ItemStoryManagerBase::setOrClearFlag(u16 flag, u16 value) {
    FlagSpace *storyFlagsPtr = this->storyFlagsPtr;
    u16 *pData = storyFlagsPtr->getFlagPtrChecked();
    this->unkFlagsPtr->setCounterOrFlag(flag, pData, storyFlagsPtr->mCount, value);
    this->FUN_800bf610(flag);
}

/** 800bf5b0 */
void ItemStoryManagerBase::setFlag(u16 flag) {
    this->setOrClearFlag(flag, true);
}

/** 800bf5c0 */
void ItemStoryManagerBase::unsetFlag(u16 flag) {
    this->setOrClearFlag(flag, false);
}

/** 800bf5d0 */
void ItemStoryManagerBase::thunk_setOrClearFlag(u16 flag, u16 value) {
    this->setOrClearFlag(flag, value);
}

/** 800bf5e0 */
u16 ItemStoryManagerBase::getFlag(u16 flag) const {
    return ItemStoryManagerBase::getCounterOrFlag(flag);
}

/** 800bf5f0 */
u16 ItemStoryManagerBase::getUncommittedValue(u16 flag) {
    return this->getUncommittedValue_Priv(flag);
}

/** 800bf600 */
u16 ItemStoryManagerBase::FUN_800bf600(u16 flag) {
    return this->FUN_800bf640(flag);
}

/** 800bf610 */
void ItemStoryManagerBase::FUN_800bf610(u16 flag) {
    this->dirty = true;
    this->onDirty();
}

/** 800bf630 */
void ItemStoryManagerBase::onDirty() {

}

/** 800bf640 */
u16 ItemStoryManagerBase::FUN_800bf640(u16 flag) {
    return this->unkFlagsPtr->maskForIdx(flag, this->flagSizeMaybe);
}

/** 800bf650 */
void ItemStoryManagerBase::doCommit_Priv() {
    this->doCommit();
    this->dirty = false;
}

/* 800bf690 */
void ItemStoryManagerBase::FUN_800bf690() {

}

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

ItemflagManager::ItemflagManager() : itemFlags(sFlags, 0x40) {}

void ItemflagManager::copyFlagsFromSave() {
    setFlagSizes(0x40, 0x80);
    copyFlagsFromSave_Priv();
}

extern "C" UnkFlagDefinition ItemflagManager__ITEMFLAG_DEFINITIONS[];

void ItemflagManager::setupUnkFlagsStuff() {
    setupUnkFlagStuff(ItemflagManager__ITEMFLAG_DEFINITIONS, 0x1FE);
}

void ItemflagManager::onDirty() {

}

bool ItemflagManager::commit() {
    if (dirty) {
        doCommit_Priv();
        FUN_800bf690();
        return true;
    }
    return false;
}

#include <toBeSorted/flag_managers/dungeonflag_manager.inc>
#include <toBeSorted/flag_managers/skipflag_manager.inc>
// clang-format on

#include <egg/core/eggHeap.h>
#include <m/m_heap.h>

static void postSetup();
static void updateFlagForFlagIndex(u16 stage);
class TBoxFlagManagerSub : public TBoxFlagManager {
    // virtual ~TBoxFlagManagerSub() {}
};

class EnemyDefeatManagerSub : public EnemyDefeatManager {
    // virtual ~EnemyDefeatManagerSub() {}
};


class StoryflagManagerSub : public StoryflagManager {
    // virtual ~StoryflagManagerSub() {}
};

class ItemflagManagerSub : public ItemflagManager {
    // virtual ~ItemflagManagerSub() {}
};




void setupFlagManagers(EGG::Heap *heap) {
    if (SceneflagManager::sInstance == nullptr) {
        SceneflagManager::sInstance = new (heap) SceneflagManager();
        mHeap m(heap);
        SceneflagManager::sInstance->doNothing();
    }

    if (TBoxFlagManager::sInstance == nullptr) {
        TBoxFlagManager::sInstance = new (heap) TBoxFlagManagerSub();
        mHeap m(heap);
        TBoxFlagManager::sInstance->init();
    }

    if (EnemyDefeatManager::sInstance == nullptr) {
        EnemyDefeatManager::sInstance = new (heap) EnemyDefeatManagerSub();
        mHeap m(heap);
        EnemyDefeatManager::sInstance->init();
    }

    if (StoryflagManager::sInstance == nullptr) {
        StoryflagManager::sInstance = new (heap) StoryflagManagerSub();
        mHeap m(heap);
        StoryflagManager::sInstance->init();
    }

    if (ItemflagManager::sInstance == nullptr) {
        ItemflagManager::sInstance = new (heap) ItemflagManagerSub();
        mHeap m(heap);
        ItemflagManager::sInstance->init();
    }

    if (DungeonflagManager::sInstance == nullptr) {
        DungeonflagManager::sInstance = new (heap) DungeonflagManager();
        mHeap m(heap);
        DungeonflagManager::sInstance->setupFlagStuff();
    }

    if (SkipflagManager::sInstance == nullptr) {
        SkipflagManager::sInstance = new (heap) SkipflagManager();
        mHeap m(heap);
        SkipflagManager::sInstance->unsetCommitFlag();
    }

    postSetup();
}

static void postSetup() {
    updateFlagForFlagIndex(0);
}

void copyAllFlagManagersFromSave() {
    u16 flag = FileManager::sInstance->getSceneFlagIndex();
    SceneflagManager::sInstance->copyFromSave(flag);
    TBoxFlagManager::sInstance->copyFromSave(flag);
    StoryflagManager::sInstance->copyFlagsFromSave_Priv();
    ItemflagManager::sInstance->copyFlagsFromSave_Priv();
    DungeonflagManager::sInstance->copyFromSave(flag);
    SkipflagManager::sInstance->copyFromSave();
    EnemyDefeatManager::sInstance->copyFromSave(flag);
}

static void updateFlagForFlagIndex(u16 stage) {}

extern "C" bool checkedMemcpy(void *dest, u32 destLen, const void *src, u32 count) {
    if (dest == nullptr || src == nullptr || destLen < count || destLen > 0xFFFF) {
        return true;
    } else {
        memcpy(dest, src, count);
        return false;
    }
}
