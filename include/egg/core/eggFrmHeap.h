#ifndef EGG_FRM_HEAP_H
#define EGG_FRM_HEAP_H

#include "egg/core/eggHeap.h"
#include <common.h>

namespace EGG {

class FrmHeap : public Heap {
public:
    // vtable at 0x0 | 8056e9d0
    /* vt 0x08 | 80495fe0 */ virtual ~FrmHeap();
    /* vt 0x0C | 80496390 */ virtual eHeapKind getHeapKind() const;
    /* vt 0x14 | 804961c0 */ virtual void *alloc(u32 size, s32 align);
    /* vt 0x18 | 80496250 */ virtual void free(void *block);
    /* vt 0x1C | 80496130 */ virtual void destroy();
    /* vt 0x20 | 804962b0 */ virtual u32 resizeForMBlock(void *block, u32 size);
    /* vt 0x24 | 804962c0 */ virtual u32 getTotalFreeSize();
    /* vt 0x28 | 804962e0 */ virtual u32 getAllocatableSize(s32 align);
    /* vt 0x2C | 804962f0 */ virtual u32 adjust();

public:
    /* 80495fa0 */ FrmHeap(MEMiHeapHead *heapHead);
    /* 80496060 */ static FrmHeap *create(void *block, size_t size, u16 attr);
    /* 804962a0 */ void free(s32 flags);
    /* 80496370 */ void recordState(u32 id); // non official for now
    /* 80496380 */ void freeState(u32 id);   // non official for now
};

} // namespace EGG

#endif
