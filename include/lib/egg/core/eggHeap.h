#pragma once

// This file is ported from https://github.com/riidefi/mkw/blob/master/source/egg/core/eggHeap.hpp

#include <types.h>
#include <rvl/MEM.h>
#include <rvl/OS.h>
#include <egg/core/eggAllocator.h>
#include <egg/core/eggDisposer.h>
#include <nw4r/ut/utList.h>
#include <egg/core/eggThread.h>

#ifndef HEAP_PRIVATE
#define HEAP_PRIVATE protected
#endif

namespace EGG {

class ExpHeap;
class Allocator;

struct HeapAllocArg {
  int userArg; // 00
  u32 size;    // 04
  int align;   // 08
  Heap* heap;  // 0C heap to allocate in

  inline HeapAllocArg() : userArg(0), size(0), align(0), heap(nullptr) {}
};
typedef void (*HeapAllocCallback)(HeapAllocArg& arg);

struct HeapErrorArg {
  const char* msg;
  void* userdata;

  inline HeapErrorArg() {}
};

typedef void (*ErrorCallback)(void*);

// Ghidra: Heap
//   size: 0x34
//  .text: [0x804953f0, 0x80495ab0]
class Heap : public Disposer {
public:
  enum eHeapKind {
    HEAP_KIND_NONE = 0,
    HEAP_KIND_EXPANDED,
    HEAP_KIND_FRAME,
    HEAP_KIND_UNIT,
    HEAP_KIND_ASSERT,
  };

  inline bool isExpHeap() { return getHeapKind() == HEAP_KIND_EXPANDED; }
//   inline Heap* getParentHeap() { return mParentHeap; } // not part of ss
  inline void* getStartAddress() { return this; }

  // vtable at 0x0
  /* 0x08 */ virtual ~Heap();
  /* 0x0C */ virtual eHeapKind getHeapKind() const = 0;
  /* 0x10 */ virtual void initAllocator(Allocator* allocator, s32 align) = 0; 
  /* 0x14 */ virtual void* alloc(u32 size, s32 align) = 0;                    
  /* 0x18 */ virtual void free(void* block) = 0;                              
  /* 0x1C */ virtual void destroy() = 0;                                      
  /* 0x20 */ virtual u32 resizeForMBlock(void* block, u32 size) = 0;          
  /* 0x24 */ virtual u32 getAllocatableSize(s32 align) = 0;                   
  /* 0x28 */ virtual u32 adjust() = 0;                                        

  HEAP_PRIVATE :
      static nw4r::ut::List sHeapList;
  static OSMutex sRootMutex;
  static Heap* sCurrentHeap;
  static int sIsHeapListInitialized;

  static Heap* sAllocatableHeap;
  static ErrorCallback sErrorCallback;
  static HeapAllocCallback sAllocCallback;
  static void* sErrorCallbackArg;
  static void* sAllocCallbackArg;
  static class Thread* sAllocatableThread;
public:
  // members
  /* 0x10 */ MEMiHeapHead* mHeapHandle;
  /* 0x14 */ void* mParentBlock;
//   Heap* mParentHeap; // does not exist in ss
  enum HeapFlag {
    // tstDisableAllocation, enableAllocation, disableAllocation
    // setBit__Q23EGG12TBitFlag<Us>FUc
    HEAP_FLAG_LOCKED = (1 << 0)
  };
  /* 0x18 */ u16 mFlag;
  /* 0x1C */ nw4r::ut::Node mNode;
  /* 0x24 */ nw4r::ut::List mChildren;
  /* 0x30 */ const char* mName; // set to "NoName" in ctor

public:
  static void initialize();
  Heap(MEMiHeapHead* heapHandle);

private:
  inline Heap(const Heap&) {}

public:
  static void* alloc(u32 size, int align, Heap* heap);

  template <typename T> static T* alloc(u32 count, Heap* heap, int align = 4) {
    return reinterpret_cast<T*>(alloc(count * sizeof(T), align, heap));
  }
  template <typename T> static T* alloc(Heap* heap, int align = 4) {
    return reinterpret_cast<T*>(alloc(sizeof(T), align, heap));
  }

  Heap* findParentHeap();
  static Heap* findContainHeap(const void* memBlock);
  static void free(void* memBlock, Heap* heap);
  void dispose();
  static void dumpAll();
  Heap* becomeCurrentHeap();

public:
  static void* addOffset(void* begin, u32 size) {
    return reinterpret_cast<char*>(begin) + size;
  }

  inline void appendDisposer(Disposer* disposer) {
    nw4r::ut::List_Append(&mChildren, disposer);
  }
  inline void removeDisposer(Disposer* disposer) {
    nw4r::ut::List_Remove(&mChildren, disposer);
  }

  inline MEMiHeapHead* getHeapHandle() { return mHeapHandle; }

  static inline Heap* getCurrentHeap() { return sCurrentHeap; }

  inline int getArenaEnd() {
    return (int)mHeapHandle->end;
  }
};

} // namespace EGG

void* operator new(size_t size);
// __nwa(ulong, ulong)
void* operator new[](size_t size, u32 align);
// __nw(ulong, EGG::Heap *, int)
void* operator new(size_t size, EGG::Heap* heap, int align);
// __nwa(ulong)
void* operator new[](size_t size);
// __nwa(ulong, int)
void* operator new[](size_t size, int align);
// __nwa(ulong, EGG::Heap *, int)
void* operator new[](size_t size, EGG::Heap* heap, int align);
// __dl(void *)
void operator delete(void* p);
// __dla(void *)
void operator delete[](void*);

#undef HEAP_PRIVATE