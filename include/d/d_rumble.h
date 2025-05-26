#ifndef D_RUMBLE_H
#define D_RUMBLE_H

#include "common.h"

class dRumbleEntry_c {
    friend class dRumble_c;

public:
    dRumbleEntry_c(s32 length, u32 bits, f32 intensity);

private:
    /* 0x00 */ s32 mLength;
    /* 0x04 */ s32 mBits;
    /* 0x08 */ f32 mIntensity;
};

class dRumbleIdx_c {
public:
    ~dRumbleIdx_c() {
        stop();
    }

    /* Queues the entry into the Rumble Manager */
    bool start(const dRumbleEntry_c &entry, u32 flags);

    /* Removes the entry from the Rumble Manager*/
    void stop();

    bool isActive() const {
        return mIdx != -1;
    }
    void setIdx(s32 idx) {
        mIdx = idx;
    }
    s32 getIdx() const {
        return mIdx;
    }

private:
    s32 mIdx;
};

/**
 * Made-up name.
 * Size: 0x54
 */
class dRumble_c {
public:
    enum Flags_c {
        FLAG_SLOT0 = 0x1,
        FLAG_SLOT1 = 0x2,
        FLAG_ACTIVE = 0x10,
        FLAG_INITIALIZE = 0x200,
    };

    struct RumbleData {
        /* 0x00 */ s32 mLength;
        /* 0x04 */ s32 mBitsLeft;
        /* 0x08 */ s32 mRumbleBits;
        /* 0x0C */ u32 mFlags;
        /* 0x10 */ f32 mIntensity;
    };

public:
    /** Generates the single instance */
    static void create();

    /** Deletes the single instance */
    static void remove();

    /** Runs the rumble instances */
    static void execute();

    /** inserts a rumble instance into the manager. returns the index */
    static s32 start(const dRumbleEntry_c &entry, u32 flags);

    /** Stops one of the data instances (sets length to 0). -1 means to stop all */
    static void stop(s32 idx);

    virtual ~dRumble_c();

    /** Rumble Presets */
    static dRumbleEntry_c sRumblePreset0;
    static dRumbleEntry_c sRumblePreset1;
    static dRumbleEntry_c sRumblePreset2;
    static dRumbleEntry_c sRumblePreset3;
    static dRumbleEntry_c sRumblePreset4;
    static dRumbleEntry_c sRumblePreset5;
    static dRumbleEntry_c sRumblePreset6;
    static dRumbleEntry_c sRumblePreset7;
    static dRumbleEntry_c sRumblePreset8;
    static dRumbleEntry_c sRumblePreset9;
    static dRumbleEntry_c sRumblePreset10;

private:
    RumbleData mRumbleData[4];

private:
    dRumble_c();
    static dRumble_c *spInstance;
};

#endif
