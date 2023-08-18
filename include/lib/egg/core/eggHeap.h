#pragma once

// This file is ported from https://github.com/riidefi/mkw/blob/master/source/egg/core/eggHeap.hpp

#include <types.h>
#include <rvl/MEM.h>
#include <rvl/OS.h>
#include <egg/core/eggAllocator.h>
#include <egg/core/eggDisposer.h>
#include <egg/core/eggThread.h>
#include <nw4r/ut/List.h>
#include <egg/prim/eggBitFlag.h>

namespace EGG {

// class ExpHeap;
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

struct HeapFreeArg {
  u32 arg1; // Idk the args
  u32 arg2;
};
typedef void (*HeapFreeCallback)(void*);

typedef void (*HeapCreateCallback)(void*);
typedef void (*HeapDestroyCallback)(void*);

class Heap : public Disposer {
public:      
  enum eHeapKind {
    HEAP_KIND_NONE = 0,
    HEAP_KIND_EXPANDED,
    HEAP_KIND_FRAME,
    HEAP_KIND_UNIT,
    HEAP_KIND_ASSERT,
  };
  // vtable at 0x0 | 8056e950
  /* vt 0x08 | 804954c0 */ virtual ~Heap();
  /* vt 0x0C | 00000000 */ virtual eHeapKind getHeapKind() const = 0;
  /* vt 0x10 | 80495a40 */ virtual void initAllocator(Allocator* allocator, s32 align) = 0; 
  /* vt 0x14 | 00000000 */ virtual void* alloc(u32 size, s32 align) = 0;                    
  /* vt 0x18 | 00000000 */ virtual void free(void* block) = 0;                              
  /* vt 0x1C | 00000000 */ virtual void destroy() = 0;                                      
  /* vt 0x20 | 00000000 */ virtual u32 resizeForMBlock(void* block, u32 size) = 0; 
  /* vt 0x24 | 00000000 */ virtual u32 getTotalFreeSize() = 0;        
  /* vt 0x24 | 00000000 */ virtual u32 getAllocatableSize(s32 align) = 0;                   
  /* vt 0x28 | 00000000 */ virtual u32 adjust() = 0;

public:
  inline bool isExpHeap() { return getHeapKind() == HEAP_KIND_EXPANDED; }
//   inline Heap* getParentHeap() { return mParentHeap; } // not part of ss
  inline void* getStartAddress() { return this; }
                                  
public:
  // members
  /* 0x10 */ MEMiHeapHead* mHeapHandle;
  /* 0x14 */ void* mParentBlock;
  /* 0x18 */ TBitFlag<u16> mFlag;
  /* 0x1C */ nw4r::ut::Node mNode;
  /* 0x24 */ nw4r::ut::List mChildren;
  /* 0x30 */ const char* mName; // set to "NoName" in ctor

  enum HeapFlag {
    // tstDisableAllocation, enableAllocation, disableAllocation
    // setBit__Q23EGG12TBitFlag<Us>FUc
    HEAP_FLAG_LOCKED = (1 << 0)
  };

public:
  /* 804953f0 */ static void initialize();
  /* 80495430 */ Heap(MEMiHeapHead* heapHandle);
  /* 80495690 */ static Heap* findHeap(MEMiHeapHead* heapHandle);
  /* 80495730 */ Heap* findParentHeap();
  /* 80495780 */ static Heap* findContainHeap(const void* memBlock);
  /* 80495560 */ static void* alloc(u32 size, int align, Heap* heap);
  /* 804957c0 */ static void free(void* memBlock, Heap* heap);
  /* 80495830 */ void dispose();
  /* 804958a0 */ void dump();
  /* 804958b0 */ void dumpAll();
  /* 804959a0 */ Heap* becomeCurrentHeap();
  /* 80495a00 */ Heap* _becomeCurrentHeapWithoutLock();
  
public:
  template <typename T> static T* alloc(u32 count, Heap* heap, int align = 4) {
    return reinterpret_cast<T*>(alloc(count * sizeof(T), align, heap));
  }
  template <typename T> static T* alloc(Heap* heap, int align = 4) {
    return reinterpret_cast<T*>(alloc(sizeof(T), align, heap));
  }
  
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
  /* 80673ae8 */ static nw4r::ut::List sHeapList;
  /* 80673af8 */ static OSMutex sRootMutex;
  /* 80576740 */ static Heap* sCurrentHeap;
  /* 80576744 */ static int sIsHeapListInitialized;
  /* 80576748 */ static Heap* sAllocatableHeap;
  /* 8057674c */ static ErrorCallback sErrorCallback;
  /* 80576750 */ static HeapAllocCallback sAllocCallback;
  /* 80576754 */ static HeapFreeCallback sFreeCallback;
  /* 80576758 */ static void* sErrorCallbackArg;
  /* 8057675c */ static void* sAllocCallbackArg;
  /* 80576760 */ static void* sFreeCallbackArg;
  /* 80576764 */ static HeapCreateCallback sCreateCallback;
  /* 80576764 */ static HeapDestroyCallback sDestroyCallback;
};

} // namespace EGG

/* 80495a60 */ void* operator new(size_t, void* p);
/* 80495a70 */ void* operator new(size_t size, EGG::Heap* heap, u32 align);
/* 80495a80 */ void* operator new(size_t size, EGG::Allocator* alloc);
/* 80495a90 */ void* operator new[](size_t size, u32 align);
/* 80495aa0 */ void* operator new[](size_t size, EGG::Heap* heap, int align);
