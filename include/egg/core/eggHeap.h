#ifndef EGG_HEAP_H
#define EGG_HEAP_H

// This file is ported from https://github.com/riidefi/mkw/blob/master/source/egg/core/eggHeap.hpp

#include "common.h"
#include "egg/core/eggDisposer.h"
#include "egg/egg_types.h"
#include "egg/prim/eggBitFlag.h"

#include "nw4r/ut.h"

#include "rvl/MEM.h"
#include "rvl/OS.h"

namespace EGG {

// class ExpHeap;
class Allocator;

struct HeapAllocArg {
    void *userArg; // 00
    u32 size;      // 04
    int align;     // 08
    void *ptr;     // 0C the allocated ptr
    Heap *heap;    // 10 heap to allocate in

    inline HeapAllocArg() : userArg(0), size(0), align(0), heap(nullptr), ptr(nullptr) {}
};
typedef void (*HeapAllocCallback)(HeapAllocArg *arg);

struct HeapErrorArg {
    const char *msg;
    void *userdata;

    inline HeapErrorArg() {}
};
typedef void (*ErrorCallback)(HeapErrorArg *);

struct HeapFreeArg {
    void *userArg;
    void *ptr;
    Heap *heap;
};
typedef void (*HeapFreeCallback)(HeapFreeArg *);

typedef void (*HeapCreateCallback)(void *);
typedef void (*HeapDestroyCallback)(void *);

class Heap : public Disposer {
public:
    enum eHeapKind {
        HEAP_KIND_NONE = 0,
        HEAP_KIND_EXPANDED,
        HEAP_KIND_FRAME,
        HEAP_KIND_UNIT,
        HEAP_KIND_ASSERT,
    };
    // vtable at 0x0
    /* vt 0x08 */ virtual ~Heap();
    /* vt 0x0C */ virtual eHeapKind getHeapKind() const = 0;
    /* vt 0x10 */ virtual void initAllocator(Allocator *allocator, s32 align);
    /* vt 0x14 */ virtual void *alloc(u32 size, s32 align) = 0;
    /* vt 0x18 */ virtual void free(void *block) = 0;
    /* vt 0x1C */ virtual void destroy() = 0;
    /* vt 0x20 */ virtual u32 resizeForMBlock(void *block, u32 size) = 0;
    /* vt 0x24 */ virtual u32 getTotalFreeSize() = 0;
    /* vt 0x28 */ virtual u32 getAllocatableSize(s32 align) = 0;
    /* vt 0x2C */ virtual u32 adjust() = 0;

public:
    void setName(const char *name) {
        mName = name;
    }
    inline bool isExpHeap() {
        return getHeapKind() == HEAP_KIND_EXPANDED;
    }
    // inline Heap* getParentHeap() { return mParentHeap; } // not part of ss
    inline void *getStartAddress() {
        return this;
    }

    static inline EGG::FrmHeap *toFrmHeap(EGG::Heap *heap) {
        if (heap != nullptr && heap->getHeapKind() == HEAP_KIND_FRAME) {
            return (FrmHeap *)heap;
        }
        return nullptr;
    }

    static inline EGG::ExpHeap *toExpHeap(EGG::Heap *heap) {
        if (heap != nullptr && heap->getHeapKind() == HEAP_KIND_EXPANDED) {
            return (ExpHeap *)heap;
        }
        return nullptr;
    }

public:
    // members
    /* 0x10 */ MEMiHeapHead *mHeapHandle;
    /* 0x14 */ void *mParentBlock;
    /* 0x18 */ TBitFlag<u16> mFlag;
    /* 0x1C */ nw4r::ut::Node mNode;
    /* 0x24 */ nw4r::ut::List mChildren;
    /* 0x30 */ const char *mName; // set to "NoName" in ctor

    enum HeapFlag {
        // tstDisableAllocation, enableAllocation, disableAllocation
        // setBit__Q23EGG12TBitFlag<Us>FUc
        HEAP_FLAG_LOCKED = (1 << 0)
    };

public:
    static void initialize();
    Heap(MEMiHeapHead *heapHandle);
    static Heap *findHeap(MEMiHeapHead *heapHandle);
    Heap *findParentHeap();
    static Heap *findContainHeap(const void *memBlock);
    static void *alloc(size_t size, int align, Heap *heap);
    static void free(void *memBlock, Heap *heap);
    void dispose();
    void dump();
    static void dumpAll();
    Heap *becomeCurrentHeap();
    Heap *_becomeCurrentHeapWithoutLock();

    void disableAllocation() {
        mFlag.set(HEAP_FLAG_LOCKED);
    }

public:
    template <typename T>
    static T *alloc(u32 count, Heap *heap, int align = 4) {
        return reinterpret_cast<T *>(alloc(count * sizeof(T), align, heap));
    }
    template <typename T>
    static T *alloc(Heap *heap, int align = 4) {
        return reinterpret_cast<T *>(alloc(sizeof(T), align, heap));
    }

public:
    static void *addOffset(void *begin, u32 size) {
        return reinterpret_cast<char *>(begin) + size;
    }

    inline void appendDisposer(Disposer *disposer) {
        nw4r::ut::List_Append(&mChildren, disposer);
    }
    inline void removeDisposer(Disposer *disposer) {
        nw4r::ut::List_Remove(&mChildren, disposer);
    }

    inline MEMiHeapHead *getHeapHandle() {
        return mHeapHandle;
    }

    static inline Heap *getCurrentHeap() {
        return sCurrentHeap;
    }

    inline int getArenaEnd() {
        return (int)mHeapHandle->end;
    }

    inline const char *getName() const {
        return mName;
    }

    struct AllocCallbackBundle {
        HeapAllocCallback AllocCallback;
        void *AllocCallbackArg;
    };

    struct FreeCallbackBundle {
        HeapFreeCallback FreeCallback;
        void *FreeCallbackArg;
    };

    static inline AllocCallbackBundle setAllocCallback(HeapAllocCallback AllocCallback, void *AllocCallbackArg) {
        AllocCallbackBundle ret;

        ret.AllocCallback = sAllocCallback;
        ret.AllocCallbackArg = sAllocCallbackArg;
        sAllocCallback = AllocCallback;
        sAllocCallbackArg = AllocCallbackArg;

        return ret;
    }

    static inline FreeCallbackBundle setFreeCallback(HeapFreeCallback FreeCallback, void *FreeCallbackArg) {
        FreeCallbackBundle ret;

        ret.FreeCallback = sFreeCallback;
        ret.FreeCallbackArg = sFreeCallbackArg;
        sFreeCallback = FreeCallback;
        sFreeCallbackArg = FreeCallbackArg;

        return ret;
    }

    static nw4r::ut::List sHeapList;
    static OSMutex sRootMutex;
    static Heap *sCurrentHeap;
    static int sIsHeapListInitialized;
    static Heap *sAllocatableHeap;
    static ErrorCallback sErrorCallback;
    static HeapAllocCallback sAllocCallback;
    static HeapFreeCallback sFreeCallback;
    static void *sErrorCallbackArg;
    static void *sAllocCallbackArg;
    static void *sFreeCallbackArg;
    static HeapCreateCallback sCreateCallback;
    static HeapDestroyCallback sDestroyCallback;
};
} // namespace EGG

void *operator new(size_t, void *p);
void *operator new(size_t size, EGG::Heap *heap, int align = 4);
void *operator new(size_t size, EGG::Allocator *alloc);
void *operator new[](size_t size, int align);
void *operator new[](size_t size, EGG::Heap *heap, int align = 4);

#endif
