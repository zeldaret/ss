#ifndef RVL_SDK_MEM_HEAP_COMMON_H
#define RVL_SDK_MEM_HEAP_COMMON_H
#include "rvl/MEM/mem_list.h"
#include "rvl/OS.h"
#include <MSL_C/string.h>
#include <common.h>

// #include "string.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MEM_HEAP_OPT_CLEAR_ALLOC = (1 << 0),
    MEM_HEAP_OPT_DEBUG_FILL = (1 << 1),
    MEM_HEAP_OPT_CAN_LOCK = (1 << 2)
} MEMHeapOpt;

typedef struct MEMiHeapHead {
    u32 magic;     // at 0x0
    MEMLink link;  // at 0x4
    MEMList list;  // at 0xC
    u8 *start;     // at 0x18
    u8 *end;       // at 0x1C
    OSMutex mutex; // at 0x20

    union {
        u32 attribute;
        struct {
            u32 attribute_0_24 : 24;
            u32 opt : 8;
        };
    }; // at 0x38
} MEMiHeapHead;

void MEMiInitHeapHead(MEMiHeapHead *heap, u32 magic, void *start, void *end, u16 opt);
void MEMiFinalizeHeap(MEMiHeapHead *heap);
MEMiHeapHead *MEMFindContainHeap(const void *memBlock);

static inline int GetUIntPtr(const void *p) {
    return (int)p;
}

static inline void *AddU32ToPtr(const void *p, u32 ofs) {
    return (void *)(GetUIntPtr(p) + ofs);
}

static inline void *SubU32ToPtr(const void *p, u32 ofs) {
    return (void *)(GetUIntPtr(p) - ofs);
}

static inline const void *AddU32ToCPtr(const void *p, u32 ofs) {
    return (const void *)(GetUIntPtr(p) + ofs);
}

static inline const void *SubU32ToCPtr(const void *p, u32 ofs) {
    return (const void *)(GetUIntPtr(p) - ofs);
}

static inline s32 GetOffsetFromPtr(const void *start, const void *end) {
    return GetUIntPtr(end) - GetUIntPtr(start);
}

static inline u16 GetOptForHeap(const MEMiHeapHead *heap) {
    return heap->opt;
}

static inline void SetOptForHeap(MEMiHeapHead *heap, u16 opt) {
    heap->opt = (u8)opt;
}

static inline void LockHeap(MEMiHeapHead *heap) {
    if (GetOptForHeap(heap) & MEM_HEAP_OPT_CAN_LOCK) {
        OSLockMutex(&heap->mutex);
    }
}

static void UnlockHeap(MEMiHeapHead* heap) {
    if (GetOptForHeap(heap) & MEM_HEAP_OPT_CAN_LOCK) {
        OSUnlockMutex(&heap->mutex);
    }
}

static void FillAllocMemory(MEMiHeapHead* heap, void* memBlock, u32 size) {
    if (GetOptForHeap(heap) & MEM_HEAP_OPT_CLEAR_ALLOC) {
        memset(memBlock, 0, size);
    }
}

static s32 MEMGetHeapTotalSize(MEMiHeapHead* heap) {
    return GetOffsetFromPtr(heap, heap->end);
}

static void FillAllocMemory(MEMiHeapHead *heap, void *memBlock, u32 size);

#ifdef __cplusplus
}
#endif
#endif
