#ifndef NW4R_SND_AXFX_IMPL_H
#define NW4R_SND_AXFX_IMPL_H
#include "nw4r/types_nw4r.h"
#include "rvl/AXFX.h" // IWYU pragma: export
#include "rvl/MEM.h"  // IWYU pragma: export

namespace nw4r {
namespace snd {
namespace detail {

struct AxfxImpl {
    MEMiHeapHead *mHeap; // at 0x4
    u32 mAllocCount;     // at 0x8

    static const u32 HEAP_SIZE_MIN = MEM_FRM_HEAP_MIN_SIZE + 32;

    AxfxImpl() : mHeap(NULL), mAllocCount(0) {}

    bool CreateHeap(void *pBuffer, u32 size);
    void DestroyHeap();

    u32 GetHeapTotalSize() {
        if (mHeap == NULL) {
            return 0;
        }

        return MEMGetHeapTotalSize(mHeap);
    }

    void HookAlloc(AXFXAllocHook *pAllocHook, AXFXFreeHook *pFreeHook);
    u32 RestoreAlloc(AXFXAllocHook allocHook, AXFXFreeHook freeHook);

    static void *Alloc(u32 size);
    static void Free(void *pBlock);

    static AxfxImpl *mCurrentFx;
    static u32 mAllocatedSize;
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
