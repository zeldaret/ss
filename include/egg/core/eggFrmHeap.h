#ifndef EGG_FRM_HEAP_H
#define EGG_FRM_HEAP_H

#include "common.h"
#include "egg/core/eggHeap.h"


namespace EGG {

class FrmHeap : public Heap {
public:
    // vtable at 0x0
    virtual ~FrmHeap();
    virtual eHeapKind getHeapKind() const override;
    virtual void *alloc(u32 size, s32 align) override;
    virtual void free(void *block) override;
    virtual void destroy() override;
    virtual u32 resizeForMBlock(void *block, u32 size) override;
    virtual u32 getTotalFreeSize() override;
    virtual u32 getAllocatableSize(s32 align) override;
    virtual u32 adjust() override;

public:
    FrmHeap(MEMiHeapHead *heapHead);
    static FrmHeap *create(void *block, u32 size, u16 attr);
    void free(s32 flags);
    void recordState(u32 id);
    void freeByState(u32 id);
};

} // namespace EGG

#endif
