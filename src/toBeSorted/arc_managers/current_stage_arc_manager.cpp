#include "toBeSorted/arc_managers/current_stage_arc_manager.h"

#include "d/d_heap.h"


CurrentStageArcManager *CurrentStageArcManager::sInstance;

CurrentStageArcManager::CurrentStageArcManager() {
    sInstance = this;
}

CurrentStageArcManager::~CurrentStageArcManager() {
    sInstance = nullptr;
}

void CurrentStageArcManager::init(EGG::Heap *heap) {
    mArcTable.init(18, &ArcCallbackHandler::sInstance, heap);
}

bool CurrentStageArcManager::setStage(const char *newStage) {
    mStageName = newStage;
    // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
    mCurrentLoadingStageArcName.sprintf("%s_stg_l0", mStageName);
    if (dRawArcEntry_c::checkArcExistsOnDisk(mCurrentLoadingStageArcName, getCurrentStageDirectory())) {
        return (bool)mArcTable.getArcOrLoadFromDisk(
            mCurrentLoadingStageArcName, getCurrentStageDirectory(), 0, dHeap::work2Heap.heap
        );
    } else {
        // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
        mCurrentLoadingStageArcName.sprintf("%s_stg", mStageName);
        return (bool)mArcTable.getArcOrLoadFromDisk(
            mCurrentLoadingStageArcName, getCurrentStageDirectory(), 0, dHeap::work2Heap.heap
        );
    }
}

bool CurrentStageArcManager::decrement() {
    return mArcTable.decreaseRefCount(mCurrentLoadingStageArcName);
}

void *CurrentStageArcManager::getData(const char *fileName) {
    return mArcTable.getDataFromOarc(mCurrentLoadingStageArcName, fileName);
}

void *CurrentStageArcManager::getFileFromCurrentLay0Arc(const char *fileName) {
    return mArcTable.getSubEntryData(mCurrentLoadingStageArcName, fileName);
}

bool CurrentStageArcManager::loadFileFromExtraLayerArc(int layer) {
    if (layer == 0) {
        return true;
    }

    // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
    mStageExtraLayerArcName.sprintf("%s_stg_l%d", mStageName, layer);
    if (dRawArcEntry_c::checkArcExistsOnDisk(mStageExtraLayerArcName, getCurrentStageDirectory())) {
        return (bool
        )mArcTable.getArcOrLoadFromDisk(mStageExtraLayerArcName, getCurrentStageDirectory(), 0, dHeap::work2Heap.heap);
    } else {
        return true;
    }
}

bool CurrentStageArcManager::unloadExtraLayerArc() {
    if (mArcTable.hasEntry(mStageExtraLayerArcName)) {
        return mArcTable.decreaseRefCount(mStageExtraLayerArcName);
    }
    return true;
}

bool CurrentStageArcManager::loadRoomArc(int room) {
    return (bool
    )mArcTable.getArcOrLoadFromDisk(getRoomArcDirectory(room), getCurrentStageDirectory(), 0, dHeap::work2Heap.heap);
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

EGG::ExpHeap *CurrentStageArcManager::getHeap(s32 roomid) {
    return dHeap::workExHeap.heap;
}

const char *CurrentStageArcManager::getCurrentStageDirectory() {
    static SizedString<64> sStageDirTmp;
    sStageDirTmp = "Stage/";
    sStageDirTmp += mStageName;

    return sStageDirTmp;
}

static SizedString<32> s_roomArcTmp;

const char *CurrentStageArcManager::getRoomArcDirectory(int room) const {
    // UB: Cannot pass object of non-POD type 'const SizedString<32>' through variadic method
    s_roomArcTmp.sprintf("%s_r%02d", mStageName, room);
    return s_roomArcTmp;
}

bool CurrentStageArcManager::create(EGG::Heap *heap) {
    new (heap, 0x04) CurrentStageArcManager();

    if (CurrentStageArcManager::sInstance == nullptr) {
        return false;
    }
    CurrentStageArcManager::sInstance->init(heap);
    return true;
}
