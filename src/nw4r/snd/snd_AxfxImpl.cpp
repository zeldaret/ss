#include "nw4r/snd/snd_AxfxImpl.h"

#include "nw4r/ut/ut_algorithm.h"

namespace nw4r {
namespace snd {
namespace detail {

AxfxImpl* AxfxImpl::mCurrentFx = NULL;
u32 AxfxImpl::mAllocatedSize = 0;

bool AxfxImpl::CreateHeap(void* pBuffer, u32 size) {
    if (pBuffer == NULL || size == 0)
    {
        mHeap = NULL;
        return false;
    }
    mHeap = MEMCreateFrmHeap(pBuffer, size);
    return mHeap != NULL;
}

void AxfxImpl::DestroyHeap() {
    if (mHeap != NULL) {
        MEMDestroyFrmHeap(mHeap);
        mHeap = NULL;
    }
}

void AxfxImpl::HookAlloc(AXFXAllocHook* pAllocHook, AXFXFreeHook* pFreeHook) {
    AXFXGetHooks(pAllocHook, pFreeHook);
    AXFXSetHooks(Alloc, Free);
    mCurrentFx = this;
    mAllocatedSize = 0;
}

u32 AxfxImpl::RestoreAlloc(AXFXAllocHook allocHook, AXFXFreeHook freeHook) {
    AXFXSetHooks(allocHook, freeHook);
    mCurrentFx = NULL;
    return mAllocatedSize;
}

void* AxfxImpl::Alloc(u32 size) {
    void* pBlock = MEMAllocFromFrmHeap(mCurrentFx->mHeap, size);

    mCurrentFx->mAllocCount++;
    mAllocatedSize += ut::RoundUp(size, 4);

    return pBlock;
}

void AxfxImpl::Free(void* pBlock) {
#pragma unused(pBlock)

    if (mCurrentFx->mAllocCount != 0) {
        mCurrentFx->mAllocCount--;
    }

    if (mCurrentFx->mAllocCount == 0) {
        MEMFreeToFrmHeap(mCurrentFx->mHeap, MEM_FRM_HEAP_FREE_ALL);
    }
}

} // namespace detail
} // namespace snd
} // namespace nw4r
