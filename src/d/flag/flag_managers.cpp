
#include "d/flag/flag_managers.h"

// clang-format off
#include "common.h"
#include "egg/core/eggHeap.h"
#include "m/m_heap.h"
// vtable order
#include "d/flag/flag_space.h"
#include "d/flag/tboxflag_manager.h"
#include "d/flag/enemyflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/flag/itemflag_manager.h"

// actual managers
#include "d/flag/sceneflag_manager.inc"
#include "d/flag/committable_flag_manager.inc"
#include "d/flag/tboxflag_manager.inc"
#include "d/flag/enemyflag_manager.inc"
#include "d/flag/flag_space.inc"
#include "d/flag/flag_index.inc"
#include "d/flag/bitwise_flag_helper.inc"
#include "d/flag/baseflag_manager.inc"
#include "d/flag/storyflag_manager.inc"
#include "d/flag/itemflag_manager.inc"
#include "d/flag/dungeonflag_manager.inc"
#include "d/flag/skipflag_manager.inc"
// clang-format on


static void postSetup();

template <typename T>
class MyFlagManager : public T {
public:
    MyFlagManager<T>() {}
    ~MyFlagManager<T>() {}
};

void setupFlagManagers(EGG::Heap *heap) {
    if (SceneflagManager::sInstance == nullptr) {
        SceneflagManager::sInstance = new (heap) MyFlagManager<SceneflagManager>();
        mHeap m(heap);
        SceneflagManager::sInstance->init();
    }

    if (TBoxflagManager::sInstance == nullptr) {
        TBoxflagManager::sInstance = new (heap) MyFlagManager<TBoxflagManager>();
        mHeap m(heap);
        TBoxflagManager::sInstance->init();
    }

    if (EnemyflagManager::sInstance == nullptr) {
        EnemyflagManager::sInstance = new (heap) MyFlagManager<EnemyflagManager>();
        mHeap m(heap);
        EnemyflagManager::sInstance->init();
    }

    if (StoryflagManager::sInstance == nullptr) {
        StoryflagManager::sInstance = new (heap) MyFlagManager<StoryflagManager>();
        mHeap m(heap);
        StoryflagManager::sInstance->init();
    }

    if (ItemflagManager::sInstance == nullptr) {
        ItemflagManager::sInstance = new (heap) MyFlagManager<ItemflagManager>();
        mHeap m(heap);
        ItemflagManager::sInstance->init();
    }

    if (DungeonflagManager::sInstance == nullptr) {
        DungeonflagManager::sInstance = new (heap) MyFlagManager<DungeonflagManager>();
        mHeap m(heap);
        DungeonflagManager::sInstance->init();
    }

    if (SkipflagManager::sInstance == nullptr) {
        SkipflagManager::sInstance = new (heap) MyFlagManager<SkipflagManager>();
        mHeap m(heap);
        SkipflagManager::sInstance->init();
    }

    postSetup();
}

static void postSetup() {
    updateFlagForFlagIndex(0);
}

void copyAllFlagManagersFromSave() {
    u16 flag = FileManager::sInstance->getSceneFlagIndex();
    SceneflagManager::sInstance->copyFromSave(flag);
    TBoxflagManager::sInstance->copyFromSave(flag);
    StoryflagManager::sInstance->copyFromSave();
    ItemflagManager::sInstance->copyFromSave();
    DungeonflagManager::sInstance->copyFromSave(flag);
    SkipflagManager::sInstance->copyFromSave();
    EnemyflagManager::sInstance->copyFromSave(flag);
}

void updateFlagForFlagIndex(u16 stage) {
    SceneflagManager::sInstance->updateFlagindex(stage);
    TBoxflagManager::sInstance->copyFromSave(stage);
    EnemyflagManager::sInstance->updateFlagIndex(stage);
    DungeonflagManager::sInstance->copyFromSave(stage);
}

void commitAllFlagManagers() {
    StoryflagManager::sInstance->commit();
    ItemflagManager::sInstance->commit();
    DungeonflagManager::sInstance->commit();
    SkipflagManager::sInstance->commit();
    SceneflagManager::sInstance->commit();
    TBoxflagManager::sInstance->commit();
    EnemyflagManager::sInstance->commit();
}

bool checkedMemcpy(void *dest, u32 destLen, const void *src, u32 count) {
    if (dest == nullptr || src == nullptr || destLen < count || destLen > 0xFFFF) {
        return true;
    } else {
        memcpy(dest, src, count);
        return false;
    }
}
