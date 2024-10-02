#ifndef LAYOUT_ARC_MANAGER_H
#define LAYOUT_ARC_MANAGER_H

#include <d/d_rawarchive.h>
#include <egg/core/eggHeap.h>
#include <sized_string.h>

class LayoutArcManager {
public:
    LayoutArcManager();
    virtual ~LayoutArcManager();

    dRawArcTable_c mArcTable;

    static LayoutArcManager *sInstance;

    static bool create(EGG::Heap *heap);

    void init(EGG::Heap *heap);

    bool loadLayoutArcFromDisk(const char *object, EGG::Heap *heap);
    void ensureLoaded1(const char *object);
    void ensureLoaded2(const char *object);
    bool hasEntry(const char *object);
    bool decrement(const char *path);
    void *getData(const char *oarcName, const char *fileName);
    void *getLoadedData(const char *path);
};

#endif
