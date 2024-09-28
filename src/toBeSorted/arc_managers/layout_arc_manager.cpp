#include <d/d_heap.h>
#include <toBeSorted/arc_managers/layout_arc_manager.h>

LayoutArcManager *LayoutArcManager::sInstance;

LayoutArcManager::LayoutArcManager() {
    sInstance = this;
}

LayoutArcManager::~LayoutArcManager() {
    sInstance = nullptr;
}

void LayoutArcManager::init(EGG::Heap *heap) {
    mArcTable.init(16, &ArcCallbackHandler::sInstance, heap);
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
