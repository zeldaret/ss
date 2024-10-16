#ifndef EVENT_H
#define EVENT_H

#include "common.h"

class Event {
public:
    void *unk;
    u32 eventFlags;
    s32 roomid;
    s32 toolDataid;
    char eventName[32];
    void *eventZevData;
    void *callback1;
    void *callback2;

public:
    /** 0x800A0C40 */
    Event(char *eventName, u32 unk, u32 eventFlags, void *callback1, void *callback2);
    Event(u32 eventId, s32 roomId, u32 eventFlags, void *callback1, void *callback2);
    /** 0x800A1240 */
    virtual ~Event();
};

#endif
