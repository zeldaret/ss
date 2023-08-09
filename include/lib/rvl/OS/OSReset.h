#ifndef RVL_SDK_OS_RESET_H
#define RVL_SDK_OS_RESET_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef BOOL (*OSShutdownFunction)(u32 pass, u32 event);

typedef enum { OS_SD_PASS_FIRST, OS_SD_PASS_SECOND } OSShutdownPass;

typedef enum {
    OS_SD_EVENT_SHUTDOWN = 2,
    OS_SD_EVENT_RESTART = 4,
    OS_SD_EVENT_RETURN_TO_MENU = 5,
    OS_SD_EVENT_LAUNCH_APP = 6,
} OSShutdownEvent;

typedef struct OSShutdownFunctionInfo {
    OSShutdownFunction func;             // at 0x0
    u32 prio;                            // at 0x4
    struct OSShutdownFunctionInfo* next; // at 0x8
    struct OSShutdownFunctionInfo* prev; // at 0xC
} OSShutdownFunctionInfo;

typedef struct OSShutdownFunctionQueue {
    OSShutdownFunctionInfo* head; // at 0x0
    OSShutdownFunctionInfo* tail; // at 0x4
} OSShutdownFunctionQueue;

void OSRegisterShutdownFunction(OSShutdownFunctionInfo* info);
BOOL __OSCallShutdownFunctions(u32 pass, u32 event);
void __OSShutdownDevices(u32 event);
void __OSGetDiscState(u8* out);
void OSShutdownSystem(void);
void OSReturnToMenu(void);
u32 OSGetResetCode(void);
void OSResetSystem(u32 arg0, u32 arg1, u32 arg2);

#ifdef __cplusplus
}
#endif
#endif
