#ifndef EGG_EXP_HEAP_H
#define EGG_EXP_HEAP_H

#include "common.h"
#include "egg/core/eggHeap.h"


namespace EGG {

class ExpHeap : public Heap {
public:
    // vtable at 0x0
    /* vt 0x08 */ virtual ~ExpHeap();
    /* vt 0x0C */ virtual eHeapKind getHeapKind() const override;
    /* vt 0x14 */ virtual void *alloc(u32 size, s32 align) override;
    /* vt 0x18 */ virtual void free(void *block) override;
    /* vt 0x1C */ virtual void destroy() override;
    /* vt 0x20 */ virtual u32 resizeForMBlock(void *block, u32 size) override;
    /* vt 0x24 */ virtual u32 getTotalFreeSize() override;
    /* vt 0x28 */ virtual u32 getAllocatableSize(s32 align) override;
    /* vt 0x2C */ virtual u32 adjust() override;

public:
    ExpHeap(MEMiHeapHead *heapHead);
    static ExpHeap *create(void *block, size_t size, u16 attr);
    static ExpHeap *create(size_t size, Heap *heap, u16 attr);
    void setAllocMode(u16 allocMode);
    static u32 getSizeForMBlock(const void *block);

    // Placement new for ::create
    inline void *operator new(size_t size, void *ptr) {
        return ptr;
    }
};

} // namespace EGG

#endif
