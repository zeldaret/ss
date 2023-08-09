#ifndef RVL_SDK_MEM_FRAME_HEAP_H
#define RVL_SDK_MEM_FRAME_HEAP_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct MEMiHeapHead;

// Specify how to free memory
typedef enum {
    MEM_FRM_HEAP_FREE_TO_HEAD = (1 << 0),
    MEM_FRM_HEAP_FREE_TO_TAIL = (1 << 1)
} MEMiFrmFreeFlag;

typedef struct MEMiFrmHeapState {
    u32 id;                        // at 0x0
    u8* head;                      // at 0x4
    u8* tail;                      // at 0x8
    struct MEMiFrmHeapState* next; // at 0xC
} MEMiFrmHeapState;

// Placed in heap after base heap head
typedef struct MEMiFrmHeapHead {
    u8* head;                 // at 0x0
    u8* tail;                 // at 0x4
    MEMiFrmHeapState* states; // at 0x8
} MEMiFrmHeapHead;

struct MEMiHeapHead* MEMCreateFrmHeapEx(void* start, u32 size, u16 opt);
struct MEMiHeapHead* MEMDestroyFrmHeap(struct MEMiHeapHead* heap);
void* MEMAllocFromFrmHeapEx(struct MEMiHeapHead* heap, u32 size, s32 align);
void MEMFreeToFrmHeap(struct MEMiHeapHead* heap, u32 flags);
u32 MEMGetAllocatableSizeForFrmHeapEx(struct MEMiHeapHead* heap, s32 align);
BOOL MEMRecordStateForFrmHeap(struct MEMiHeapHead* heap, u32 id);
BOOL MEMFreeByStateToFrmHeap(struct MEMiHeapHead* heap, u32 id);
u32 MEMAdjustFrmHeap(struct MEMiHeapHead* heap);
u32 MEMResizeForMBlockFrmHeap(struct MEMiHeapHead* heap, void* memBlock,
                              u32 size);

static inline struct MEMiHeapHead* MEMCreateFrmHeap(void* start, u32 size) {
    return MEMCreateFrmHeapEx(start, size, 0);
}

static inline void* MEMAllocFromFrmHeap(struct MEMiHeapHead* heap, u32 size) {
    return MEMAllocFromFrmHeapEx(heap, size, 4);
}

static inline u32 MEMGetAllocatableSizeForFrmHeap(struct MEMiHeapHead* heap) {
    return MEMGetAllocatableSizeForFrmHeapEx(heap, 4);
}

#ifdef __cplusplus
}
#endif
#endif
