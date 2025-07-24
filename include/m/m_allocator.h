#ifndef M_ALLOCATOR_H
#define M_ALLOCATOR_H

#include "egg/core/eggAllocator.h"
#include "m/m_heap.h"

class mAllocator_c : public EGG::Allocator {
public:
    mAllocator_c();
    // vtable at 0x10
    /* 0x08 */ virtual ~mAllocator_c();
    /* 0x0C */ virtual void *alloc(u32 size);
    /* 0x10 */ virtual void free(void *block);
    bool attach(EGG::Heap *heap, int align);
};
class mHeapAllocator_c : public mAllocator_c {
public:
    mHeapAllocator_c();
    // vtable at 0x10
    /* 0x08 */ virtual ~mHeapAllocator_c();
    /* 0x0C */ // virtual void* alloc(u32 size);  // see mAlloctor::alloc
    /* 0x10 */ // virtual void free(void* block); // see mAlloctor::free
    bool createFrmHeap(u32 size, EGG::Heap *newHeap, const char *heapName, u32 align, mHeap::AllocOptBit_t attr);
    bool createExpHeap(u32 size, EGG::Heap *newHeap, const char *heapName, u32 align, mHeap::AllocOptBit_t attr);
    void destroyHeap();
    s32 adjustFrmHeap();
    s32 adjustExpHeap();
    bool createFrmHeapToCurrent(u32 size, EGG::Heap *newHeap, const char *heapName, u32 align, mHeap::AllocOptBit_t attr);
    void adjustFrmHeapRestoreCurrent();
};

void *operator new[](size_t size, mAllocator_c *);

#endif
