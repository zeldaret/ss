#include "nw4r/snd/snd_InstancePool.h"
#include "nw4r/ut.h"

namespace nw4r {
namespace snd {
namespace detail {

u32 PoolImpl::CreateImpl(void* pBuffer, u32 size, u32 stride) {
    ut::AutoInterruptLock lock;

    u8* pPtr = static_cast<u8*>(ut::RoundUp(pBuffer, 4));
    stride = ut::RoundUp(stride, 4);

    // Account for the aligned buffer address
    u32 length = (size - ut::GetOffsetFromPtr(pBuffer, pPtr)) / stride;

    for (u32 i = 0; i < length; i++, pPtr += stride) {
        PoolImpl* pHead = reinterpret_cast<PoolImpl*>(pPtr);
        pHead->mNext = mNext;
        mNext = pHead;
    }

    return length;
}

void PoolImpl::DestroyImpl(void* pBuffer, u32 size) {
    ut::AutoInterruptLock lock;

    void* pBegin = pBuffer;
    void* pEnd = static_cast<u8*>(pBegin) + size;

    PoolImpl* pIt = mNext;
    PoolImpl* pPrev = this;

    for (; pIt != NULL; pIt = pIt->mNext) {
        if (pBegin <= pIt && pIt < pEnd) {
            pPrev->mNext = pIt->mNext;
        } else {
            pPrev = pIt;
        }
    }
}

int PoolImpl::CountImpl() const {
    ut::AutoInterruptLock lock;

    int num = 0;

    for (PoolImpl* pIt = mNext; pIt != NULL; pIt = pIt->mNext) {
        num++;
    }

    return num;
}

void* PoolImpl::AllocImpl() {
    ut::AutoInterruptLock lock;

    if (mNext == NULL) {
        return NULL;
    }

    PoolImpl* pHead = mNext;
    mNext = pHead->mNext;

    return pHead;
}

void PoolImpl::FreeImpl(void* pElem) {
    ut::AutoInterruptLock lock;

    PoolImpl* pHead = static_cast<PoolImpl*>(pElem);
    pHead->mNext = mNext;
    mNext = pHead;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
