#include "toBeSorted/nand_request_thread.h"

#include "common.h"
#include "egg/core/eggAssertHeap.h" // IWYU pragma: export
#include "egg/core/eggThread.h"
#include "m/m_heap.h"
#include "rvl/NAND/NANDCheck.h"
#include "rvl/NAND/NANDCore.h"
#include "rvl/NAND/nand.h"
#include "rvl/OS/OSMessage.h"
#include "rvl/OS/OSMutex.h"
#include "rvl/OS/OSThread.h"

#include <cstring>

extern "C" void fn_802F2920(OSThread *, size_t);

NandRequestThread::NandRequestThread(
    int priority, EGG::Heap *commandHeap, void *bufFromGameHeap, size_t bufSize, void *mThreadArg, EGG::Heap *heap
)
    : EGG::Thread(0x4000, 0, priority, heap), mMessageBuffer(nullptr) {
    // TODO suffers from TList ctor
    sInstance = this;

    mpCommandHeap = commandHeap;
    mBufFromGameHeap = bufFromGameHeap;
    mBufSize = bufSize;

    setThreadCurrentHeap(mHeap::g_assertHeap);
    fn_802F2920(mOSThread, bufSize);
    OSInitMessageQueue(&mMessageQueue, &mMessageBuffer, 1);
    OSInitMutex(&mMutex);
    OSResumeThread(mOSThread);
}

NandRequestThread::~NandRequestThread() {
    sInstance = nullptr;
}

void *NandRequestThread::run() {
    NandRequest *req;
    while (waitForMessage()) {
        while (mRequestList.mCount != 0) {
            req = dequeueRequest();
            req->execute();
            req->bHandled = true;
        }
    }
    return nullptr;
}

NandRequestCheck *NandRequestThread::checkRequest(u32 neededBlocks, u32 neededFiles) {
    NandRequestCheck *req = new NandRequestCheck(neededBlocks, neededFiles);
    if (req != nullptr) {
        enqueueRequest(req);
    }
    return req;
}

NandRequestCreate *NandRequestThread::createRequest(const char *filePath, u8 perm, u8 attr) {
    NandRequestCreate *req = new NandRequestCreate(filePath, perm, attr);
    if (req != nullptr) {
        enqueueRequest(req);
    }
    return req;
}

NandRequestWrite *NandRequestThread::writeRequest(const char *filePath, void *data, size_t dataSize) {
    NandRequestWrite *req = new NandRequestWrite(filePath, data, dataSize);
    if (req != nullptr) {
        enqueueRequest(req);
    }
    return req;
}

bool NandRequestThread::waitForMessage() {
    return OSReceiveMessage(&mMessageQueue, nullptr, OS_MSG_PERSISTENT);
}

void NandRequestThread::sendMessage() {
    OSSendMessage(&mMessageQueue, nullptr, 0);
}

void NandRequestThread::enqueueRequest(NandRequest *request) {
    mRequestList.append(request);
    sendMessage();
}

NandRequest *NandRequestThread::dequeueRequest() {
    // TODO TList
    NandRequest *req = mRequestList.GetFirst();

    OSLockMutex(&mMutex);
    mRequestList.RemoveFirst();
    OSUnlockMutex(&mMutex);

    sendMessage();
    return req;
}

NandRequestThread *NandRequestThread::create(
    int priority, EGG::Heap *commandHeap, void *bufFromGameHeap, size_t bufSize, void *mThreadArg, EGG::Heap *heap
) {
    return new (heap, 0x04) NandRequestThread(priority, commandHeap, bufFromGameHeap, bufSize, mThreadArg, heap);
}

void *NandRequest::operator new(size_t size) {
    return NandRequestThread::GetInstance()->mpCommandHeap->alloc(size, -4);
}

void NandRequest::free(NandRequest *ptr) {
    return NandRequestThread::GetInstance()->mpCommandHeap->free(ptr);
}

NANDResult NandRequest::getStatus() const {
    return mStatus;
}

NandRequestCheck::NandRequestCheck(u32 neededBlocks, u32 neededFiles) {
    mResult = 0;
    mNeededBlocks = neededBlocks;
    mNeededFiles = neededFiles;
}

bool NandRequestCheck::execute() {
    mStatus = NANDCheck(mNeededBlocks, mNeededFiles, &mResult);
    return true;
}

NandRequestCreate::NandRequestCreate(const char *filePath, u8 perm, u8 attr) {
    mPerm = perm;
    mAttr = attr;
    mFilePath = filePath;
}

const char *splitComponent(const char *inPath, char *outPath, size_t size) {
    char *end = outPath + size - 1;
    while (true) {
        if (outPath >= end) {
            break;
        }
        char curr = inPath[0];
        char next = inPath[1];
        inPath++;
        *outPath = curr;
        outPath++;
        if (next == '\0' || next == '/') {
            break;
        }
    }

    *outPath = '\0';
    char curr = *inPath;
    if (curr != '/') {
        return inPath;
    }
    return inPath + 1;
}

