#ifndef RVL_SDK_OS_MESSAGE_H
#define RVL_SDK_OS_MESSAGE_H
#include <lib/rvl/OS/OSThread.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// General-purpose typedef
typedef void* OSMessage;

typedef enum { OS_MSG_PERSISTENT = (1 << 0) } OSMessageFlags;

typedef struct OSMessageQueue {
    OSThreadQueue sendQueue; // at 0x0
    OSThreadQueue recvQueue; // at 0x8
    OSMessage* buffer;       // at 0x10
    s32 capacity;            // at 0x14
    s32 front;               // at 0x18
    s32 size;                // at 0x1C
} OSMessageQueue;

void OSInitMessageQueue(OSMessageQueue* queue, OSMessage* buffer, s32 capacity);
BOOL OSSendMessage(OSMessageQueue* queue, OSMessage mesg, u32 flags);
BOOL OSReceiveMessage(OSMessageQueue* queue, OSMessage* mesg, u32 flags);
BOOL OSJamMessage(OSMessageQueue* queue, OSMessage mesg, u32 flags);

#ifdef __cplusplus
}
#endif
#endif
