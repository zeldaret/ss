#include <egg/core/eggArchive.h>
#include <egg/core/eggDecomp.h>
#include <egg/core/eggDvdFile.h>
#include <egg/core/eggDvdRipper.h>
#include <egg/core/eggHeap.h>
#include <m/m_dvd.h>
#include <m/m_heap.h>

namespace mDvd {

TUncompressInfo_c<EGG::StreamDecompSZS> DECOMP_SZS(3, ".szs");
TUncompressInfo_c<EGG::StreamDecompLZ> DECOMP_LZ(5, ".LZ");
TUncompressInfo_c<EGG::StreamDecompLH> DECOMP_LH(7, ".LH");
TUncompressInfo_c<EGG::StreamDecompLRC> DECOMP_LRC(8, ".LRC");
TUncompressInfo_c<EGG::StreamDecompRL> DECOMP_RL(4, ".RL");

// TODO This the space within which the actual decompressors
// are allocated.
char likely_align[0x7A04];
char more_empty_space[0x1638];
char decompressor_alloc_space[0x9930];

mDvd_param_c DVD_MANAGER;

TUncompressInfo_Base_c *decompressorPtrs[1] = {
        &DECOMP_LZ,
};

u8 g_mountDirection = 1;
int maxChunkSize = 0x10000;

void *somePtr;
u32 someNumber;
TUncompressInfo_Base_c **compressors_ptr;
TUncompressInfo_Base_c **compressors_last;
OSThread *sOsThread;
mDvd::MyThread_c *sThread;
EGG::Heap *sCommandHeap;
EGG::Heap *sArchiveHeap;
bool isAutoStreamDecomp;
bool isRegistered;

/** 802eece0 */
extern "C" void fn_802EECE0(TUncompressInfo_Base_c **ptr, TUncompressInfo_Base_c **last) {
    compressors_ptr = ptr;
    compressors_last = last;
}

/** 802eecf0 */
extern "C" void fn_802EECF0() {
    fn_802EECE0(decompressorPtrs, &decompressorPtrs[1]);
    somePtr = decompressor_alloc_space;
    someNumber = 0x24;
}

/** 802eed30 */
TUncompressInfo_Base_c *findUncompressInfo(u8 type) {
    TUncompressInfo_Base_c **ptr = compressors_ptr;
    while (ptr != compressors_last) {
        if ((*ptr)->mType == type) {
            return *ptr;
        }
        ptr++;
    }
    return nullptr;
}

/** 802eed70 */
EGG::StreamDecomp *newUncompressObj(u8 type) {
    TUncompressInfo_Base_c *factory = findUncompressInfo(type);
    if (factory != nullptr) {
        return factory->Construct();
    } else {
        return nullptr;
    }
}

/** 802eedb0 */
void deleteUncompressObj(u8 type) {
    TUncompressInfo_Base_c *factory = findUncompressInfo(type);
    if (factory != nullptr) {
        factory->Destruct();
    }
}

OSThread *getOSThread() {
    return sOsThread;
}

EGG::Heap *getArchiveHeap() {
    return sArchiveHeap;
}

/** 802eee10 */
void *MyThread_c::run(mDvd_param_c *dvd) {
    dvd->mainLoop();
    return nullptr;
}

/** 802eee40 */
MyThread_c::MyThread_c(int priority, EGG::Heap *heap) : EGG::Thread(0x4000, 0, priority, heap) {}

/** 802eee90 */
void *MyThread_c::run() {
    return run(mDvd_param_c::mInstance);
}

/** 802eeea0 */
void create(int priority, EGG::Heap *commandHeap, EGG::Heap *archiveHeap, EGG::Heap *threadHeap) {
    sCommandHeap = commandHeap;
    sArchiveHeap = archiveHeap;
    mDvd::MyThread_c *thread = new (threadHeap, 0x04) mDvd::MyThread_c(priority, threadHeap);
    sThread = thread;
    sOsThread = thread->getMyOsThread();
    if (mHeap::g_assertHeap != nullptr) {
        thread->setThreadHeap((EGG::Heap *)mHeap::g_assertHeap);
    }
    OSResumeThread(sThread->getMyOsThread());
}

/** 802eef30 */
void *loadToMainRAM(int entryNum, char *dst, EGG::Heap *heap, EGG::DvdRipper::EAllocDirection allocDir, u32 offset,
        u32 *p6, u32 *p7, u32 decompressorType) {
    // TODO

    if (decompressorType == 0) {
    } else {
        extern EGG::DvdFile DvdFile;
        if (mDvd::isRegistered == false) {
            static EGG::DvdFile DvdFile;

            isRegistered = true;
        }
    }
}

int ConvertPathToEntrynum(const char *path) {
    return ::ConvertPathToEntrynum(path, nullptr);
}

u32 IsExistPath(const char *path) {
    u32 entry = ConvertPathToEntrynum(path);
    // TODO fake match
    return (((-entry - 1) | (entry + 1)) >> 0x1f);
}

} // namespace mDvd

