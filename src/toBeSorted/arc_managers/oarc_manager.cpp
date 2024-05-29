#include <d/d_heap.h>
#include <d/d_rawarchive.h>
#include <egg/core/eggHeap.h>
// clang-format off
#include <sized_string.h>
// clang-format on

class OarcManager {
    OarcManager();
    virtual ~OarcManager();

    dRawArcTable_c mArcTable;

    static OarcManager *sInstance;

    static bool create(EGG::Heap *heap);

    void init(EGG::Heap *heap);

    bool checkIfObjectArcExistsOnDisk(const char *object);
    bool loadObjectArcFromDisk(const char *object, EGG::Heap *heap);
    bool addEntryFromSuperArc(const char *object, void *data, EGG::Heap *heap);
    void ensureLoaded1(const char *object);
    void ensureLoaded2(const char *object);
    bool decrement(const char *path);
    void *getData(const char *oarcName, const char *fileName);

    void *getMdlFromArc2(const char *oarcName);
    void *getMdlFromArc(const char *oarcName);
    void *getZevFromArc(const char *oarcName);
    void *getDzbFromArc(const char *oarcName, const char *fileName);
    void *getPlcFromArc(const char *oarcName, const char *fileName);
    void *getSubEntryData(const char *oarcName, const char *fileName);
};

OarcManager *OarcManager::sInstance;

OarcManager::OarcManager() {
    sInstance = this;
}

OarcManager::~OarcManager() {
    sInstance = nullptr;
}

extern char lbl_80575250;

void OarcManager::init(EGG::Heap *heap) {
    mArcTable.init(200, &lbl_80575250, heap);
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

void OarcManager::ensureLoaded1(const char *object) {
    mArcTable.ensureLoadedMaybe2(object);
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
    return mArcTable.getDataFromOarc(oarcName, &str);
}

void *OarcManager::getPlcFromArc(const char *oarcName, const char *fileName) {
    SizedString<32> str = "dat/";
    str += fileName;
    str += ".plc";
    return mArcTable.getDataFromOarc(oarcName, &str);
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
