#ifndef OARC_MANAGER_H
#define OARC_MANAGER_H

#include <d/d_rawarchive.h>
#include <egg/core/eggHeap.h>
#include <sized_string.h>

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

#endif