mDvd_param_c *mDvd_param_c::mInstance;

/** 802ef0d0 */
mDvd_command_c::~mDvd_command_c() {
    waitDone();
}

/** 802ef130 */
void *mDvd_command_c::operator new(size_t size) {
    return EGG::Heap::alloc(size, -4, mDvd::sCommandHeap);
}

void mDvd_command_c::operator delete(void *ptr) {
    EGG::Heap::free(ptr, mDvd::sCommandHeap);
}

/** 802ef150 */
void mDvd_command_c::doClear() {
    onComplete();
}

/** 802ef170 */
void mDvd_command_c::done() {}

void mDvd_command_c::waitDone() {
    mStatus = 1;
    OSWakeupThread(&mDvd_param_c::mInstance->mThreadQueue2);
}

/** 802ef190 */
void mDvd_command_c::waitUntilDone() {
    while (mStatus == 0) {
        OSSleepThread(&mDvd_param_c::mInstance->mThreadQueue2);
    }
}

/** 802ef1e0 */
mDvd_param_c::mDvd_param_c() {
    mFirstRequest = nullptr;
    mLastRequest = nullptr;
    OSInitMutex(&mMutex);
    OSInitThreadQueue(&mThreadQueue1);
    OSInitThreadQueue(&mThreadQueue2);
    mDvd_param_c::mInstance = this;
}

/** 802ef230 */
void mDvd_param_c::kick() {
    OSWakeupThread(&mThreadQueue1);
}

/** 802ef240 */
void mDvd_param_c::waitForKick() {
    OSSleepThread(&mThreadQueue1);
}

/** 802ef250 */
mDvd_command_c *mDvd_param_c::getFirstCommand() {
    return mFirstRequest;
}

/** 802ef260 */
void mDvd_param_c::addCommand(mDvd_command_c *cmd) {
    OSThread *dvdThread = mDvd::getOSThread();
    OSThread *currThread = OSGetCurrentThread();
    if (currThread == dvdThread) {
        cmd->execute();
    } else {
        OSLockMutex(&mMutex);
        cmd->mNext = nullptr;
        if (mLastRequest == nullptr) {
            mLastRequest = cmd;
            mFirstRequest = cmd;
        } else {
            mLastRequest->mNext = cmd;
            mLastRequest = cmd;
        }
        OSUnlockMutex(&mMutex);
        kick();
    }
}

/** 802ef310 */
void mDvd_param_c::removeCommand(mDvd_command_c *toRemove) {
    OSLockMutex(&mMutex);
    mDvd_command_c *prev = nullptr;

    for (mDvd_command_c *cmd = mFirstRequest; cmd != nullptr; cmd = cmd->mNext) {
        if (cmd == toRemove) {
            if (prev != nullptr) {
                prev->mNext = cmd->mNext;
                if (mLastRequest == cmd) {
                    mLastRequest = prev;
                }
            } else {
                if (mFirstRequest == mLastRequest) {
                    mFirstRequest = nullptr;
                    mLastRequest = nullptr;
                } else {
                    mFirstRequest = cmd->mNext;
                }
            }
        }

        prev = cmd;
    }

    OSUnlockMutex(&mMutex);
    kick();
}

u32 mDvd_param_c::executeCB(mDvd_command_c **cmd) {
    (*cmd)->execute();
    return 0;
}

/** 802ef410 */
void mDvd_param_c::mainLoop() {
    mDvd_command_c *cmd;
    while (true) {
        waitForKick();
        while ((cmd = getFirstCommand()) != nullptr) {
            removeCommand(cmd);
            executeCB(&cmd);
        }
    }
}

/** 802ef460 */
mDvd_command_c::mDvd_command_c() {
    mNext = nullptr;
    mStatus = 0;
}

/** 802ef480 */
extern "C" void fn_802EF480(mDvd_command_c *cmd) {
    delete cmd;
}

/** 802ef4a0 */
void mDvd_command_c::destroy(mDvd_command_c **cmd) {
    if (cmd != nullptr && *cmd != nullptr) {
        (*cmd)->doClear();
        // TODO
        delete *cmd;
        *cmd = nullptr;
    }
}

/** 802ef500 */
mDvdCommandReadCallback_c::~mDvdCommandReadCallback_c() {}

