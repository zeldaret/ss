#ifndef RVL_SDK_OS_ALARM_H
#define RVL_SDK_OS_ALARM_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSAlarm;
typedef struct OSContext;

typedef void (*OSAlarmHandler)(struct OSAlarm* alarm, struct OSContext* ctx);

typedef struct OSAlarm {
    OSAlarmHandler handler; // at 0x0
    u32 tag;                // at 0x4
    s64 end;
    struct OSAlarm* prev; // at 0x10
    struct OSAlarm* next; // at 0x14
    s64 period;           // at 0x18
    s64 start;            // at 0x20
    void* userData;       // at 0x28
} OSAlarm;

typedef struct OSAlarmQueue {
    OSAlarm* head; // at 0x0
    OSAlarm* tail; // at 0x4
} OSAlarmQueue;

void __OSInitAlarm(void);
void OSCreateAlarm(OSAlarm* alarm);
void OSSetAlarm(OSAlarm* alarm, s64 tick, OSAlarmHandler handler);
void OSSetPeriodicAlarm(OSAlarm* alarm, s64 tick, s64 period,
                        OSAlarmHandler handler);
void OSCancelAlarm(OSAlarm* alarm);
void OSSetAlarmTag(OSAlarm* alarm, u32 tag);
void OSSetAlarmUserData(OSAlarm* alarm, void* userData);
void* OSGetAlarmUserData(const OSAlarm* alarm);

#ifdef __cplusplus
}
#endif
#endif
