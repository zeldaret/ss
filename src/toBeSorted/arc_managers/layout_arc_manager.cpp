#include <d/d_heap.h>
#include <d/d_rawarchive.h>
#include <egg/core/eggHeap.h>
// clang-format off
#include <sized_string.h>
// clang-format on

class LayoutArcManager {
    LayoutArcManager();
    virtual ~LayoutArcManager();

    dRawArcTable_c mArcTable;

    static LayoutArcManager *sInstance;

    static bool create(EGG::Heap *heap);

    void init(EGG::Heap *heap);

    bool loadLayoutArcFromDisk(const char *object, EGG::Heap *heap);
    void ensureLoaded1(const char *object);
    void ensureLoaded2(const char *object);
    bool hasEntry(const char *object);
    bool decrement(const char *path);
    void *getData(const char *oarcName, const char *fileName);
    void *getLoadedData(const char *path);
};

LayoutArcManager *LayoutArcManager::sInstance;

LayoutArcManager::LayoutArcManager() {
    sInstance = this;
}

LayoutArcManager::~LayoutArcManager() {
    sInstance = nullptr;
}

extern char lbl_80575250;

void LayoutArcManager::init(EGG::Heap *heap) {
    mArcTable.init(16, &lbl_80575250, heap);
}

bool LayoutArcManager::loadLayoutArcFromDisk(const char *object, EGG::Heap *heap) {
    if (heap == nullptr) {
        heap = dHeap::layoutResHeap.heap;
    }
    return mArcTable.getArcOrLoadFromDisk(object, "Layout", 0, heap);
}

void LayoutArcManager::ensureLoaded1(const char *object) {
    mArcTable.ensureLoadedMaybe2(object);
}

void LayoutArcManager::ensureLoaded2(const char *object) {
    mArcTable.ensureLoadedMaybe(object);
}

bool LayoutArcManager::hasEntry(const char *object) {
    return mArcTable.hasEntry(object);
}

bool LayoutArcManager::decrement(const char *path) {
    return mArcTable.decreaseRefCount(path);
}

void *LayoutArcManager::getData(const char *oarcName, const char *fileName) {
    return mArcTable.getDataFromOarc(oarcName, fileName);
}

void *LayoutArcManager::getLoadedData(const char *path) {
    return mArcTable.getLoadedArcData(path);
}

bool LayoutArcManager::create(EGG::Heap *heap) {
    new (heap, 0x04) LayoutArcManager();

    if (LayoutArcManager::sInstance == nullptr) {
        return false;
    }
    LayoutArcManager::sInstance->init(heap);
    return true;
}
