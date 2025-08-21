#include "toBeSorted/arc_managers/layout_arc_manager.h"

#include "d/d_heap.h"

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

dArcResult_e LayoutArcManager::ensureLoaded1(const char *object) {
    return mArcTable.ensureLoadedMaybe2(object);
}

dArcResult_e LayoutArcManager::ensureLoaded2(const char *object) {
    return mArcTable.ensureLoadedMaybe(object);
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

    if (GetInstance() == nullptr) {
        return false;
    }
    GetInstance()->init(heap);
    return true;
}

LayoutArcControl::~LayoutArcControl() {
    if (mLayoutArcs != nullptr) {
        release();
    }
}

void LayoutArcControl::set(const char *const *layoutArcs, s32 numArcs) {
    mLayoutArcs = layoutArcs;
    mNumArcs = numArcs;
}

bool LayoutArcControl::load(EGG::Heap *heap) const {
    if (mLayoutArcs == nullptr) {
        return true;
    }

    const char *const *ptr = mLayoutArcs;
    for (int i = 0; i < mNumArcs; i++) {
        LayoutArcManager::GetInstance()->loadLayoutArcFromDisk(*ptr, heap);
        ptr++;
    }

    return true;
}

bool LayoutArcControl::allLoaded() const {
    if (mLayoutArcs == nullptr) {
        return true;
    }

    const char *const *ptr = mLayoutArcs;
    for (int i = 0; i < mNumArcs; i++) {
        dArcResult_e res = LayoutArcManager::GetInstance()->ensureLoaded1(*ptr);
        if (res != D_ARC_RESULT_OK) {
            return false;
        }
        ptr++;
    }
    return true;
}

bool LayoutArcControl::release() {
    if (mLayoutArcs == nullptr) {
        return true;
    }

    const char *const *ptr = mLayoutArcs;
    for (int i = 0; i < mNumArcs; i++) {
        dArcResult_e res = LayoutArcManager::GetInstance()->ensureLoaded2(*ptr);
        if (res != D_ARC_RESULT_ERROR_NOT_FOUND && res != D_ARC_RESULT_OK) {
            return false;
        }
        ptr++;
    }

    ptr = mLayoutArcs;
    for (int i = 0; i < mNumArcs; i++) {
        LayoutArcManager::GetInstance()->decrement(*ptr);
        ptr++;
    }
    mLayoutArcs = nullptr;

    return true;
}
