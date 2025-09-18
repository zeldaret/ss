#include "m/m_dvd.h"

#include "egg/core/eggArchive.h"
#include "egg/core/eggDvdFile.h"
#include "egg/core/eggDvdRipper.h"
#include "egg/core/eggHeap.h"
#include "egg/core/eggStreamDecomp.h"
#include "m/m_heap.h"
#include "string.h"

#include "rvl/DVD.h" // IWYU pragma: export

static int ConvertPathToEntrynum(const char *path, u8 *outType);

namespace mDvd {

TUncompressInfo_c<EGG::StreamDecompSZS> s_UncompressInfoSZS(3, ".szs");
TUncompressInfo_c<EGG::StreamDecompLZ> s_UncompressInfoLZ(5, ".LZ");
TUncompressInfo_c<EGG::StreamDecompLH> s_UncompressInfoLH(7, ".LH");
TUncompressInfo_c<EGG::StreamDecompLRC> s_UncompressInfoLRC(8, ".LRC");
TUncompressInfo_c<EGG::StreamDecompRL> s_UncompressInfoRL(4, ".RL");

// TODO This the space within which the actual decompressors
// are allocated.
char likely_align[0x7A04];
char more_empty_space[0x1638];
char decompressor_alloc_space[0x9930];

mDvd_param_c l_param;

// unofficial
TUncompressInfo_Base_c *decompressorPtrs[1] = {
    &s_UncompressInfoLZ,
};

u8 g_mountDirection = 1;
int maxChunkSize = 0x10000;

void *somePtr;
u32 someNumber;
TUncompressInfo_Base_c **compressors_ptr;
TUncompressInfo_Base_c **compressors_last;
// official
OSThread *l_OSThread;
mDvd::MyThread_c *l_MyThread;
EGG::Heap *l_CommandHeap;
EGG::Heap *l_ArchiveHeap;
bool l_IsAutoStreamDecomp;

static void unk_setDecompressorPtrs(TUncompressInfo_Base_c **ptr, TUncompressInfo_Base_c **last) {
    compressors_ptr = ptr;
    compressors_last = last;
}

void unk_initDecompressors() {
    unk_setDecompressorPtrs(decompressorPtrs, &decompressorPtrs[1]);
    somePtr = decompressor_alloc_space;
    someNumber = 0x24;
}

static TUncompressInfo_Base_c *findUncompressInfo(u8 type) {
    TUncompressInfo_Base_c **ptr = compressors_ptr;
    while (ptr != compressors_last) {
        if ((*ptr)->mType == type) {
            return *ptr;
        }
        ptr++;
    }
    return nullptr;
}

static EGG::StreamDecomp *newUncompressObj(u8 type) {
    TUncompressInfo_Base_c *factory = findUncompressInfo(type);
    if (factory != nullptr) {
        return factory->Construct();
    } else {
        return nullptr;
    }
}

static void deleteUncompressObj(u8 type) {
    TUncompressInfo_Base_c *factory = findUncompressInfo(type);
    if (factory != nullptr) {
        factory->Destruct();
    }
}

OSThread *getOSThread() {
    return l_OSThread;
}

EGG::Heap *getArchiveHeap() {
    return l_ArchiveHeap;
}

void *MyThread_c::run(mDvd_param_c *dvd) {
    dvd->mainLoop();
    return nullptr;
}

MyThread_c::MyThread_c(int priority, EGG::Heap *heap) : EGG::Thread(0x4000, 0, priority, heap) {}

void *MyThread_c::run() {
    return run(mDvd_param_c::mInstance);
}

void create(int priority, EGG::Heap *commandHeap, EGG::Heap *archiveHeap, EGG::Heap *threadHeap) {
    l_CommandHeap = commandHeap;
    l_ArchiveHeap = archiveHeap;
    mDvd::MyThread_c *thread = new (threadHeap, 0x04) mDvd::MyThread_c(priority, threadHeap);
    l_MyThread = thread;
    l_OSThread = thread->getMyOsThread();
    if (mHeap::g_assertHeap != nullptr) {
        thread->setThreadHeap((EGG::Heap *)mHeap::g_assertHeap);
    }
    OSResumeThread(l_MyThread->getMyOsThread());
}

static void *loadToMainRAM(
    int entryNum, char *dst, EGG::Heap *heap, EGG::DvdRipper::EAllocDirection allocDir, s32 offset, u32 *outAmountRead,
    u32 *outFileSize, u32 decompressorType
) {
    void *result;
    u32 amountRead = 0;
    u32 fileSize = 0;

    if (decompressorType != 0) {
        EGG::StreamDecomp *decomp = newUncompressObj(decompressorType);
        static EGG::DvdFile DvdFile;
        DvdFile.open(entryNum);

        result = EGG::DvdRipper::loadToMainRAMDecomp(
            &DvdFile, decomp, (u8 *)dst, heap, allocDir, offset, 0, maxChunkSize, nullptr, nullptr
        );

        deleteUncompressObj(decompressorType);
        u32 size = DvdFile.mFileInfo.size;
        DvdFile.close();
        if (result != nullptr) {
            u32 b = EGG::ExpHeap::getSizeForMBlock(result);
            DCStoreRangeNoSync(result, b);
            fileSize = size;
            amountRead = size;
        }
    } else {
        result = EGG::DvdRipper::loadToMainRAM(entryNum, (u8 *)dst, heap, allocDir, offset, &amountRead, &fileSize);
    }

    if (outAmountRead != nullptr) {
        *outAmountRead = amountRead;
    }
    if (outFileSize != nullptr) {
        *outFileSize = fileSize;
    }
    return result;
}

static int ConvertPathToEntrynum(const char *path, u8 *);

int ConvertPathToEntrynum(const char *path) {
    return mDvd::ConvertPathToEntrynum(path, nullptr);
}

u32 IsExistPath(const char *path) {
    u32 entry = ConvertPathToEntrynum(path);
    return entry != -1;
}

} // namespace mDvd

