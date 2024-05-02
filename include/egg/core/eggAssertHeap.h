#ifndef EGG_ASSERT_HEAP_H
#define EGG_ASSERT_HEAP_H

#include "egg/core/eggHeap.h"
#include <common.h>

namespace EGG {

class AssertHeap : public Heap {
public:
    // vtable at 0x0 | 8056ea00
    /* vt 0x08 | 804963e0 */ virtual ~AssertHeap();
    /* vt 0x0C | 80496820 */ virtual eHeapKind getHeapKind() const;
    /* vt 0x14 | 804966b0 */ virtual void *alloc(u32 size, s32 align);
    /* vt 0x18 | 80496710 */ virtual void free(void *block);
    /* vt 0x1C | 80496620 */ virtual void destroy();
    /* vt 0x20 | 80496760 */ virtual u32 resizeForMBlock(void *block, u32 size);
    /* vt 0x24 | 80496770 */ virtual u32 getTotalFreeSize();
    /* vt 0x28 | 80496780 */ virtual u32 getAllocatableSize(s32 align);
    /* vt 0x2C | 80496790 */ virtual u32 adjust();

public:
    /* 804963a0 */ AssertHeap(MEMiHeapHead *heapHead);
    /* 80496460 */ static AssertHeap *create(void *block, size_t size, u16 attr);
    /* 80496530 */ static AssertHeap *create(size_t size, Heap *heap);
    /* 80496810 */ static size_t getSize(); // returns 0x7C
};

} // namespace EGG

#endif
