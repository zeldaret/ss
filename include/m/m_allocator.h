#ifndef M_ALLOCATOR_H
#define M_ALLOCATOR_H

#include "egg/core/eggAllocator.h"

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
    bool replaceWithNewFrmHeap(s32 size, EGG::Heap *newHeap, char *heapName, s32 align, u32 unk);
    bool replaceWithNewExpHeap(s32 size, EGG::Heap *newHeap, char *heapName, s32 align, u32 unk);
    void destroyHeap();
    s32 adjustFrmHeap();
    s32 adjustExpHeap();
    bool createNewTempFrmHeap(s32 size, EGG::Heap *newHeap, char *heapName, s32 align, u32 unk);
    void adjustFrmHeapRestoreCurrent();
};

void *operator new[](size_t size, mAllocator_c *);

#endif