mDvd_param_c *mDvd_param_c::mInstance;

mDvd_command_c::~mDvd_command_c() {
    waitUntilDone();
}

void *mDvd_command_c::operator new(size_t size) {
    return EGG::Heap::alloc(size, -4, mDvd::l_CommandHeap);
}

void mDvd_command_c::operator delete(void *ptr) {
    EGG::Heap::free(ptr, mDvd::l_CommandHeap);
}

void mDvd_command_c::done() {
    doClear();
}

void mDvd_command_c::doClear() {}

void mDvd_command_c::waitDone() {
    mStatus = 1;
    OSWakeupThread(&mDvd_param_c::mInstance->mThreadQueue2);
}

void mDvd_command_c::waitUntilDone() {
    while (mStatus == 0) {
        OSSleepThread(&mDvd_param_c::mInstance->mThreadQueue2);
    }
}

mDvd_param_c::mDvd_param_c() {
    mFirstRequest = nullptr;
    mLastRequest = nullptr;
    OSInitMutex(&mMutex);
    OSInitThreadQueue(&mThreadQueue1);
    OSInitThreadQueue(&mThreadQueue2);
    mDvd_param_c::mInstance = this;
}

void mDvd_param_c::kick() {
    OSWakeupThread(&mThreadQueue1);
}

void mDvd_param_c::waitForKick() {
    OSSleepThread(&mThreadQueue1);
}

mDvd_command_c *mDvd_param_c::getFirstCommand() {
    return mFirstRequest;
}

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

mDvd_command_c::mDvd_command_c() {
    mNext = nullptr;
    mStatus = 0;
}

void mDvd_command_c::do_delete() {
    delete this;
}

void mDvd_command_c::destroy(mDvd_command_c **cmd) {
    // TODO fake match, this looks like an inlined dtor
    if (cmd != nullptr && cmd != nullptr && *cmd != nullptr) {
        (*cmd)->done();
        (*cmd)->do_delete();
        *cmd = nullptr;
    }
}

mDvd_callback_c::~mDvd_callback_c() {}

mDvd_callback_c::mDvd_callback_c(dvdReadCallback cb, void *cbData) {
    mCallback = cb;
    mCallbackData = cbData;
    mCallbackResult = 0;
}

mDvd_callback_c *mDvd_callback_c::create(dvdReadCallback cb, void *cbData) {
    mDvd_callback_c *cmd = new mDvd_callback_c(cb, cbData);
    if (cmd != nullptr) {
        mDvd_param_c::mInstance->addCommand(cmd);
    }
    return cmd;
}

mDvd_callback_c *mDvd_callback_c::createOrDie(dvdReadCallback cb, void *cbData) {
    mDvd_callback_c *cmd = mDvd_callback_c::create(cb, cbData);
    while (!cmd) {}
    return cmd;
}

u32 mDvd_callback_c::execute() {
    mCallbackResult = (mCallback)(mCallbackData);
    waitDone();
    return (bool)mCallbackResult;
}

mDvd_mountMemArchive_c::~mDvd_mountMemArchive_c() {}

mDvd_mountMemArchive_c::mDvd_mountMemArchive_c(int mountDirection) {
    if (mountDirection == 0) {
        mountDirection = mDvd::g_mountDirection;
    }
    mMountDirection = mountDirection;
    mCompressionType = 0;
    mEntryNum = -1;
    mDataPtr = nullptr;
    mHeap = nullptr;
    mAmountRead = 0;
}

