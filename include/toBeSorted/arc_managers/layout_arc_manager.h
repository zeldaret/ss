#ifndef LAYOUT_ARC_MANAGER_H
#define LAYOUT_ARC_MANAGER_H

#include "common.h"
#include "d/d_rawarchive.h"
#include "egg/core/eggHeap.h"

class LayoutArcControl {
public:
    LayoutArcControl() : mLayoutArcs(nullptr), mNumArcs(0) {}
    virtual ~LayoutArcControl();
    void set(const char *const *layoutArcs, s32 numArcs);
    bool load(EGG::Heap *heap) const;
    bool allLoaded() const;
    bool release();

private:
    const char *const *mLayoutArcs;
    s32 mNumArcs;
};

class LayoutArcManager {
public:
    LayoutArcManager();
    virtual ~LayoutArcManager();

    static bool create(EGG::Heap *heap);

    void init(EGG::Heap *heap);

    inline dArcResult_e ensureAllEntriesLoaded() {
        return mArcTable.ensureAllEntriesLoaded();
    }

    bool loadLayoutArcFromDisk(const char *object, EGG::Heap *heap);
    dArcResult_e ensureLoaded1(const char *object);
    dArcResult_e ensureLoaded2(const char *object);
    bool hasEntry(const char *object);
    bool decrement(const char *path);
    void *getData(const char *oarcName, const char *fileName);
    void *getLoadedData(const char *path);

    static LayoutArcManager *GetInstance() {
        return sInstance;
    }

private:
    static LayoutArcManager *sInstance;
    dRawArcTable_c mArcTable;
};

#endif
