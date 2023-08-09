#pragma once

#include <lib/egg/core/eggAllocator.h>

class mAllocator : public EGG::Allocator {
public:
        mAllocator();
        // vtable at 0x10
        /* 0x08 */ virtual ~mAllocator();
        /* 0x0C */ virtual void* alloc(u32 size);
        /* 0x10 */ virtual void free(void* block);
        bool attach(EGG::Heap* heap, s32 align);

};
class mHeapAllocator : public mAllocator {
public:
    mHeapAllocator();
    // vtable at 0x10
    /* 0x08 */ virtual ~mHeapAllocator();
    /* 0x0C */ // virtual void* alloc(u32 size);  // see mAlloctor::alloc
    /* 0x10 */ // virtual void free(void* block); // see mAlloctor::free
    int replaceWithNewFrmHeap(s32 size, EGG::Heap* newHeap, char* heapName, s32 align, u32 unk);
    int replaceWithNewExpHeap(s32 size, EGG::Heap* newHeap, char* heapName, s32 align, u32 unk);
    void destroyHeap();
    s32 adjustFrmHeap();
    s32 adjustExpHeap();
    s32 createNewTempFrmHeap(s32 size, EGG::Heap* newHeap, char* heapName, s32 align, u32 unk);
    void adjustFrmHeapRestoreCurrent();
    static void* allocOnHeap(s32 size, mHeapAllocator* allocator);
};