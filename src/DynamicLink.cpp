#include <DynamicLink.h>

DynamicModuleControlBase *DynamicModuleControlBase::mFirst;
DynamicModuleControlBase *DynamicModuleControlBase::mLast;

DynamicModuleControlBase::~DynamicModuleControlBase() {
    if (mPrev != nullptr) {
        mPrev->mNext = mNext;
    }
    if (mNext != nullptr) {
        mNext->mPrev = mPrev;
    }
    if (mFirst == this) {
        mFirst = mNext;
    }
    if (mLast == this) {
        mLast = mPrev;
    }
    mNext = nullptr;
    mPrev = nullptr;
}

DynamicModuleControlBase::DynamicModuleControlBase() {
    mLinkCount = 0;
    mDoLinkCount = 0;
    mNext = nullptr;
    if (mFirst == nullptr) {
        mFirst = this;
    }
    DynamicModuleControlBase *last = mLast;
    mPrev = last;
    if (last != nullptr) {
        last->mNext = this;
    }
    mLast = this;
}

BOOL DynamicModuleControlBase::link() {
    if (mLinkCount == 0) {
        do_load();
        if (!do_link()) {
            return false;
        }

        if (mDoLinkCount < 0xFFFF) {
            mDoLinkCount++;
        }
    }

    if (mLinkCount < 0xFFFF) {
        mLinkCount++;
    }

    return true;
}

BOOL DynamicModuleControlBase::unlink() {
    if (mLinkCount != 0) {
        if (mLinkCount < 0xFFFF && --mLinkCount == 0) {
            do_unlink();
            do_unload();
        }
    }

    return true;
}

BOOL DynamicModuleControlBase::load_async() {
    if (mLinkCount == 0) {
        return do_load_async();
    }
    return true;
}

const char *DynamicModuleControlBase::getModuleName() const {
    return nullptr;
}

int DynamicModuleControlBase::getModuleSize() const {
    return 0;
}

const char *DynamicModuleControlBase::getModuleTypeString() const {
    return "Base";
}

void DynamicModuleControlBase::dump() {}

bool DynamicModuleControlBase::do_load() {
    return true;
}

BOOL DynamicModuleControlBase::do_load_async() {
    return true;
}

bool DynamicModuleControlBase::do_unload() {
    return true;
}

BOOL DynamicModuleControlBase::do_link() {
    return true;
}

bool DynamicModuleControlBase::do_unlink() {
    return true;
}

u32 DynamicModuleControl::sAllocBytes;
mDvd_toMainRam_base_c *DynamicModuleControl::sDvdFile;
EGG::ExpHeap *DynamicModuleControl::sDylinkHeap;
u32 DynamicModuleControl::sCurrentAllocatableSize;
u32 DynamicModuleControl::sTotalFreeSize;
EGG::Archive *DynamicModuleControl::sArchive;

DynamicModuleControl::DynamicModuleControl(const char *name, EGG::ExpHeap *heap)
    : mModule(nullptr), mBss(nullptr), unk_24(0), mName(name), mResourceType(0), unk_33(0), unk_40(0),
      mDvdCallbackRequest(0), mHeap(heap), mpRelMapFile() {}

DynamicModuleControl::~DynamicModuleControl() {}

const char *DynamicModuleControl::getModuleName() const {
    return mName;
}

void DynamicModuleControl::initialize(EGG::ExpHeap *heap) {
    sDylinkHeap = heap;
    sCurrentAllocatableSize = heap->getAllocatableSize(4);
    sTotalFreeSize = heap->getTotalFreeSize();
}

u32 DynamicModuleControl::callback(void *arg) {
    return static_cast<DynamicModuleControl *>(arg)->do_load();
}

void DynamicModuleControl::checkHeapStatus() {
    if (mHeap == sDylinkHeap) {
        int size = mHeap->getAllocatableSize(4);
        if (sCurrentAllocatableSize > size) {
            sCurrentAllocatableSize = size;
        }
        size = mHeap->getTotalFreeSize();
        if (sTotalFreeSize > size) {
            sTotalFreeSize = size;
        }
    }
}

const char *DynamicModuleControl::sRelsDir = "/rels";

bool DynamicModuleControl::do_load() {
    char buf[64];
    if (mModule != nullptr) {
        return true;
    }
    if (mHeap == nullptr) {
        mHeap = sDylinkHeap;
    }
    snprintf(buf, sizeof(buf), "%s/%sNP.rel", sRelsDir, mName);
    if (mModule == nullptr) {
        if (sArchive != nullptr) {
            sDvdFile = mDvd_toMainRam_arc_c::createOrFail(sArchive, buf, 1, mHeap);
        } else {
            sDvdFile = mDvd_toMainRam_normal_c::createOrFail(buf, 1, mHeap);
        }

        if (sDvdFile != nullptr) {
            sDvdFile->waitUntilDone();
            checkHeapStatus();
            void *ptr = sDvdFile->mDataPtr;
            sDvdFile->mDataPtr = nullptr;
            mModule = static_cast<OSModuleHeader *>(ptr);
            sDvdFile->do_delete();
            sDvdFile = nullptr;
            if (mModule != nullptr) {
                unk_40 = 0;
                mResourceType = 3;
            }
        }
    }

    if (mModule == nullptr) {
        return false;
    }
    if (unk_33 < 0xFF) {
        unk_33++;
    }
    checkHeapStatus();
    return true;
}

