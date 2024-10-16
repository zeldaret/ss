#ifndef D_RAWARCHIVE_H
#define D_RAWARCHIVE_H

#include <common.h>
#include <m/m_dvd.h>
#include <sized_string.h>

class ArcCallbackHandlerBase {
public:
    ArcCallbackHandlerBase(): mPrefix('    ') {}
    /* vtable at 8050df50 */
    /** 800651c0 */
    virtual void CreateArcEntry(void *data, const char *path);
    /** 800653d0 */
    virtual void DestroyArcEntry(const char *path);

    u32 mPrefix;
};

class ArcCallbackHandler : public ArcCallbackHandlerBase {
public:
    ArcCallbackHandler() {}
    static ArcCallbackHandler sInstance;
};

// TODO: loading status could be an enum (-2/-1/0/+1)

// Ghidra: ArcManagerEntry
//   size: 0x40
class dRawArcEntry_c {
public:
    dRawArcEntry_c();
    ~dRawArcEntry_c();

    bool destroy(void *arg);
    bool loadArcFromDiskChecked(const char *fileName, const char *dirName, u8 mountDirection, EGG::Heap *heap);
    bool loadArcFromDisk(const char *fileName, const char *dirName, u8 mountDirection, EGG::Heap *heap);
    static BOOL checkArcExistsOnDisk(const char *fileName, const char *dirName);
    static BOOL checkArcExistsOnDiskInner(SizedString<128> &path, const char *fileName, const char *dirName);

    int mount(const char *name, void *data, ArcCallbackHandler *callbackArg, u8 mountDirection, EGG::Heap *heap);
    int onMount(void *callbackArg);

    int ensureLoadedMaybe(void *callbackArg);

    inline bool isReferenced() {
        return mRefCount != 0;
    }

    inline const SizedString<32> &name() const {
        return mArcName;
    }

    inline bool isNotLoaded() {
        return mpArc == nullptr;
    }

    inline bool isLoading() {
        return mpDvdReq != nullptr;
    }

    inline void increaseRefCount() {
        mRefCount++;
    }

    inline void decreaseRefCount() {
        mRefCount--;
    }

    inline void *getData() {
        return mpData;
    }

    inline EGG::Archive *getArc() {
        return mpArc;
    }

private:
    static void searchCallback(void *, void *, const ARCDirEntry *, const char *, bool);
    static void searchCallback1(void *, void *, const ARCDirEntry *, const char *);
    static void searchCallback2(void *, void *, const ARCDirEntry *, const char *);

private:
    /* 0x00 */ SizedString<32> mArcName;
    /* 0x20 */ u16 mRefCount;
    /* 0x24 */ mDvd_mountMemArchive_c *mpDvdReq;
    /* 0x28 */ EGG::Archive *mpArc;
    /* 0x2C */ EGG::Heap *mpCommandHeap;
    /* 0x30 */ void *mpData;
    /* 0x34 */ u32 mAmountRead;
    /* 0x38 */ u32 mChecksum;
    /* 0x3C */ EGG::FrmHeap *mpFrmHeap;
};

// Ghidra: ArcManagerEntryTable
//   size: 0xC
class dRawArcTable_c {
public:
    dRawArcTable_c();
    ~dRawArcTable_c();

    bool init(u16 count, ArcCallbackHandler *callbackArg, EGG::Heap *heap);
    BOOL getArcOrLoadFromDisk(const char *name, const char *dirName, u8 mountDirection, EGG::Heap *heap);
    BOOL addEntryFromSuperArc(const char *name, void *data, u8 mountDirection, EGG::Heap *heap);
    int ensureLoadedMaybe2(const char *name);
    int ensureLoadedMaybe(const char *name);
    bool hasEntry(const char *name);
    BOOL decreaseRefCount(const char *name);
    void *getDataFromOarc(const char *name, const char *path);
    void *getSubEntryData(const char *name, const char *path);
    void *getLoadedArcData(const char *name);
    int ensureAllEntriesLoaded();
    dRawArcEntry_c *findEntry(const char *name) const;
    dRawArcEntry_c *findEmptySlot();
    dRawArcEntry_c *findLoadedEntry(const char *name);

private:
    /* 0x0 */ dRawArcEntry_c *mpEntries;
    /* 0x4 */ u16 mCount;
    /* 0x8 */ ArcCallbackHandler *mCallbackArg;
};

#endif;
