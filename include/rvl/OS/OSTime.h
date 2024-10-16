#ifndef RVL_SDK_OS_TIME_H
#define RVL_SDK_OS_TIME_H
#include "common.h"
#include "rvl/OS/OSHardware.h" // IWYU pragma: export


#ifdef __cplusplus
extern "C" {
#endif

typedef s64 OSTime;
typedef u32 OSTick;

// Time base frequency = 1/4 bus clock
#define OS_TIME_SPEED (OS_BUS_CLOCK_SPEED / 4)

// OS time -> Real time
#define OS_TICKS_TO_SEC(x) ((x) / (OS_TIME_SPEED))
#define OS_TICKS_TO_MSEC(x) ((x) / (OS_TIME_SPEED / 1000))
#define OS_TICKS_TO_USEC(x) (((x) * 8) / (OS_TIME_SPEED / 125000))
#define OS_TICKS_TO_NSEC(x) (((x) * 8000) / (OS_TIME_SPEED / 125000))

// Real time -> OS time
#define OS_SEC_TO_TICKS(x) ((x) * (OS_TIME_SPEED))
#define OS_MSEC_TO_TICKS(x) ((x) * (OS_TIME_SPEED / 1000))
#define OS_USEC_TO_TICKS(x) ((x) * (OS_TIME_SPEED / 125000) / 8)
#define OS_NSEC_TO_TICKS(x) ((x) * (OS_TIME_SPEED / 125000) / 8000)

typedef struct OSCalendarTime {
    s32 sec;       // at 0x0
    s32 min;       // at 0x4
    s32 hour;      // at 0x8
    s32 month_day; // at 0xC
    s32 month;     // at 0x10
    s32 year;      // at 0x14
    s32 week_day;  // at 0x18
    s32 year_day;  // at 0x1C
    s32 msec;      // at 0x20
    s32 usec;      // at 0x24
} OSCalendarTime;

OSTime OSGetTime(void);
OSTick OSGetTick(void);

OSTime __OSGetSystemTime(void);
OSTime __OSTimeToSystemTime(OSTime time);

void OSTicksToCalendarTime(OSTime time, OSCalendarTime *cal);
OSTime OSCalendarTimeToTicks(const OSCalendarTime *cal);

#ifdef __cplusplus
}
#endif
#endif
