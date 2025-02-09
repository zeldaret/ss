#ifndef LAYOUT_ARC_MANAGER_H
#define LAYOUT_ARC_MANAGER_H

#include "d/d_rawarchive.h"
#include "egg/core/eggHeap.h"

class LayoutArcManager {
public:
    LayoutArcManager();
    virtual ~LayoutArcManager();

    static LayoutArcManager *sInstance;

    static bool create(EGG::Heap *heap);

    void init(EGG::Heap *heap);

    inline int ensureAllEntriesLoaded() {
        return mArcTable.ensureAllEntriesLoaded();
    }

    bool loadLayoutArcFromDisk(const char *object, EGG::Heap *heap);
    int ensureLoaded1(const char *object);
    void ensureLoaded2(const char *object);
    bool hasEntry(const char *object);
    bool decrement(const char *path);
    void *getData(const char *oarcName, const char *fileName);
    void *getLoadedData(const char *path);

private:
    dRawArcTable_c mArcTable;
};

#endif