bool NandRequestCreate::execute() {
    char homeDir[64];
    char pathComponent[64];

    mStatus = NANDGetHomeDir(homeDir);
    if (mStatus != NAND_RESULT_OK) {
        return true;
    }

    const char *ptr = mFilePath;
    if (ptr[0] == '/') {
        ptr = splitComponent(ptr, pathComponent, sizeof(pathComponent));
    } else {
        std::strncpy(pathComponent, homeDir, sizeof(pathComponent));
    }

    mStatus = NANDChangeDir(pathComponent);
    if (mStatus != NAND_RESULT_OK) {
        return true;
    }

    const char *component = splitComponent(ptr, pathComponent, sizeof(pathComponent));
    while (component[0] != '\0') {
        mStatus = NANDCreateDir(pathComponent, mPerm, mAttr);
        if (mStatus != NAND_RESULT_OK && mStatus != NAND_RESULT_EXISTS) {
            return true;
        }
        mStatus = NANDChangeDir(pathComponent);
        if (mStatus != NAND_RESULT_OK) {
            return true;
        }
        component = splitComponent(component, pathComponent, sizeof(pathComponent));
    }

    mStatus = NANDChangeDir(homeDir);
    if (mStatus != NAND_RESULT_OK) {
        return true;
    }

    mStatus = NANDCreate(mFilePath, mPerm, mAttr);
    return mStatus == NAND_RESULT_OK || mStatus == NAND_RESULT_EXISTS;
}

NandRequestWrite::NandRequestWrite(const char *someString, void *data, size_t dataSize) {
    mData = data;
    mDataSize = dataSize;
    mFailedWrite = false;
    mFilePath = someString;
}

bool NandRequestWrite::execute() {
    NANDFileInfo info;
    mStatus = NANDSimpleSafeOpen(
        mFilePath, &info, 2, NandRequestThread::GetInstance()->getBuf(), NandRequestThread::GetInstance()->getBufSize()
    );
    if (mStatus != NAND_RESULT_OK) {
        NANDResult res = NANDSimpleSafeCancel(&info);
        if (res != NAND_RESULT_OK) {
            mStatus = res;
        }
        return true;
    }

    mStatus = NANDWrite(&info, mData, mDataSize);
    if (mStatus == NAND_RESULT_ECC_CRIT || mStatus == NAND_RESULT_AUTHENTICATION) {
        mStatus = NANDSimpleSafeCancel(&info);
        if (mStatus == NAND_RESULT_OK) {
            mFailedWrite = true;
        }
        return true;
    }

    NANDResult res = NANDSimpleSafeClose(&info);
    if (mStatus != mDataSize) {
        mFailedWrite = true;
    } else {
        mStatus = res;
    }

    return true;
}

bool NandRequestHolderBase::isCompleted() const {
    if (mpRequest != nullptr) {
        return mpRequest->bHandled;
    }
    return true;
}

NANDResult NandRequestHolderBase::getResult() const {
    if (mpRequest != nullptr) {
        return mpRequest->getStatus();
    }
    return NAND_RESULT_BUSY;
}

bool NandRequestHolderBase::finish() {
    if (mpRequest == nullptr) {
        return true;
    }

    do {
    } while (!isCompleted());
    NandRequest::free(mpRequest);
    mpRequest = nullptr;

    return true;
}

bool NandRequestCheckHolder::check(u32 neededBlocks, u32 neededFiles) {
    if (mpRequest != nullptr) {
        return false;
    }

    NandRequestCheck *req = NandRequestThread::GetInstance()->checkRequest(neededBlocks, neededFiles);
    if (req == nullptr) {
        return false;
    }
    mpRequest = req;
    return true;
}

u32 NandRequestCheckHolder::getCheckResult() {
    return static_cast<NandRequestCheck *>(mpRequest)->mResult;
}

bool NandRequestCreateHolder::create(const char *filePath, u8 perm, u8 attr) {
    if (mpRequest != nullptr) {
        return false;
    }

    NandRequestCreate *req = NandRequestThread::GetInstance()->createRequest(filePath, perm, attr);
    if (req == nullptr) {
        return false;
    }
    mpRequest = req;
    return true;
}

bool NandRequestWriteHolder::write(const char *filePath, void *data, size_t dataSize) {
    if (mpRequest != nullptr) {
        return false;
    }

    NandRequestWrite *req = NandRequestThread::GetInstance()->writeRequest(filePath, data, dataSize);
    if (req == nullptr) {
        return false;
    }
    mpRequest = req;
    return true;
}

bool NandRequestWriteHolder::failedWrite() {
    return static_cast<NandRequestWrite *>(mpRequest)->mFailedWrite;
}
