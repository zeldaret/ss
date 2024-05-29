#include <d/d_heap.h>
#include <d/d_rawarchive.h>
#include <egg/core/eggHeap.h>
// clang-format off
#include <sized_string.h>
// clang-format on

class CurrentStageArcManager {
    CurrentStageArcManager();
    virtual ~CurrentStageArcManager();

    SizedString<32> mStageName;
    SizedString<32> mCurrentLoadingStageArcName;
    SizedString<32> mStageExtraLayerArcName;

    dRawArcTable_c mArcTable;

    static CurrentStageArcManager *sInstance;

    static bool create(EGG::Heap *heap);

    void init(EGG::Heap *heap);
    bool setStage(const char *stage);
    bool decrement();
    void *getData(const char *fileName);
    void *getFileFromCurrentLay0Arc(const char *fileName);
    bool loadFileFromExtraLayerArc(int layer);
    bool unloadExtraLayerArc();
    bool loadRoomArc(int room);
    bool addEntryFromSuperArc(const char *name, void *data);
    bool unloadRoomArc(int room);
    bool decrement(const char *path);
    void *loadFromRoomArc(int roomId, const char *fileName);
    void *getDataFromRoomArc(int roomId, const char *fileName);
    const char *getCurrentStageDirectory();
    const char *getRoomArcDirectory(int room) const;

    static EGG::ExpHeap *getHeap();
};

CurrentStageArcManager *CurrentStageArcManager::sInstance;

CurrentStageArcManager::CurrentStageArcManager() {
    sInstance = this;
}

CurrentStageArcManager::~CurrentStageArcManager() {
    sInstance = nullptr;
}

extern char lbl_80575250;

void CurrentStageArcManager::init(EGG::Heap *heap) {
    mArcTable.init(18, &lbl_80575250, heap);
}

// sprintf
extern "C" void fn_8001F820(char *str, const char *src, ...);

bool CurrentStageArcManager::setStage(const char *newStage) {
    mStageName = newStage;

    mCurrentLoadingStageArcName.sprintf("%s_stg_l0", mStageName);
    if (dRawArcEntry_c::checkArcExistsOnDisk(&mCurrentLoadingStageArcName, getCurrentStageDirectory())) {
        return (bool)mArcTable.getArcOrLoadFromDisk(&mCurrentLoadingStageArcName, getCurrentStageDirectory(), 0,
                dHeap::work2Heap.heap);
    } else {
        mCurrentLoadingStageArcName.sprintf("%s_stg", mStageName);
        return (bool)mArcTable.getArcOrLoadFromDisk(&mCurrentLoadingStageArcName, getCurrentStageDirectory(), 0,
                dHeap::work2Heap.heap);
    }
}

bool CurrentStageArcManager::decrement() {
    return mArcTable.decreaseRefCount(&mCurrentLoadingStageArcName);
}

void *CurrentStageArcManager::getData(const char *fileName) {
    return mArcTable.getDataFromOarc(&mCurrentLoadingStageArcName, fileName);
}

void *CurrentStageArcManager::getFileFromCurrentLay0Arc(const char *fileName) {
    return mArcTable.getSubEntryData(&mCurrentLoadingStageArcName, fileName);
}

bool CurrentStageArcManager::loadFileFromExtraLayerArc(int layer) {
    if (layer == 0) {
        return true;
    }

    mStageExtraLayerArcName.sprintf("%s_stg_l%d", mStageName, layer);
    if (dRawArcEntry_c::checkArcExistsOnDisk(&mStageExtraLayerArcName, getCurrentStageDirectory())) {
        return (bool)mArcTable.getArcOrLoadFromDisk(&mStageExtraLayerArcName, getCurrentStageDirectory(), 0,
                dHeap::work2Heap.heap);
    } else {
        return true;
    }
}

bool CurrentStageArcManager::unloadExtraLayerArc() {
    if (mArcTable.hasEntry(&mStageExtraLayerArcName)) {
        return mArcTable.decreaseRefCount(&mStageExtraLayerArcName);
    }
    return true;
}

bool CurrentStageArcManager::loadRoomArc(int room) {
    return (bool)mArcTable.getArcOrLoadFromDisk(getRoomArcDirectory(room), getCurrentStageDirectory(), 0, dHeap::work2Heap.heap);
}

bool CurrentStageArcManager::addEntryFromSuperArc(const char *path, void *data) {
    return (bool)mArcTable.addEntryFromSuperArc(path, data, 0, EGG::Heap::sCurrentHeap);
}

bool CurrentStageArcManager::unloadRoomArc(int room) {
    mArcTable.decreaseRefCount(getRoomArcDirectory(room));
    return true;
}

bool CurrentStageArcManager::decrement(const char *path) {
    mArcTable.decreaseRefCount(path);
    return true;
}

void *CurrentStageArcManager::loadFromRoomArc(int roomId, const char *fileName) {
    return mArcTable.getDataFromOarc(getRoomArcDirectory(roomId), fileName);
}

void *CurrentStageArcManager::getDataFromRoomArc(int roomId, const char *fileName) {
    return mArcTable.getSubEntryData(getRoomArcDirectory(roomId), fileName);
}

EGG::ExpHeap *getHeap() {
    return dHeap::workExHeap.heap;
}

const char *CurrentStageArcManager::getCurrentStageDirectory() {
    static SizedString<64> sStageDirTmp;
    sStageDirTmp = "Stage/";
    sStageDirTmp += &mStageName;

    return &sStageDirTmp;
}

static SizedString<32> s_roomArcTmp;

const char *CurrentStageArcManager::getRoomArcDirectory(int room) const {
    s_roomArcTmp.sprintf("%s_r%02d", mStageName, room);
    return &s_roomArcTmp;
}

bool CurrentStageArcManager::create(EGG::Heap *heap) {
    new (heap, 0x04) CurrentStageArcManager();

    if (CurrentStageArcManager::sInstance == nullptr) {
        return false;
    }
    CurrentStageArcManager::sInstance->init(heap);
    return true;
}