BOOL DynamicModuleControl::do_load_async() {
    if (mDvdCallbackRequest == nullptr) {
        if (mModule != nullptr) {
            return true;
        }
        mDvdCallbackRequest = mDvd_callback_c::create(callback, this);
    }

    if (mDvdCallbackRequest != nullptr && mDvdCallbackRequest->mStatus != 0) {
        mDvdCallbackRequest->do_delete();
        mDvdCallbackRequest = nullptr;
        return true;
    } else {
        return false;
    }
}

bool DynamicModuleControl::do_unload() {
    if (mModule != nullptr) {
        mHeap->free(mModule);
        mModule = nullptr;
    }

    return true;
}

void DynamicModuleControl::dump() {}

BOOL DynamicModuleControl::do_link() {
    char mapPath[64];

    if (mModule != nullptr) {
        int alignedFixSize = ROUND_UP(mModule->fixSize, 0x20);
        int alignedFixPtr = (int)mModule + alignedFixSize;
        int totalSize = EGG::ExpHeap::getSizeForMBlock(mModule);
        BOOL result;
        if (totalSize == 0) {
            void *bss = mHeap->alloc(mModule->bssSize, 0x20);
            if (bss == nullptr) {
                goto error;
            }
            mBss = bss;
            result = OSLink(&mModule->info, bss);
            if (!result) {
                goto error;
            }
        } else {
            if (alignedFixSize + mModule->bssSize < totalSize) {
                result = OSLinkFixed(&mModule->info, (void *)alignedFixPtr);
                if (!result) {
                    goto error;
                }
                mHeap->resizeForMBlock(mModule, alignedFixSize + mModule->bssSize);
            } else if ((int)mHeap->resizeForMBlock(mModule, alignedFixSize + mModule->bssSize) > 0) {
                // TODO resizeForMBlock should maybe return int?
                result = OSLinkFixed(&mModule->info, (void *)alignedFixPtr);
                if (!result) {
                    goto error;
                }
            } else {
                void *bss = mHeap->alloc(mModule->bssSize, 0x20);
                if (!bss) {
                    goto error;
                }
                mBss = bss;
                result = OSLinkFixed(&mModule->info, bss);
                if (!result) {
                    goto error;
                }
                mHeap->resizeForMBlock(mModule, alignedFixSize);
            }
        }

        sAllocBytes = sAllocBytes + getModuleSize();
        snprintf(mapPath, sizeof(mapPath), "%s/%sNP.map", "/maps", mName);
        mpRelMapFile.RegisterOnDvd(mapPath, &mModule->info);
        unk_24 = ((UNKWORD(*)())mModule->prolog)();
        checkHeapStatus();
        return true;
    }

error:
    unk_33 = 0;
    if (mBss != nullptr) {
        mHeap->free(mBss);
        mBss = nullptr;
    }
    if (mModule != nullptr) {
        mHeap->free(mModule);
        mModule = nullptr;
    }
    return false;
}

bool DynamicModuleControl::do_unlink() {
    ((void (*)())mModule->epilog)();
    mpRelMapFile.Unregister();
    if (!OSUnlink(&mModule->info)) {
        return false;
    }
    sAllocBytes -= getModuleSize();
    if (mBss != nullptr) {
        mHeap->free(mBss);
        mBss = nullptr;
    }
    return true;
}

int DynamicModuleControl::getModuleSize() const {
    if (mModule != nullptr) {
        int size = EGG::ExpHeap::getSizeForMBlock(mModule);
        if (mBss != nullptr) {
            size += mModule->bssSize;
        }

        return size;
    }
    return 0;
}

const char *DynamicModuleControl::getModuleTypeString() const {
    static const char *REL_LOAD_TYPES[4] = {
            "????",
            "MEM",
            "ARAM",
            "DVD",
    };
    return REL_LOAD_TYPES[mResourceType & 3];
}

DbMapFile::~DbMapFile() {
    if (mMapFileHandle != nullptr) {
        Unregister();
    }
}

void DbMapFile::RegisterOnDvd(const char *path, const OSModuleInfo *info) {
    if (mDvd::IsExistPath(path)) {
        mMapFileHandle = nw4r::db::MapFile_RegistOnDvd(&mMapFile, path, info);
    } else {
        mMapFileHandle = (nw4r::db::MapFileHandle)0xffffffff;
    }
}

void DbMapFile::Unregister() {
    if (mMapFileHandle != nullptr) {
        if ((u32)mMapFileHandle != 0xffffffff) {
            nw4r::db::MapFile_Unregist(mMapFileHandle);
        }
        mMapFileHandle = nullptr;
    }
}

extern "C" {

void ModuleProlog() {}

void ModuleEpilog() {}

void ModuleUnresolved() {}

void ModuleConstructorsX(void (**ptrs)()) {
    for (; *ptrs != nullptr; ptrs++) {
        (*ptrs)();
    }
}

void ModuleDestructorsX(void (**ptrs)()) {
    for (; *ptrs != nullptr; ptrs++) {
        (*ptrs)();
    }
}
}