/** 802ef560 */
mDvdCommandReadCallback_c::mDvdCommandReadCallback_c(dvdReadCallback cb, void *cbData) {
    mCallback = cb;
    mCallbackData = cbData;
    mDataPtr = nullptr;
}

/* 802ef5d0 */
mDvdCommandReadCallback_c *mDvdCommandReadCallback_c::create(dvdReadCallback cb, void *cbData) {
    // TODO instshuffle
    mDvdCommandReadCallback_c *cmd = new mDvdCommandReadCallback_c(cb, cbData);
    if (cmd != nullptr) {
        mDvd_param_c::mInstance->addCommand(cmd);
    }
    return cmd;
}

/** 802ef650 */
extern "C" void fn_802EF650() {}

/** 802ef680 */
u32 mDvdCommandReadCallback_c::execute() {
    mDataPtr = (mCallback)(mCallbackData);
    waitDone();
    return (bool)mDataPtr;
}

/** 802ef6e0 */
mDvd_mountMemArchive_c::~mDvd_mountMemArchive_c() {}

/** 802ef740 */
mDvd_mountMemArchive_c::mDvd_mountMemArchive_c(int mountDirection) {
    if (mountDirection == 0) {
        mountDirection = mDvd::g_mountDirection;
    }
    mMountDirection = mountDirection;
    mCompressionType = 0;
    mEntryNum = -1;
    mDataPtr = nullptr;
    mHeap = nullptr;
    field_0x18 = 0;
}

/** 802ef7c0 */
int findPathWithCompressedExtension(const char *name, u8 *outType) {
    return 0; // TODO
}

/** 802ef930 */
void setAutoStreamDecomp(bool arg) {
    mDvd::isAutoStreamDecomp = arg;
}

/** 802ef940 */
bool getAutoStreamDecomp() {
    return mDvd::isAutoStreamDecomp;
}

// TODO maybe weak
/** 802ef950 */
int ConvertPathToEntrynum(const char *path, u8 *outType) {
    return 0; // TODO
}

// TODO thunk
/** 802ef950 */
int ConvertPathToEntrynum_Thunk(const char *path, u8 *outType) {
    return ConvertPathToEntrynum(path, outType);
}

/** 802ef9e0 */
mDvd_mountMemArchive_c *mDvd_mountMemArchive_c::create(const char *path, u8 mountDirection, EGG::Heap *heap) {
    u8 type;
    mDvd_mountMemArchive_c *cmd = nullptr;
    int entryNum = ConvertPathToEntrynum_Thunk(path, &type);
    if (entryNum != -1) {
        cmd = new mDvd_mountMemArchive_c(mountDirection);
        if (cmd != nullptr) {
            cmd->mEntryNum = entryNum;
            cmd->mHeap = heap;
            cmd->mCompressionType = type;
            mDvd_param_c::mInstance->addCommand(cmd);
        }
    }

    return cmd;
}

/** 802efa80 */
u32 mDvd_mountMemArchive_c::execute() {
    void *data;
    int align;
    EGG::DvdRipper::EAllocDirection allocDirection;
    EGG::Archive *archive;
    EGG::Heap *heap;

    heap = mHeap != nullptr ? mHeap : mDvd::sArchiveHeap;
    archive = nullptr;

    allocDirection = mMountDirection == 1 ? EGG::DvdRipper::ALLOC_DIR_TOP : EGG::DvdRipper::ALLOC_DIR_BOTTOM;
    data = mDvd::loadToMainRAM(mEntryNum, nullptr, heap, allocDirection, 0, (u32 *)field_0x18, 0, mCompressionType);
    if (data != nullptr) {
        align = -4;
        if (mMountDirection == 1) {
            align = 4;
        }
        archive = EGG::Archive::mount(data, heap, align);
    }

    if (archive == nullptr) {
        if (data != nullptr) {
            delete data;
        }
        field_0x18 = 0;
    } else {
        mDataPtr = archive;
    }
    waitDone();
    return (bool)mDataPtr;
}

/** 802efb80 */
void *mDvd_mountMemArchive_c::getArcBinary() {
    void *result = nullptr;
    if (mDataPtr != nullptr) {
        result = mDataPtr->mHandle.header;
    }
    return result;
}

/** 802efba0 */
void mDvd_mountMemArchive_c::onComplete() {
    if (mDataPtr != nullptr) {
        void *data = mDataPtr->mHandle.header;
        mDataPtr->unmount();
        mDataPtr = nullptr;
        if (data != nullptr) {
            EGG::Heap::free(data, nullptr);
        }
    }
}

/** 802efc10 */
mDvd_toMainRam_base_c::mDvd_toMainRam_base_c(int mountDirection) {
    if (mountDirection == 0) {
        mountDirection = mDvd::g_mountDirection;
    }
    mMountDirection = mountDirection;
    mDataPtr = nullptr;
    field_0x10 = 0;
    field_0x14 = 0;
    mHeap = nullptr;
}

