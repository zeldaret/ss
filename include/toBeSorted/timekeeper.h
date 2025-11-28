#ifndef TIMEKEEPER_H
#define TIMEKEEPER_H

#include "common.h"

#define TIME_CONVERT(min, sec, msec) ((min * 60 * 1000) + (sec * 1000) + (msec * 10))

class Timekeeper {
public:
    Timekeeper();
    virtual ~Timekeeper();

    virtual s32 vt_0x0C(); ///< Init
    virtual s32 vt_0x10(); ///< Delete
    virtual s32 vt_0x14();
    virtual s32 doCount();
    virtual s32 vt_0x1C();

    s32 getTime() const;
    void initUpCounter();

    /* 0x08 */ s64 mTime;
    /* 0x10 */ u32 mCountingUp;
    /* 0x14 */ u32 mCountingDown;
    /* 0x1C */ bool field_0x1C;
};

#endif
