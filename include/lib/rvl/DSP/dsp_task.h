#ifndef RVL_SDK_DSP_TASK_H
#define RVL_SDK_DSP_TASK_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct DSPTask;
typedef struct OSContext;

typedef enum {
    DSP_TASK_ACTIVE = (1 << 0),
    DSP_TASK_CANCELED = (1 << 1),
} DSPTaskFlags;

typedef enum {
    DSP_TASK_STATE_0,
    DSP_TASK_STATE_1,
    DSP_TASK_STATE_2,
    DSP_TASK_STATE_3,
} DSPTaskState;

typedef void (*DSPTaskCallback)(struct DSPTask* task);

typedef struct DSPTask {
    u32 state;          // at 0x0
    u32 prio;           // at 0x4
    u32 flags;          // at 0x8
    void* iramMmemAddr; // at 0xC
    u32 iramMmemLen;    // at 0x10
    void* iramDspAddr;  // at 0x14
    u32 iramDspLen;     // at 0x18
    u32 dramMmemLen;    // at 0x1C
    UNKWORD WORD_0x20;
    u16 startVector;                 // at 0x24
    u16 resumeVector;                // at 0x26
    DSPTaskCallback initCallback;    // at 0x28
    DSPTaskCallback resumeCallback;  // at 0x2C
    DSPTaskCallback doneCallback;    // at 0x30
    DSPTaskCallback requestCallback; // at 0x34
    struct DSPTask* next;            // at 0x38
    struct DSPTask* prev;            // at 0x3C
} DSPTask;

extern BOOL __DSP_rude_task_pending;
extern DSPTask* __DSP_rude_task;
extern DSPTask* __DSP_tmp_task;
extern DSPTask* __DSP_last_task;
extern DSPTask* __DSP_first_task;
extern DSPTask* __DSP_curr_task;

void __DSPHandler(s16 intr, struct OSContext* ctx);
void __DSP_exec_task(DSPTask* task1, DSPTask* task2);
void __DSP_boot_task(DSPTask* task);
void __DSP_insert_task(DSPTask* task);
void __DSP_remove_task(DSPTask* task);

#ifdef __cplusplus
}
#endif
#endif
