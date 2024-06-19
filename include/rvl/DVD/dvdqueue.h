#ifndef RVL_SDK_DVD_QUEUE_H
#define RVL_SDK_DVD_QUEUE_H
#include <rvl/DVD/dvd.h>
#include <common.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DVD_PRIO_HIGHEST,
    DVD_PRIO_HIGH,
    DVD_PRIO_MEDIUM,
    DVD_PRIO_LOW,

    DVD_PRIO_MAX,
} DVDQueuePriority;

void __DVDClearWaitingQueue(void);
BOOL __DVDPushWaitingQueue(s32 prio, DVDCommandBlock *block);
DVDCommandBlock *__DVDPopWaitingQueue(void);
BOOL __DVDCheckWaitingQueue(void);
DVDCommandBlock *__DVDGetNextWaitingQueue(void);
BOOL __DVDDequeueWaitingQueue(const DVDCommandBlock *block);

#ifdef __cplusplus
}
#endif
#endif