namespace mDvd {

static int ConvertPathToEntrynumASD_(const char *name, u8 *outType) {
    u8 type;
    char buf[256];
    int num;
    char *end;
    int size;

    type = 0;
    buf[255] = '\0';
    num = -1;

    strncpy(buf, name, sizeof(buf));
    if (buf[255] == '\0') {
        end = buf + strlen(buf);
        size = sizeof(buf) - (end - buf);
        // Append the compressor extension and try to find a compressed version
        for (mDvd::TUncompressInfo_Base_c **ptr = mDvd::compressors_ptr; ptr != mDvd::compressors_last; ptr++) {
            strncpy(end, (*ptr)->mExtension, size);
            num = DVDConvertPathToEntrynum(buf);
            if (num != -1) {
                type = (*ptr)->mType;
                goto out;
            }
        }
        *end = '\0';
        // No compressed version found
        end = strrchr(buf, '/');
        if (end == nullptr) {
            end = buf;
        }
        end = strrchr(end, '.');
        if (end != nullptr) {
            size = sizeof(buf) - (end - buf);
            // Append the compressor extension and try to find a compressed version
            for (mDvd::TUncompressInfo_Base_c **ptr = mDvd::compressors_ptr; ptr != mDvd::compressors_last; ptr++) {
                strncpy(end, (*ptr)->mExtension, size);
                num = DVDConvertPathToEntrynum(buf);
                if (num != -1) {
                    type = (*ptr)->mType;
                    goto out;
                }
            }
            *end = '\0';
        }
    }

out:
    if (num != -1 && outType != nullptr) {
        *outType = type;
    }
    return num;
}

void setAutoStreamDecomp(bool arg) {
    mDvd::l_IsAutoStreamDecomp = arg;
}

bool isAutoStreamDecomp() {
    return mDvd::l_IsAutoStreamDecomp;
}

static int ConvertPathToEntrynum(const char *path, u8 *outType) {
    int num = DVDConvertPathToEntrynum(path);
    if (num != -1) {
        if (outType != nullptr) {
            *outType = 0;
        }
    } else if (mDvd::isAutoStreamDecomp()) {
        num = mDvd::ConvertPathToEntrynumASD_(path, outType);
    }
    return num;
}

static int ConvertPathToEntrynumWithWarning(const char *path, u8 *outType) {
    return ConvertPathToEntrynum(path, outType);
}

} // namespace mDvd

