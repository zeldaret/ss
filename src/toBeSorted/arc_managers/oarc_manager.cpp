#include "toBeSorted/arc_managers/oarc_manager.h"

#include "d/d_heap.h"

OarcManager *OarcManager::sInstance;

OarcManager::OarcManager() {
    sInstance = this;
}

OarcManager::~OarcManager() {
    sInstance = nullptr;
}

void OarcManager::init(EGG::Heap *heap) {
    mArcTable.init(200, &ArcCallbackHandler::sInstance, heap);
}

bool OarcManager::checkIfObjectArcExistsOnDisk(const char *object) {
    return dRawArcEntry_c::checkArcExistsOnDisk(object, "Object");
}

bool OarcManager::loadObjectArcFromDisk(const char *object, EGG::Heap *heap) {
    if (heap == nullptr) {
        heap = dHeap::work2Heap.heap;
    }
    return mArcTable.getArcOrLoadFromDisk(object, "Object", 0, heap);
}

bool OarcManager::addEntryFromSuperArc(const char *object, void *data, EGG::Heap *heap) {
    if (heap == nullptr) {
        heap = EGG::Heap::sCurrentHeap;
    }
    return mArcTable.addEntryFromSuperArc(object, data, 0, heap);
}

int OarcManager::ensureLoaded1(const char *object) {
    return mArcTable.ensureLoadedMaybe2(object);
}

void OarcManager::ensureLoaded2(const char *object) {
    mArcTable.ensureLoadedMaybe(object);
}

bool OarcManager::decrement(const char *path) {
    return mArcTable.decreaseRefCount(path);
}

void *OarcManager::getData(const char *oarcName, const char *fileName) {
    return mArcTable.getDataFromOarc(oarcName, fileName);
}

void *OarcManager::getMdlFromArc2(const char *oarcName) {
    return mArcTable.getDataFromOarc(oarcName, "g3d/model.brres");
}

void *OarcManager::getMdlFromArc(const char *oarcName) {
    return mArcTable.getSubEntryData(oarcName, "g3d/model.brres");
}

void *OarcManager::getZevFromArc(const char *oarcName) {
    return mArcTable.getDataFromOarc(oarcName, "dat/zev.dat");
}

void *OarcManager::getDzbFromArc(const char *oarcName, const char *fileName) {
    SizedString<32> str = "dzb/";
    str += fileName;
    str += ".dzb";
    return mArcTable.getDataFromOarc(oarcName, str);
}

void *OarcManager::getPlcFromArc(const char *oarcName, const char *fileName) {
    SizedString<32> str = "dat/";
    str += fileName;
    str += ".plc";
    return mArcTable.getDataFromOarc(oarcName, str);
}

void *OarcManager::getSubEntryData(const char *oarcName, const char *fileName) {
    return mArcTable.getSubEntryData(oarcName, fileName);
}

bool OarcManager::create(EGG::Heap *heap) {
    new (heap, 0x04) OarcManager();

    if (OarcManager::sInstance == nullptr) {
        return false;
    }
    OarcManager::sInstance->init(heap);
    return true;
}