/** 802efc80 */
mDvd_toMainRam_base_c::~mDvd_toMainRam_base_c() {}

/** 802efce0 */
mDvd_toMainRam_arc_c::~mDvd_toMainRam_arc_c() {}

/** 802efd40 */
mDvd_toMainRam_arc_c::mDvd_toMainRam_arc_c(EGG::Archive *arc, int entryNum, int mountDirection)
    : mDvd_toMainRam_base_c(mountDirection), mArcPtr(arc) {
    mEntryNum = entryNum;
}

/** 802efda0 */
mDvd_toMainRam_arc_c *mDvd_toMainRam_arc_c::makeRequest(EGG::Archive *arc, int entryNum, int mountDirection,
        EGG::Heap *heap) {
    mDvd_toMainRam_arc_c *cmd = new mDvd_toMainRam_arc_c(arc, entryNum, mountDirection);
    if (cmd != nullptr) {
        cmd->mHeap = heap;
        mDvd_param_c::mInstance->addCommand(cmd);
    }
    return cmd;
}

/** 802efe20 */
mDvd_toMainRam_arc_c *create(EGG::Archive *arc, const char *path, int mountDirection, EGG::Heap *heap) {
    int entryNum = arc->convertPathToEntryID(path);
    mDvd_toMainRam_arc_c *cmd = nullptr;
    if (entryNum != -1) {
        cmd = mDvd_toMainRam_arc_c::makeRequest(arc, entryNum, mountDirection, heap);
    }
    return cmd;
}

/** 802efe90 */
extern "C" void fn_802EFE90() {
    // TODO
}

/** 802efec0 */
u32 mDvd_toMainRam_arc_c::execute() {
    EGG::Heap *heap = mHeap != nullptr ? mHeap : mDvd::sArchiveHeap;
    mDataPtr = mArcPtr->getFileFast(mEntryNum, heap, mMountDirection == 1 ? 0x20 : -0x20);
    waitDone();
    return (bool)mDataPtr;
}

/** 802eff40 */
mDvd_toMainRam_normal_c::mDvd_toMainRam_normal_c(int mountDirection) : mDvd_toMainRam_base_c(mountDirection) {
    mCompressionType2 = 0;
    mEntryNum = -1;
}

/** 802eff90 */
mDvd_toMainRam_normal_c *mDvd_toMainRam_normal_c::create(const char *path, int mountDirection, EGG::Heap *heap) {
    u8 type;
    int entryNum = ConvertPathToEntrynum(path, &type);
    mDvd_toMainRam_normal_c *cmd;
    if (entryNum == -1) {
        cmd = nullptr;
    } else {
        cmd = new mDvd_toMainRam_normal_c(mountDirection);
        if (cmd != nullptr) {
            cmd->mEntryNum = entryNum;
            cmd->mHeap = heap;
            cmd->mCompressionType2 = type;
            mDvd_param_c::mInstance->addCommand(cmd);
        }
    }
    return cmd;
}

/** 802f0030 */
extern "C" void fn_802F0030() {
    // TODO
}

/** 802f0060 */
void mDvd_toMainRam_normal_c::create2(mDvd_toMainRam_normal_c **cmd, const char *path, int mountDirection,
        EGG::Heap *heap) {
    if (*cmd == nullptr) {
        *cmd = mDvd_toMainRam_normal_c::create(path, mountDirection, heap);
    }
}

/** 802f00b0 */
mDvd_toMainRam_normal_c::~mDvd_toMainRam_normal_c() {}

/** 802f0110 */
u32 mDvd_toMainRam_normal_c::execute() {
    // TODO
    u32 p6;
    u32 p7;
    EGG::Heap *heap = mHeap != nullptr ? mHeap : mDvd::sArchiveHeap;
    EGG::DvdRipper::EAllocDirection allocDirection = mMountDirection == 1 ? EGG::DvdRipper::ALLOC_DIR_TOP : EGG::DvdRipper::ALLOC_DIR_BOTTOM;
    mDataPtr = mDvd::loadToMainRAM(mEntryNum, 0, heap, allocDirection, 0, &p6, &p7, mCompressionType2);
    if (mDataPtr != nullptr) {
        field_0x10 = p6;
        field_0x14 = p7;
    }
    waitDone();
    return (bool)mDataPtr;
}

/** 802f01b0 */
void mDvd_toMainRam_normal_c::onComplete() {
    if (mDataPtr == nullptr) {
        return;
    }
    EGG::Heap::free(mDataPtr, nullptr);
}