mDvd_mountMemArchive_c *mDvd_mountMemArchive_c::create(const char *path, u8 mountDirection, EGG::Heap *heap) {
    u8 type;
    mDvd_mountMemArchive_c *cmd = nullptr;
    int entryNum = mDvd::ConvertPathToEntrynumWithWarning(path, &type);
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

u32 mDvd_mountMemArchive_c::execute() {
    EGG::DvdRipper::EAllocDirection allocDirection;
    EGG::Archive *archive;
    void *data;
    EGG::Heap *heap;

    heap = mHeap != nullptr ? mHeap : mDvd::l_ArchiveHeap;
    archive = nullptr;

    allocDirection = mMountDirection == 1 ? EGG::DvdRipper::ALLOC_DIR_TOP : EGG::DvdRipper::ALLOC_DIR_BOTTOM;
    data = mDvd::loadToMainRAM(mEntryNum, nullptr, heap, allocDirection, 0, &mAmountRead, 0, mCompressionType);
    if (data != nullptr) {
        archive = EGG::Archive::mount(data, heap, mMountDirection == 1 ? 4 : -4);
    }

    if (archive != nullptr) {
        mDataPtr = archive;
    } else {
        if (data != nullptr) {
            // TODO so far data seems to have been passed around as void *
            // but this silences a compiler warning. Unsure about the actual type.
            delete (char *)data;
        }
        mAmountRead = 0;
    }
    waitDone();
    return (bool)mDataPtr;
}

void *mDvd_mountMemArchive_c::getArcBinary() {
    void *result = nullptr;
    if (mDataPtr != nullptr) {
        result = mDataPtr->mHandle.header;
    }
    return result;
}

void mDvd_mountMemArchive_c::doClear() {
    if (mDataPtr != nullptr) {
        void *data = mDataPtr->mHandle.header;
        mDataPtr->unmount();
        mDataPtr = nullptr;
        if (data != nullptr) {
            EGG::Heap::free(data, nullptr);
        }
    }
}

mDvd_toMainRam_base_c::mDvd_toMainRam_base_c(int mountDirection) {
    if (mountDirection == 0) {
        mountDirection = mDvd::g_mountDirection;
    }
    mMountDirection = mountDirection;
    mDataPtr = nullptr;
    mAmountRead = 0;
    mFileSize = 0;
    mHeap = nullptr;
}

mDvd_toMainRam_base_c::~mDvd_toMainRam_base_c() {}

mDvd_toMainRam_arc_c::~mDvd_toMainRam_arc_c() {}

mDvd_toMainRam_arc_c::mDvd_toMainRam_arc_c(EGG::Archive *arc, int entryNum, int mountDirection)
    : mDvd_toMainRam_base_c(mountDirection), mArcPtr(arc) {
    mEntryNum = entryNum;
}

mDvd_toMainRam_arc_c *
mDvd_toMainRam_arc_c::makeRequest(EGG::Archive *arc, int entryNum, int mountDirection, EGG::Heap *heap) {
    mDvd_toMainRam_arc_c *cmd = new mDvd_toMainRam_arc_c(arc, entryNum, mountDirection);
    if (cmd != nullptr) {
        cmd->mHeap = heap;
        mDvd_param_c::mInstance->addCommand(cmd);
    }
    return cmd;
}

mDvd_toMainRam_arc_c *
mDvd_toMainRam_arc_c::create(EGG::Archive *arc, const char *path, int mountDirection, EGG::Heap *heap) {
    int entryNum = arc->convertPathToEntryID(path);
    mDvd_toMainRam_arc_c *cmd = nullptr;
    if (entryNum != -1) {
        cmd = mDvd_toMainRam_arc_c::makeRequest(arc, entryNum, mountDirection, heap);
    }
    return cmd;
}

mDvd_toMainRam_arc_c *
mDvd_toMainRam_arc_c::createOrDie(EGG::Archive *arc, const char *path, int mountDirection, EGG::Heap *heap) {
    mDvd_toMainRam_arc_c *cmd = mDvd_toMainRam_arc_c::create(arc, path, mountDirection, heap);
    while (!cmd) {}
    return cmd;
}

u32 mDvd_toMainRam_arc_c::execute() {
    EGG::Heap *heap = mHeap != nullptr ? mHeap : mDvd::l_ArchiveHeap;
    mDataPtr = mArcPtr->getFileFast(mEntryNum, heap, mMountDirection == 1 ? 0x20 : -0x20);
    waitDone();
    return (bool)mDataPtr;
}

mDvd_toMainRam_normal_c::mDvd_toMainRam_normal_c(int mountDirection) : mDvd_toMainRam_base_c(mountDirection) {
    mCompressionType2 = 0;
    mEntryNum = -1;
}

mDvd_toMainRam_normal_c *mDvd_toMainRam_normal_c::create(const char *path, int mountDirection, EGG::Heap *heap) {
    u8 type;
    int entryNum = mDvd::ConvertPathToEntrynumWithWarning(path, &type);
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

mDvd_toMainRam_normal_c *mDvd_toMainRam_normal_c::createOrDie(const char *path, int mountDirection, EGG::Heap *heap) {
    mDvd_toMainRam_normal_c *cmd = mDvd_toMainRam_normal_c::create(path, mountDirection, heap);
    while (!cmd) {}
    return cmd;
}

void mDvd_toMainRam_normal_c::create2(
    mDvd_toMainRam_normal_c **cmd, const char *path, int mountDirection, EGG::Heap *heap
) {
    if (*cmd == nullptr) {
        *cmd = mDvd_toMainRam_normal_c::create(path, mountDirection, heap);
    }
}

mDvd_toMainRam_normal_c::~mDvd_toMainRam_normal_c() {}

u32 mDvd_toMainRam_normal_c::execute() {
    u32 amountRead;
    u32 fileSize;
    EGG::Heap *heap = mHeap != nullptr ? mHeap : mDvd::l_ArchiveHeap;
    EGG::DvdRipper::EAllocDirection allocDirection =
        mMountDirection == 1 ? EGG::DvdRipper::ALLOC_DIR_TOP : EGG::DvdRipper::ALLOC_DIR_BOTTOM;
    mDataPtr = mDvd::loadToMainRAM(mEntryNum, 0, heap, allocDirection, 0, &amountRead, &fileSize, mCompressionType2);
    if (mDataPtr != nullptr) {
        mAmountRead = amountRead;
        mFileSize = fileSize;
    }
    waitDone();
    return (bool)mDataPtr;
}

void mDvd_toMainRam_normal_c::doClear() {
    if (mDataPtr == nullptr) {
        return;
    }
    EGG::Heap::free(mDataPtr, nullptr);
}
