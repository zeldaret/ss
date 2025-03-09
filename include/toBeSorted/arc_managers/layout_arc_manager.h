#ifndef LAYOUT_ARC_MANAGER_H
#define LAYOUT_ARC_MANAGER_H

#include "common.h"
#include "d/d_rawarchive.h"
#include "egg/core/eggHeap.h"

class LayoutArcManager {
public:
    LayoutArcManager();
    virtual ~LayoutArcManager();

    static bool create(EGG::Heap *heap);

    void init(EGG::Heap *heap);

    inline int ensureAllEntriesLoaded() {
        return mArcTable.ensureAllEntriesLoaded();
    }

    bool loadLayoutArcFromDisk(const char *object, EGG::Heap *heap);
    int ensureLoaded1(const char *object);
    int ensureLoaded2(const char *object);
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

class LayoutArcControl {
public:
    LayoutArcControl() : mLayoutArcs(nullptr), mNumArcs(0) {}
    virtual ~LayoutArcControl();
    void set(const char *const *layoutArcs, s32 numArcs);
    void load(EGG::Heap *heap);
    void release();

private:
    const char **mLayoutArcs;
    s32 mNumArcs;
};

#endif
