#ifndef EGG_ASSERT_HEAP_H
#define EGG_ASSERT_HEAP_H

#include "egg/core/eggHeap.h"
#include <common.h>

namespace EGG {

class AssertHeap : public Heap {
public:
    // vtable at 0x0
    virtual ~AssertHeap();
    virtual eHeapKind getHeapKind() const override;
    virtual void *alloc(u32 size, s32 align) override;
    virtual void free(void *block) override;
    virtual void destroy() override;
    virtual u32 resizeForMBlock(void *block, u32 size) override;
    virtual u32 getTotalFreeSize() override;
    virtual u32 getAllocatableSize(s32 align) override;
    virtual u32 adjust() override;

public:
    AssertHeap(MEMiHeapHead *heapHead);
    static AssertHeap *create(void *block, u32 size);
    static AssertHeap *create(u32 size, Heap *heap);
    static size_t getSize(); // returns 0x7C
};

} // namespace EGG

#endif
