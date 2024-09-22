#ifndef EGG_EXP_HEAP_H
#define EGG_EXP_HEAP_H

#include "egg/core/eggHeap.h"
#include <common.h>

namespace EGG {

class ExpHeap : public Heap {
public:
    // vtable at 0x0 | 8056e980
    /* vt 0x08 | 80495af0 */ virtual ~ExpHeap();
    /* vt 0x0C | 80495f90 */ virtual eHeapKind getHeapKind() const override;
    /* vt 0x14 | 80495d90 */ virtual void *alloc(u32 size, s32 align) override;
    /* vt 0x18 | 80495e50 */ virtual void free(void *block) override;
    /* vt 0x1C | 80495d00 */ virtual void destroy() override;
    /* vt 0x20 | 80495ec0 */ virtual u32 resizeForMBlock(void *block, u32 size) override;
    /* vt 0x24 | 80495ed0 */ virtual u32 getTotalFreeSize() override;
    /* vt 0x28 | 80495ee0 */ virtual u32 getAllocatableSize(s32 align) override;
    /* vt 0x2C | 80495f00 */ virtual u32 adjust() override;

public:
    /* 80495ab0 */ ExpHeap(MEMiHeapHead *heapHead);
    /* 80495b70 */ static ExpHeap *create(void *block, size_t size, u16 attr);
    /* 80495c30 */ static ExpHeap *create(size_t size, Heap *heap, u16 attr);
    /* 80495d00 */ void setGroupID(u16 groupId);
    /* 80495f80 */ static u32 getSizeForMBlock(const void *block);

    // Placement new for ::create
    inline void *operator new(size_t size, void *ptr) {
        return ptr;
    }
};

} // namespace EGG

#endif
