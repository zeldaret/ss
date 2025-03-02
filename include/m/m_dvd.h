#ifndef M_DVD_H
#define M_DVD_H

#include "egg/core/eggArchive.h"
#include "egg/core/eggHeap.h"
#include "egg/core/eggStreamDecomp.h"
#include "egg/core/eggThread.h"
#include "m/m_heap.h"

// Note: names are taken from NSMBW where available but a lot are made up

void unk_initDecompressors();

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

void create(int priority, EGG::Heap *commandHeap, EGG::Heap *archiveHeap, EGG::Heap *threadHeap);
int ConvertPathToEntrynum(const char *path);
u32 IsExistPath(const char *path);

OSThread *getOSThread();
EGG::Heap *getArchiveHeap();
} // namespace mDvd

class mDvd_command_c {
public:
    mDvd_command_c();
    virtual ~mDvd_command_c();

    virtual u32 execute() = 0;
    virtual void doClear();

    void done();
    void waitDone();
    void waitUntilDone();
    static void destroy(mDvd_command_c **cmd);
    void do_delete();

    void *operator new(size_t size);
    void operator delete(void *ptr);

    mDvd_command_c *mNext;
    u8 mStatus;
    u8 mMountDirection;
    u8 mCompressionType;
};

typedef void *(*dvdReadCallback)(void *);

class mDvd_callback_c : public mDvd_command_c {
public:
    mDvd_callback_c(dvdReadCallback cb, void *cbData);
    virtual ~mDvd_callback_c();
    virtual u32 execute() override;

    static mDvd_callback_c *create(dvdReadCallback cb, void *cbData);
    // createOrDie__15mDvd_callback_cFPFPv_PvPv ?
    static mDvd_callback_c *createOrDie(dvdReadCallback cb, void *cbData);

    dvdReadCallback mCallback;
    void *mCallbackData;
    void *mCallbackResult;
};

class mDvd_mountMemArchive_c : public mDvd_command_c {
public:
    mDvd_mountMemArchive_c(int mountDirection);
    virtual ~mDvd_mountMemArchive_c();
    virtual u32 execute() override;
    virtual void doClear() override;

    static mDvd_mountMemArchive_c *create(const char *path, u8 mountDirection, EGG::Heap *heap);
    void *getArcBinary();

    int mEntryNum;
    EGG::Archive *mDataPtr;
    EGG::Heap *mHeap;
    u32 mAmountRead;
};

class mDvd_toMainRam_base_c : public mDvd_command_c {
public:
    mDvd_toMainRam_base_c(int mountDirection);
    virtual ~mDvd_toMainRam_base_c();
    virtual u32 execute() = 0;

    void *mDataPtr;
    int mAmountRead;
    u32 mFileSize;
    EGG::Heap *mHeap;
};

class mDvd_toMainRam_arc_c : public mDvd_toMainRam_base_c {
public:
    mDvd_toMainRam_arc_c(EGG::Archive *arc, int entryNum, int mountDirection);
    virtual ~mDvd_toMainRam_arc_c();
    virtual u32 execute();

    static mDvd_toMainRam_arc_c *makeRequest(EGG::Archive *arc, int entryNum, int mountDirection, EGG::Heap *heap);
    static mDvd_toMainRam_arc_c *create(EGG::Archive *arc, const char *path, int mountDirection, EGG::Heap *heap);
    static mDvd_toMainRam_arc_c *createOrDie(EGG::Archive *arc, const char *path, int mountDirection, EGG::Heap *heap);

    EGG::Archive *mArcPtr;
    int mEntryNum;
};

class mDvd_toMainRam_normal_c : public mDvd_toMainRam_base_c {
public:
    mDvd_toMainRam_normal_c(int mountDirection);
    ~mDvd_toMainRam_normal_c();
    virtual u32 execute();
    virtual void doClear();

    static mDvd_toMainRam_normal_c *create(const char *path, int mountDirection, EGG::Heap *heap);
    static mDvd_toMainRam_normal_c *createOrDie(const char *path, int mountDirection, EGG::Heap *heap);
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
