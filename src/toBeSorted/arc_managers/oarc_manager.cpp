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

dArcResult_e OarcManager::ensureLoaded1(const char *object) {
    return mArcTable.ensureLoadedMaybe2(object);
}

dArcResult_e OarcManager::ensureLoaded2(const char *object) {
    return mArcTable.ensureLoadedMaybe(object);
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

    if (GetInstance() == nullptr) {
        return false;
    }
    GetInstance()->init(heap);
    return true;
}

ObjectArcControl::~ObjectArcControl() {
    if (mObjectArcs != nullptr) {
        release();
    }
}

void ObjectArcControl::set(const char *const *objectArcs, s32 numArcs) {
    mObjectArcs = objectArcs;
    mNumArcs = numArcs;
}

bool ObjectArcControl::load(EGG::Heap *heap) const {
    if (mObjectArcs == nullptr) {
        return true;
    }

    const char *const *ptr = mObjectArcs;
    for (int i = 0; i < mNumArcs; i++) {
        OarcManager::GetInstance()->loadObjectArcFromDisk(*ptr, heap);
        ptr++;
    }

    return true;
}

bool ObjectArcControl::release() {
    if (mObjectArcs == nullptr) {
        return true;
    }

    const char *const *ptr = mObjectArcs;
    for (int i = 0; i < mNumArcs; i++) {
        dArcResult_e res = OarcManager::GetInstance()->ensureLoaded2(*ptr);
        if (res != D_ARC_RESULT_ERROR_NOT_FOUND && res != D_ARC_RESULT_OK) {
            return false;
        }
        ptr++;
    }

    ptr = mObjectArcs;
    for (int i = 0; i < mNumArcs; i++) {
        OarcManager::GetInstance()->decrement(*ptr);
        ptr++;
    }
    mObjectArcs = nullptr;

    return true;
}
