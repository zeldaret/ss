#ifndef RVL_SDK_MEM_EXP_HEAP_H
#define RVL_SDK_MEM_EXP_HEAP_H
#include "common.h"
#ifdef __cplusplus
extern "C" {
#endif

#define MEM_EXP_HEAP_MIN_SIZE (sizeof(MEMiHeapHead) + sizeof(MEMiExpHeapHead) + sizeof(MEMiExpHeapMBlock) + 4)

// Forward declarations
typedef struct MEMiHeapHead;

typedef enum {
    MEM_EXP_HEAP_ALLOC_FAST, //!< When allocating memory blocks, take the first
                             //!< usable found block rather than trying to
                             //!< find a more optimal block
} MEMiExpHeapAllocMode;

typedef struct MEMiExpHeapMBlock {
    u16 state; // at 0x0
    union {
        u16 settings;
        struct {
            u16 allocDir : 1;
            u16 align : 7;
            u16 group : 8;
        };
    };                              // at 0x2
    u32 size;                       // at 0x4
    struct MEMiExpHeapMBlock *prev; // at 0x8
    struct MEMiExpHeapMBlock *next; // at 0xC
} MEMiExpHeapMBlock;

typedef struct MEMiExpHeapMBlockList {
    MEMiExpHeapMBlock *head; // at 0x0
    MEMiExpHeapMBlock *tail; // at 0x4
} MEMiExpHeapMBlockList;

// Placed in heap after base heap head
typedef struct MEMiExpHeapHead {
    MEMiExpHeapMBlockList freeMBlocks; // at 0x0
    MEMiExpHeapMBlockList usedMBlocks; // at 0x8
    u16 group;                         // at 0x10
    union {
        u16 SHORT_0x12;
        struct {
            u16 SHORT_0x12_0_15 : 15;
            u16 allocMode : 1;
        };
    }; // at 0x12
} MEMiExpHeapHead;

struct MEMiHeapHead *MEMCreateExpHeapEx(void *start, u32 size, u16 opt);
struct MEMiHeapHead *MEMDestroyExpHeap(struct MEMiHeapHead *heap);
void *MEMAllocFromExpHeapEx(struct MEMiHeapHead *heap, u32 size, s32 align);
u32 MEMResizeForMBlockExpHeap(struct MEMiHeapHead *heap, void *memBlock, u32 size);
void MEMFreeToExpHeap(struct MEMiHeapHead *heap, void *memBlock);
u32 MEMGetAllocatableSizeForExpHeap(struct MEMiHeapHead *heap);
u32 MEMGetTotalFreeSizeForExpHeap(struct MEMiHeapHead *heap);
u32 MEMGetAllocatableSizeForExpHeapEx(struct MEMiHeapHead *heap, s32 align);
void MEMSetGroupIdForExpHeap(struct MEMiHeapHead *mHeapHandle, u16 groupId);
u32 MEMAdjustExpHeap(struct MEMiHeapHead *heap);
u32 MEMGetSizeForMBlockExpHeap(const void *block);
void MEMSetAllocModeForExpHeap(struct MEMiHeapHead *heap, u16 allocMode);

static inline struct MEMiHeapHead *MEMCreateExpHeap(void *start, u32 size) {
    return MEMCreateExpHeapEx(start, size, 0);
}

static inline void *MEMAllocFromExpHeap(struct MEMiHeapHead *heap, u32 size) {
    return MEMAllocFromExpHeapEx(heap, size, 4);
}
// This doesn't appear to be inline in SS yet
/*
static inline u32 MEMGetAllocatableSizeForExpHeap(struct MEMiHeapHead *heap) {
    return MEMGetAllocatableSizeForExpHeapEx(heap, 4);
}*/

#ifdef __cplusplus
}
#endif
#endif
