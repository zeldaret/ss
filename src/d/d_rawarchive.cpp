#include <d/d_rawarchive.h>
#include <rvl/VI.h>


int computeChecksumInner(void *d, u32 len) {
    u32 *data = (u32 *)d;
    u32 result = 0;
    // Compiler will unroll this loop
    for (u32 len_words = len / 4; len_words != 0; len_words--) {
        result += *data++;
    }
    return result;
}

int computeChecksum(void *data, u32 len) {
    int result = computeChecksumInner(data, len);
    return result != 0 ? result : -1;
}

void setPrefix(ArcCallbackHandler *mgr, const char *name, size_t len) {
    mgr->mPrefix = '    ';
    // Copy the actual name
    memcpy(&mgr->mPrefix, name, len);
}

dRawArcEntry_c::dRawArcEntry_c() {
    mRefCount = 0;
    mpDvdReq = nullptr;
    mpArc = nullptr;
    mpCommandHeap = nullptr;
    mpData = nullptr;
    mChecksum = 0;
    mpFrmHeap = 0;
}

dRawArcEntry_c::~dRawArcEntry_c() {
    // Wait for request to complete before destroying
    if (mpDvdReq->mStatus == 0) {
        do {
            VIWaitForRetrace();
        } while (mpDvdReq->mStatus == 0);
    }
    destroy(nullptr);
}

void dRawArcEntry_c::searchCallback(void *arg, void *data, const ARCDirEntry *entry, const char *path, bool ctrl) {
    ArcCallbackHandler *mgr = (ArcCallbackHandler *)arg;
    if (entry->isDir) {
        int len = strlen(entry->name);
        setPrefix(mgr, entry->name, len <= 4 ? len : 4);
    } else {
        // dolphin: arg vtable at 8050df50
        // any others?
        if (ctrl) {
            mgr->CreateArcEntry(data, path);
            // branch to 800651c0, sets up room
        } else {
            mgr->DestroyArcEntry(path);
            // branch to 800653d0, destroys room
        }
    }
}

void dRawArcEntry_c::searchCallback1(void *a, void *b, const ARCDirEntry *c, const char *d) {
    searchCallback(a, b, c, d, true);
}

void dRawArcEntry_c::searchCallback2(void *a, void *b, const ARCDirEntry *c, const char *d) {
    searchCallback(a, b, c, d, false);
}

bool dRawArcEntry_c::destroy(void *arg) {
    if (mpDvdReq != nullptr) {
        if (mpDvdReq->mStatus == 0) {
            // Can't destroy if the request is still ongoing
            return false;
        }
        mpArc = mpDvdReq->mDataPtr;
        mpData = mpDvdReq->getArcBinary();
        mAmountRead = mpDvdReq->mAmountRead;
        mpCommandHeap = mpDvdReq->mHeap;
        mpDvdReq->do_delete();
        mpDvdReq = nullptr;
    }

    if (mpArc != nullptr) {
        if (arg != nullptr) {
            mpArc->searchInside(searchCallback2, arg);
        }

        if (mpFrmHeap != nullptr) {
            mHeap::destroyFrmHeap(mpFrmHeap);
            mpFrmHeap = nullptr;
        }
        mpArc->unmount();
        mpArc = nullptr;
    }

    if (mpData != nullptr) {
        EGG::Heap::free(mpData, nullptr);
        mpData = nullptr;
    }

    mAmountRead = 0;
    mChecksum = 0;
    mpCommandHeap = 0;
    return true;
}

bool dRawArcEntry_c::loadArcFromDiskChecked(const char *fileName, const char *dirName, u8 mountDirection,
        EGG::Heap *heap) {
    SizedString<128> path;

    if (checkArcExistsOnDiskInner(path, fileName, dirName)) {
        return loadArcFromDisk(fileName, &path, mountDirection, heap);
    }
    return false;
}

bool dRawArcEntry_c::loadArcFromDisk(const char *arcName, const char *arcPath, u8 mountDirection, EGG::Heap *heap) {
    mpDvdReq = mDvd_mountMemArchive_c::create(arcPath, mountDirection, heap);
    if (mpDvdReq == nullptr) {
        return false;
    }
    mArcName = arcName;
    return true;
}

