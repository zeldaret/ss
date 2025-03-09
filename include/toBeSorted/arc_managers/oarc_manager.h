#ifndef OARC_MANAGER_H
#define OARC_MANAGER_H

#include "d/d_rawarchive.h"
#include "egg/core/eggHeap.h"

class OarcManager {
public:
    OarcManager();
    virtual ~OarcManager();

    static OarcManager *sInstance;

    static bool create(EGG::Heap *heap);

    void init(EGG::Heap *heap);

    inline int ensureAllEntriesLoaded() {
        return mArcTable.ensureAllEntriesLoaded();
    }

    bool checkIfObjectArcExistsOnDisk(const char *object);
    bool loadObjectArcFromDisk(const char *object, EGG::Heap *heap);
    bool addEntryFromSuperArc(const char *object, void *data, EGG::Heap *heap);
    int ensureLoaded1(const char *object);
    void ensureLoaded2(const char *object);
    bool decrement(const char *path);
    void *getData(const char *oarcName, const char *fileName);

    void *getMdlFromArc2(const char *oarcName);
    void *getMdlFromArc(const char *oarcName);
    void *getZevFromArc(const char *oarcName);
    void *getDzbFromArc(const char *oarcName, const char *fileName);
    void *getPlcFromArc(const char *oarcName, const char *fileName);
    void *getSubEntryData(const char *oarcName, const char *fileName);

private:
    dRawArcTable_c mArcTable;
};

class ObjectArcControl {
public:
    ObjectArcControl() : mObjectArcs(nullptr), mNumArcs(0) {}
    virtual ~ObjectArcControl();
    void set(const char *const *objectArcs, s32 numArcs);
    void load(EGG::Heap *heap);
    void release();

private:
    const char **mObjectArcs;
    s32 mNumArcs;
};

#endif
