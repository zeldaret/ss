#ifndef NW4R_SND_AXFX_IMPL_H
#define NW4R_SND_AXFX_IMPL_H
#include <nw4r/types_nw4r.h>
#include <rvl/AXFX.h>
#include <rvl/MEM.h>

namespace nw4r {
namespace snd {
namespace detail {

struct AxfxImpl {
    static void *Alloc(u32 size);
    static void Free(void *block);

    AxfxImpl() : mIsActive(false), mHeap(NULL), mAllocCount(0) {}

    u32 GetHeapTotalSize() {
        if (mHeap == NULL) {
            return 0;
        }

        return MEMGetHeapTotalSize(mHeap);
    }

    bool CreateHeap(void *buffer, u32 size);
    void DestroyHeap();
    void HookAlloc(AXFXAllocHook *allocHook, AXFXFreeHook *freeHook);
    void RestoreAlloc(AXFXAllocHook allocHook, AXFXFreeHook freeHook);

    bool mIsActive;      // at 0x0
    MEMiHeapHead *mHeap; // at 0x4
    u32 mAllocCount;     // at 0x8

    static AxfxImpl *mCurrentFx;
    static u32 mAllocatedSize;
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