BOOL dRawArcEntry_c::checkArcExistsOnDisk(const char *fileName, const char *dirName) {
    SizedString<128> path;
    return checkArcExistsOnDiskInner(path, fileName, dirName);
}

BOOL dRawArcEntry_c::checkArcExistsOnDiskInner(SizedString<128> &path, const char *fileName, const char *dirName) {
    path.sprintf("/US/%s/%s.arc", dirName, fileName);
    if (!mDvd::IsExistPath(&path)) {
        path.sprintf("/%s/%s.arc", dirName, fileName);
        if (!mDvd::IsExistPath(&path)) {
            return false;
        }
    }
    return true;
}

int dRawArcEntry_c::mount(const char *name, void *data, ArcCallbackHandler *callbackArg, u8 mountDirection, EGG::Heap *heap) {
    mArcName = name;
    mpArc = EGG::Archive::mount(data, heap, (mountDirection == 0 || mountDirection == 1) ? 4 : -4);
    if (mpArc == nullptr) {
        return -1;
    } else {
        int result = onMount(callbackArg);
        int ret = 0;
        if (result == -1) {
            ret = result;
        }
        return ret;
    }
}

int dRawArcEntry_c::ensureLoadedMaybe(void *callbackArg) {
    if (mpArc == nullptr) {
        if (mpDvdReq == nullptr) {
            return -1;
        }

        if (mpDvdReq->mStatus == 0) {
            return 1;
        }

        mpArc = mpDvdReq->mDataPtr;
        mpData = mpDvdReq->getArcBinary();
        mAmountRead = mpDvdReq->mAmountRead;
        if (mpData != nullptr) {
            u32 blockSize = EGG::ExpHeap::getSizeForMBlock(mpData);
            if (mAmountRead != blockSize) {
                mAmountRead = blockSize;
            }
        }

        mpCommandHeap = mpDvdReq->mHeap;
        mpDvdReq->do_delete();
        mpDvdReq = nullptr;
        if (mpArc == nullptr) {
            return -1;
        }
        mpFrmHeap = mHeap::makeHeapOnCurrentGameHeap(-1, name(), 0x20, 0);
        if (mpFrmHeap == nullptr) {
            return -1;
        }
        int result = onMount(callbackArg);
        mHeap::restoreCurrentHeap();
        mHeap::adjustFrmHeap(mpFrmHeap);
        mChecksum = computeChecksum(mpData, mAmountRead);
        if (result == -1) {
            return result;
        }
        DCStoreRange(mpFrmHeap, mpFrmHeap->mHeapHandle->end - (u8 *)mpFrmHeap);
    }

    return 0;
}

int dRawArcEntry_c::onMount(void *callbackArg) {
    mpArc->countFile();
    if (callbackArg != nullptr) {
        mpArc->searchInside(searchCallback1, callbackArg);
    }
    return 0;
}

dRawArcTable_c::dRawArcTable_c() {
    mpEntries = nullptr;
    mCount = 0;
    mCallbackArg = nullptr;
}

dRawArcTable_c::~dRawArcTable_c() {
    if (mpEntries) {
        delete[] mpEntries;
        mpEntries = nullptr;
        mCount = 0;
    }
}

bool dRawArcTable_c::init(u16 count, ArcCallbackHandler *callbackArg, EGG::Heap *heap) {
    mpEntries = new (heap, 0x04) dRawArcEntry_c[count]();
    if (mpEntries == nullptr) {
        return false;
    }

    mCount = count;
    mCallbackArg = callbackArg;
    return true;
}

BOOL dRawArcTable_c::getArcOrLoadFromDisk(const char *name, const char *dirName, u8 mountDirection, EGG::Heap *heap) {
    dRawArcEntry_c *entry = findEntry(name);
    if (entry == nullptr) {
        entry = findEmptySlot();
        if (entry == nullptr) {
            return false;
        }
        bool result = entry->loadArcFromDiskChecked(name, dirName, mountDirection, heap);
        if (!result) {
            entry->destroy(mCallbackArg);
            return false;
        }
    }
    entry->increaseRefCount();
    return true;
}

