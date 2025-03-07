#ifndef CURRENT_STAGE_ARC_MANAGER_H
#define CURRENT_STAGE_ARC_MANAGER_H

#include "d/d_rawarchive.h"
#include "egg/core/eggHeap.h"
#include "sized_string.h"

class CurrentStageArcManager {
public:
    CurrentStageArcManager();
    virtual ~CurrentStageArcManager();

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

    EGG::ExpHeap *getHeap(s32 roomid);

private:
    SizedString<32> mStageName;
    SizedString<32> mCurrentLoadingStageArcName;
    SizedString<32> mStageExtraLayerArcName;

    dRawArcTable_c mArcTable;
};

#endif
