#ifndef RVL_SDK_OS_THREAD_H
#define RVL_SDK_OS_THREAD_H
#include <lib/rvl/OS/OSContext.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define OS_PRIORITY_MIN 0
#define OS_PRIORITY_MAX 31

#define OS_THREAD_STACK_MAGIC 0xDEADBABE

typedef enum {
    OS_THREAD_STATE_EXITED = 0,
    OS_THREAD_STATE_READY = 1,
    OS_THREAD_STATE_RUNNING = 2,
    OS_THREAD_STATE_SLEEPING = 4,
    OS_THREAD_STATE_MORIBUND = 8
} OSThreadState;

typedef enum { OS_THREAD_DETACHED = (1 << 0) } OSThreadFlag;

typedef struct OSThreadQueue {
    struct OSThread* head; // at 0x0
    struct OSThread* tail; // at 0x4
} OSThreadQueue;

typedef struct OSMutexQueue {
    struct OSMutex* head; // at 0x0
    struct OSMutex* tail; // at 0x4
} OSMutexQueue;

typedef struct OSThread {
    OSContext context;
    u16 state;                   // at 0x2C8
    u16 flags;                   // at 0x2CA
    s32 suspend;                 // at 0x2CC
    s32 priority;                // at 0x2D0
    s32 base;                    // at 0x2D4
    u32 val;                     // at 0x2D8
    OSThreadQueue* queue;        // at 0x2DC
    struct OSThread* next;       // at 0x2E0
    struct OSThread* prev;       // at 0x2E4
    OSThreadQueue joinQueue;     // at 0x2E8
    struct OSMutex* mutex;       // at 0x2F0
    OSMutexQueue mutexQueue;     // at 0x2F4
    struct OSThread* nextActive; // at 0x2FC
    struct OSThread* prevActive; // at 0x300
    u32* stackBegin;             // at 0x304
    u32* stackEnd;               // at 0x308
    s32 error;                   // at 0x30C
    void* specific[2];           // at 0x310
} OSThread;

typedef void (*OSSwitchThreadCallback)(OSThread* currThread,
                                       OSThread* newThread);
typedef void* (*OSThreadFunc)(void* arg);

OSSwitchThreadCallback
OSSetSwitchThreadCallback(OSSwitchThreadCallback callback);
void __OSThreadInit(void);
void OSSetCurrentThread(OSThread* thread);
void OSInitMutexQueue(OSMutexQueue* queue);
void OSInitThreadQueue(OSThreadQueue* queue);
OSThread* OSGetCurrentThread(void);
BOOL OSIsThreadTerminated(OSThread* thread);
s32 OSDisableScheduler(void);
s32 OSEnableScheduler(void);
s32 __OSGetEffectivePriority(OSThread* thread);
void __OSPromoteThread(OSThread* thread, s32 prio);
void __OSReschedule(void);
void OSYieldThread(void);
BOOL OSCreateThread(OSThread* thread, OSThreadFunc func, void* funcArg,
                    void* stackBegin, u32 stackSize, s32 prio, u16 flags);
void OSExitThread(OSThread* thread);
void OSCancelThread(OSThread* thread);
BOOL OSJoinThread(OSThread* thread, void* val);
void OSDetachThread(OSThread* thread);
s32 OSResumeThread(OSThread* thread);
s32 OSSuspendThread(OSThread* thread);
void OSSleepThread(OSThreadQueue* queue);
void OSWakeupThread(OSThreadQueue* queue);
BOOL OSSetThreadPriority(OSThread* thread, s32 prio);
void OSClearStack(u8 val);
void OSSleepTicks(s64 ticks);

#ifdef __cplusplus
}
#endif
#endif