BOOL dRawArcTable_c::addEntryFromSuperArc(const char *name, void *data, u8 mountDirection, EGG::Heap *heap) {
    dRawArcEntry_c *entry = findEntry(name);
    if (entry == nullptr) {
        entry = findEmptySlot();
        if (entry == nullptr) {
            return false;
        }
        int result = entry->mount(name, data, mCallbackArg, mountDirection, heap);
        if (result != 0) {
            entry->destroy(mCallbackArg);
            return false;
        }
    }
    entry->increaseRefCount();
    return true;
}

int dRawArcTable_c::ensureLoadedMaybe2(const char *name) {
    dRawArcEntry_c *entry = findEntry(name);
    if (entry == nullptr) {
        return -2;
    }
    return entry->ensureLoadedMaybe(mCallbackArg);
}

int dRawArcTable_c::ensureLoadedMaybe(const char *name) {
    dRawArcEntry_c *entry = findEntry(name);
    if (entry == nullptr) {
        return -2;
    }
    return entry->ensureLoadedMaybe(mCallbackArg);
}

bool dRawArcTable_c::hasEntry(const char *name) {
    return findEntry(name) != nullptr;
}

BOOL dRawArcTable_c::decreaseRefCount(const char *name) {
    dRawArcEntry_c *entry = findEntry(name);
    if (entry == nullptr) {
        return false;
    }
    entry->decreaseRefCount();
    if (!entry->isReferenced()) {
        entry->destroy(mCallbackArg);
    }
    return true;
}

void *dRawArcTable_c::getDataFromOarc(const char *name, const char *path) {
    void *data = nullptr;
    dRawArcEntry_c *entry = findLoadedEntry(name);
    if (entry != nullptr) {
        EGG::Archive *arc = entry->getArc();
        if (arc != nullptr) {
            data = arc->getFile(path, nullptr);
        }
    }
    return data;
}

void *dRawArcTable_c::getSubEntryData(const char *name, const char *path) {
    void *data = nullptr;
    dRawArcEntry_c *entry = findLoadedEntry(name);
    if (entry != nullptr) {
        EGG::Archive *arc = entry->getArc();
        if (arc != nullptr) {
            int num = arc->convertPathToEntryID(path);
            if (num >= 0) {
                data = arc->getFileFast(num, nullptr);
            }
        }
    }
    return data;
}

void *dRawArcTable_c::getLoadedArcData(const char *name) {
    dRawArcEntry_c *entry = findLoadedEntry(name);
    return entry == nullptr ? nullptr : entry->getData();
}

int dRawArcTable_c::ensureAllEntriesLoaded() {
    dRawArcEntry_c *entry = mpEntries;
    int result = 0;
    for (int i = 0; i < mCount; i++) {
        if (entry->isLoading()) {
            result = entry->ensureLoadedMaybe(mCallbackArg);
            if (result != 0) {
                return result;
            }
        }
        entry++;
    }

    return 0;
}

extern "C" bool strequals(const char *, const char *);

dRawArcEntry_c *dRawArcTable_c::findEntry(const char *name) const {
    dRawArcEntry_c *entry = mpEntries;
    for (int i = 0; i < mCount; i++) {
        if (entry->isReferenced() && strequals(entry->name(), name)) {
            return entry;
        }
        entry++;
    }

    return nullptr;
}

dRawArcEntry_c *dRawArcTable_c::findEmptySlot() {
    dRawArcEntry_c *entry = mpEntries;
    for (int i = 0; i < mCount; i++) {
        if (!entry->isReferenced()) {
            return entry;
        }
        entry++;
    }

    return nullptr;
}

dRawArcEntry_c *dRawArcTable_c::findLoadedEntry(const char *name) {
    dRawArcEntry_c *entry = findEntry(name);
    if (entry == nullptr) {
        return nullptr;
    } else if (entry->isNotLoaded()) {
        return nullptr;
    }
    return entry;
}
