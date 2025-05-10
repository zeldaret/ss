#ifndef NAND_REQUEST_H
#define NAND_REQUEST_H

#include "common.h"
#include "egg/core/eggHeap.h"
#include "egg/core/eggThread.h"
#include "rvl/NAND/nand.h"
#include "rvl/OS/OSMessage.h"
#include "rvl/OS/OSMutex.h"
#include "sized_string.h"
#include "toBeSorted/tlist.h"

struct NandRequest {
    NandRequest() : mStatus(NAND_RESULT_BUSY), bHandled(false) {}
    ~NandRequest() {}
    virtual bool execute() = 0;

    /* 0x04 */ TListNode<NandRequest> mNode;
    /* 0x0C */ NANDResult mStatus;
    /* 0x10 */ bool bHandled;

    static void *operator new(size_t size);
    static void free(NandRequest *);

    NANDResult getStatus() const;
};

struct NandRequestCheck : public NandRequest {
    NandRequestCheck(u32 neededBlocks, u32 neededFiles);

    virtual bool execute() override;

    /* 0x14 */ u32 mNeededBlocks;
    /* 0x18 */ u32 mNeededFiles;
    /* 0x1C */ u32 mResult;
};

struct NandRequestCreate : public NandRequest {
    NandRequestCreate(const char *filePath, u8 perm, u8 attr);

    virtual bool execute() override;

    // cursed offsets
    /* 0x11 */ SizedString<64> mFilePath;
    /* 0x51 */ u8 mPerm;
    /* 0x52 */ u8 mAttr;
};

struct NandRequestWrite : public NandRequest {
    NandRequestWrite(const char *str, void *data, size_t dataSize);

    virtual bool execute() override;

    /* 0x11 */ SizedString<64> mFilePath;
    /* 0x54 */ void *mData;
    /* 0x58 */ size_t mDataSize;
    /* 0x5C */ bool mFailedWrite;
};

struct NandRequestHolderBase {
    NandRequestHolderBase(NandRequest *req) : mpRequest(req) {}
    NandRequest *mpRequest;

    bool isCompleted() const;
    NANDResult getResult() const;
    bool finish();
};

struct NandRequestCheckHolder : public NandRequestHolderBase {
    NandRequestCheckHolder(NandRequestCheck *req) : NandRequestHolderBase(req) {}

    bool check(u32 neededBlocks, u32 neededFiles);
    u32 getCheckResult();
};

struct NandRequestCreateHolder : public NandRequestHolderBase {
    NandRequestCreateHolder(NandRequestCreate *req) : NandRequestHolderBase(req) {}

    bool create(const char *filePath, u8 perm, u8 attr);
};

struct NandRequestWriteHolder : public NandRequestHolderBase {
    NandRequestWriteHolder(NandRequestWrite *req) : NandRequestHolderBase(req) {}
    bool failedWrite();

    bool write(const char *filePath, void *data, size_t dataSize);
};

class NandRequestThread : EGG::Thread {
    friend class NandRequest;

public:
    NandRequestThread(
        int priority, EGG::Heap *commandHeap, void *bufFromGameHeap, size_t bufSize, void *mThreadArg, EGG::Heap *heap
    );
    virtual ~NandRequestThread();

    virtual void *run() override;
    void enqueueRequest(NandRequest *request);
    NandRequest *dequeueRequest();

    static NandRequestThread *create(
        int priority, EGG::Heap *commandHeap, void *bufFromGameHeap, size_t bufSize, void *mThreadArg, EGG::Heap *heap
    );

    static NandRequestThread *GetInstance() {
        return sInstance;
    }

    void *getBuf() {
        return mBufFromGameHeap;
    }

    size_t getBufSize() {
        return mBufSize;
    }

    NandRequestCheck *checkRequest(u32 neededBlocks, u32 neededFiles);
    NandRequestCreate *createRequest(const char *filePath, u8 perm, u8 attr);
    NandRequestWrite *writeRequest(const char *filePath, void *data, size_t dataSize);

private:
    typedef TList<NandRequest, offsetof(NandRequest, mNode)> NandRequestList;
    static NandRequestThread *sInstance;

    bool waitForMessage();
    void sendMessage();

    u8 field_0x4C[0x368 - 0x4C];
    /* 0x368 */ OSMessageQueue mMessageQueue;
    /* 0x388 */ OSMessage mMessageBuffer;
    /* 0x38C */ OSMutex mMutex;
    /* 0x3A4 */ NandRequestList mRequestList;
    /* 0x3B0 */ EGG::Heap *mpCommandHeap;
    /* 0x3B4 */ void *mBufFromGameHeap;
    /* 0x3B8 */ size_t mBufSize;
    /* 0x3BC */ UNKWORD field_0x3BC;
};

#endif
