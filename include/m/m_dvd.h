#ifndef M_DVD_H
#define M_DVD_H

#include <egg/core/eggHeap.h>
#include <egg/core/eggThread.h>
#include <m/m_heap.h>

// TODO these are scattered
int findPathWithCompressedExtension(const char *name, u8 *outType);
int ConvertPathToEntrynum(const char *path, u8 *outType);

// TODO onComplete -> doClear?

namespace mDvd {

extern void *somePtr;

class TUncompressInfo_Base_c {
public:
    TUncompressInfo_Base_c(u8 type, const char *extension) : mType(type), mExtension(extension) {}
    virtual ~TUncompressInfo_Base_c() {}
    virtual EGG::StreamDecomp *Construct() const {
        return nullptr;
    }
    virtual void Destruct() const {}

    u8 mType;
    const char *mExtension;
};

template <typename T>
class TUncompressInfo_c : public TUncompressInfo_Base_c {
public:
    TUncompressInfo_c(u8 type, const char *extension) : TUncompressInfo_Base_c(type, extension) {}
    virtual T *Construct() const {
        return new (somePtr) T();
    }
    virtual void Destruct() const {}
};

TUncompressInfo_Base_c *findUncompressInfo(u8 type);

void create(int priority, EGG::Heap *commandHeap, EGG::Heap *archiveHeap, EGG::Heap *threadHeap);
void *loadToMainRAM(u32 entryNum, char *dst, EGG::Heap *heap, EGG::DvdRipper::EAllocDirection allocDir, u32 offset, u32 *p6, u32 *p7,
        u32 decompressorType);

u32 IsExistPath(const char *path);

OSThread *getOSThread();
EGG::Heap *getArchiveHeap();
} // namespace mDvd

class mDvd_command_c {
public:
    mDvd_command_c();
    virtual ~mDvd_command_c();

    virtual u32 execute() = 0;
    virtual void onComplete();

    void doClear();
    void waitDone();
    void waitUntilDone();
    static void destroy(mDvd_command_c **cmd);

    void *operator new(size_t size);
    void operator delete(void *ptr);

    mDvd_command_c *mNext;
    u8 mStatus;
    u8 mMountDirection;
    u8 mCompressionType;
};

typedef void *(*dvdReadCallback)(void *);

class mDvdCommandReadCallback_c : public mDvd_command_c {
public:
    mDvdCommandReadCallback_c(dvdReadCallback cb, void *cbData);
    virtual ~mDvdCommandReadCallback_c();
    virtual u32 execute() override;

    static mDvdCommandReadCallback_c *create(dvdReadCallback cb, void *cbData);

    dvdReadCallback mCallback;
    void *mCallbackData;
    void *mDataPtr;
};

class mDvd_mountMemArchive_c : public mDvd_command_c {
public:
    mDvd_mountMemArchive_c(int mountDirection);
    virtual ~mDvd_mountMemArchive_c();
    virtual u32 execute();
    virtual void onComplete();

    static mDvd_mountMemArchive_c *create(const char *path, u8 mountDirection, EGG::Heap *heap);
    void *getArcBinary();

    int mEntryNum;
    EGG::Archive *mDataPtr;
    EGG::Heap *mHeap;
    u32 field_0x18;
};

class mDvd_toMainRam_base_c : public mDvd_command_c {
public:
    mDvd_toMainRam_base_c(int mountDirection);
    virtual ~mDvd_toMainRam_base_c();
    virtual u32 execute() = 0;

    void *mDataPtr;
    int field_0x10;
    u32 field_0x14;
    EGG::Heap *mHeap;
};

class mDvd_toMainRam_arc_c : public mDvd_toMainRam_base_c {
public:
    mDvd_toMainRam_arc_c(EGG::Archive *arc, int entryNum, int mountDirection);
    virtual ~mDvd_toMainRam_arc_c();
    virtual u32 execute();

    static mDvd_toMainRam_arc_c *makeRequest(EGG::Archive *arc, int entryNum, int mountDirection, EGG::Heap *heap);
    static mDvd_toMainRam_arc_c *create(EGG::Archive *arc, const char *path, int mountDirection, EGG::Heap *heap);

    EGG::Archive *mArcPtr;
    int mEntryNum;
};

class mDvd_toMainRam_normal_c : public mDvd_toMainRam_base_c {
public:
    mDvd_toMainRam_normal_c(int mountDirection);
    ~mDvd_toMainRam_normal_c();
    virtual u32 execute();
    virtual void onComplete();

    static mDvd_toMainRam_normal_c *create(const char *path, int mountDirection, EGG::Heap *heap);
    static void create2(mDvd_toMainRam_normal_c **cmd, const char *path, int mountDirection, EGG::Heap *heap);

    u8 mCompressionType2;
    int mEntryNum;
};

class mDvd_param_c {
public:
    mDvd_param_c();

    OSMutex mMutex;
    mDvd_command_c *mFirstRequest;
    mDvd_command_c *mLastRequest;
    OSThreadQueue mThreadQueue1;
    OSThreadQueue mThreadQueue2;

    void kick();
    void waitForKick();

    mDvd_command_c *getFirstCommand();
    void addCommand(mDvd_command_c *cmd);
    void removeCommand(mDvd_command_c *cmd);
    static u32 executeCB(mDvd_command_c **cmd);
    void mainLoop();

    static mDvd_param_c *mInstance;
};

namespace mDvd {

class MyThread_c : EGG::Thread {
public:
    MyThread_c(int priority, EGG::Heap *heap);

    virtual void *run() override;

    OSThread *getMyOsThread() {
        return mOSThread;
    }

    void setThreadHeap(EGG::Heap *heap) {
        setThreadCurrentHeap(heap);
    }

    static void *run(mDvd_param_c *dvd);
};
} // namespace mDvd

#endif
