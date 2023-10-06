#pragma once

#include "egg/core/eggAllocator.h"

class mAllocator_c : public EGG::Allocator {
public:
        mAllocator_c();
        // vtable at 0x10
        /* 0x08 */ virtual ~mAllocator_c();
        /* 0x0C */ virtual void* alloc(u32 size);
        /* 0x10 */ virtual void free(void* block);
        bool attach(EGG::Heap* heap, s32 align);

};
class mHeapAllocator_c : public mAllocator_c {
public:
    mHeapAllocator_c();
    // vtable at 0x10
    /* 0x08 */ virtual ~mHeapAllocator_c();
    /* 0x0C */ // virtual void* alloc(u32 size);  // see mAlloctor::alloc
    /* 0x10 */ // virtual void free(void* block); // see mAlloctor::free
    int replaceWithNewFrmHeap(s32 size, EGG::Heap* newHeap, char* heapName, s32 align, u32 unk);
    int replaceWithNewExpHeap(s32 size, EGG::Heap* newHeap, char* heapName, s32 align, u32 unk);
    void destroyHeap();
    s32 adjustFrmHeap();
    s32 adjustExpHeap();
    s32 createNewTempFrmHeap(s32 size, EGG::Heap* newHeap, char* heapName, s32 align, u32 unk);
    void adjustFrmHeapRestoreCurrent();
    static void* allocOnHeap(s32 size, mHeapAllocator_c* allocator);
};